//********************************************************************************
//
// DemoRender.cpp
//
// デモ用モデルレンダリング
//
// Date: 2019/07/30 更新
//
//********************************************************************************

//================================================================================
// インクルード
//================================================================================
#include "DemoRender.hpp"

using namespace FileConst;

const float MODE_SELECT_DIST = -2.0f;
const float START_SELECT_DIST = 2.0f;

//================================================================================
// クラス定義
//================================================================================

//--------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------
CDemoRender::CDemoRender()
	: m_pCamera(new CCamera)
	, m_pField(new CXFileModel)
	, m_pPlayer(new CXFileModel)
	, m_pSky(new CXFileModel)
	, m_vecLockAt(Vector3(0.0f, 1.5f, 0.0f))
	, m_vecAngle(Vector3(0.0f, Mathf::PI, 0.0f))
	, m_cameraXDist(0.0f)
	, m_cameraZDist(5.0f)
	, m_baseAngle(0.0f)
	, m_demoAngle(0.0f)
	, m_meshAlpha(1.0f)
	, m_isChangeMesh(false)
	, m_isChangeAlpha(false)

{
}

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
CDemoRender::~CDemoRender()
{
	SafeDelete(m_pCamera);
	SafeDelete(m_pField);
	SafeDelete(m_pPlayer);
	SafeDelete(m_pSky);
}

//--------------------------------------------------------------------------------
// ファイル読み込み
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CDemoRender::LoadFile(CSystem * pSystem)
{
	pSystem->LoadXFile(MODEL_PLAYER,  "Resource/Models/Player/player.x");
	pSystem->LoadXFile(MODEL_SKYDOME, "Resource/Models/Skydome/skydome.x");
	pSystem->LoadXFile(MODEL_FIELD1,  "Resource/Models/Stage/Field4.x");
	pSystem->LoadXFile(MODEL_FIELD2,  "Resource/Models/Stage/Field2.x");
	pSystem->LoadXFile(MODEL_FIELD3,  "Resource/Models/Stage/Field3.x");
}

//--------------------------------------------------------------------------------
// 初期化
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CDemoRender::Initialize(CSystem * pSystem)
{
	m_pCamera->Initialize(
		pSystem->GetDevice(), 
		Mathf::ToRadian(60),
		16.0f / 9.0f,
		0.1f, 
		1000.0f);

	m_pField->SetModel(pSystem->GetXFile(MODEL_FIELD3));
	m_pSky->SetModel(pSystem->GetXFile(MODEL_SKYDOME));

	m_pPlayer->SetModel(pSystem->GetXFile(MODEL_PLAYER));
	m_pPlayer->SetAdvanceTime(1.0 / 60.0);
	m_timer.SetTimer(10000);
}

//--------------------------------------------------------------------------------
// 更新
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CDemoRender::Update(CSystem * pSystem)
{
	m_isChangeMesh = m_timer.IsTimeOut(pSystem->GetDeltaTime());
	
	ChangeStage(pSystem);

	UpdateMeshAlpha();

	CDemoRender::SetCameraDist();

	CDemoRender::SetWorldTransform();
}

//--------------------------------------------------------------------------------
// 描画
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CDemoRender::ZDraw(CSystem * pSystem)
{
	m_pField->Draw(pSystem, Shader::DEPTH);
	m_pPlayer->Draw(pSystem, Shader::DEPTH, true, false);
}

//--------------------------------------------------------------------------------
// 描画
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CDemoRender::Draw(CSystem * pSystem)
{
	m_pField->Draw(pSystem, Shader::SHADOW);
	m_pPlayer->Draw(pSystem, Shader::SHADOW);
	m_pSky->Draw(pSystem, Shader::DEFAULT);
}

//--------------------------------------------------------------------------------
// カメラクラスのポインタ取得
// 戻り値：const CCamera*	カメラクラスのポインタ
//--------------------------------------------------------------------------------
CCamera * CDemoRender::GetCamera()
{
	return m_pCamera;
}

//--------------------------------------------------------------------------------
// タイトルのステージを変更します
//--------------------------------------------------------------------------------
void CDemoRender::ChangeStage(CSystem* pSystem)
{
	if (m_meshAlpha > 0.01f) return;

	auto randomStage = Calculate::Random(0, 2);

	auto stage = MODEL_FIELD1;

	switch (randomStage)
	{
	case 0: stage = MODEL_FIELD1; break;
	case 1: stage = MODEL_FIELD2; break;
	case 2: stage = MODEL_FIELD3; break;
	}

	m_pField->SetModel(pSystem->GetXFile(stage));
}

void CDemoRender::UpdateMeshAlpha()
{
	if (m_isChangeMesh)
	{
		m_isChangeAlpha = true;
	}

	if (m_isChangeAlpha)
	{
		if (m_meshAlpha >= 0.01f)
		{
			m_meshAlpha -= 0.005f;
		}
		else {
			m_isChangeAlpha = false;
		}
	}
	else {
		if (m_meshAlpha < 1.0f)
		{
			m_meshAlpha += 0.005f;
		}
	}

	m_pSky->SetMeshAlpha(m_meshAlpha);
	m_pField->SetMeshAlpha(m_meshAlpha);
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// 世界行列の設定
//--------------------------------------------------------------------------------
void CDemoRender::SetWorldTransform()
{
	Matrix matWorld;

	matWorld = MatrixCalculate::Transform(
		Vector3(1.0f, 1.0f, 1.0f), 
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f, 0.0f, 0.0f));

	m_pField->SetTransform(&matWorld);
	m_pSky->SetTransform(&matWorld);
	m_pPlayer->SetTransform(&matWorld);
}

//--------------------------------------------------------------------------------
// カメラの距離設定
//--------------------------------------------------------------------------------
void CDemoRender::SetCameraDist()
{
	m_baseAngle += Mathf::ToRadian(0.1f);
	m_sinAngle = sinf(m_baseAngle) * 5.0f;

	m_demoAngle += Mathf::ToRadian(0.25f);
	m_vecAngle.y = m_demoAngle;
	m_vecAngle.x = sinf(m_sinAngle) * 0.2f;
	m_cameraXDist = sinf(m_sinAngle) * 2.0f;
	m_cameraZDist = (cosf(m_sinAngle) + 2.0f) * 1.5f;

	m_pCamera->SetThirdPerson(m_vecLockAt, m_vecAngle, m_cameraXDist, m_cameraZDist);
}

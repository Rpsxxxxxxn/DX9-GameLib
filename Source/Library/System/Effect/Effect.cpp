//*************************************************************
//
// Effect.cpp
//
// エフェクトクラス
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "Effect.h"
#include "../../Graphics/Camera/Camera.h"

//=============================================================
// シングルトン生成
//=============================================================
CEffectSystem * CEffectSystem::Create()
{
	static CEffectSystem instance;
	return &instance;
}

//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
CEffectSystem::CEffectSystem()
{
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CEffectSystem::~CEffectSystem()
{
	// エフェクトの停止
	m_pManager->StopAllEffects();

	// エフェクト管理用インスタンスを破棄
	m_pManager->Destroy();

	// 描画用インスタンスを破
	m_pRenderer->Destroy();

	// エフェクトの開放
	for (UINT i = 0; i < m_pEffect.size(); i++)
	{
		if (m_pEffect[i] != nullptr)
		{
			m_pEffect[i]->Release();
			m_pEffect[i] = nullptr;
		}
	}

	m_pManager = nullptr;
	m_pRenderer = nullptr;
}

//-------------------------------------------------------------
// 初期化
// 引　数：LPDIRECT3DDEVICE9*　	デバイスのポインタ
// 　　　：UINT					表示最大スプライト数
//-------------------------------------------------------------
void CEffectSystem::Initialize(LPDIRECT3DDEVICE9 pDevice, UINT drawCntMax)
{
	// 描画管理インスタンスの生成
	m_pRenderer = EffekseerRendererDX9::Renderer::Create(pDevice, drawCntMax);

	// エフェクト管理用インスタンスの生成
	m_pManager = Effekseer::Manager::Create(drawCntMax);

	// 描画方法の指定
	m_pManager->SetSpriteRenderer(m_pRenderer->CreateSpriteRenderer());
	m_pManager->SetRibbonRenderer(m_pRenderer->CreateRibbonRenderer());
	m_pManager->SetRingRenderer(m_pRenderer->CreateRingRenderer());
	m_pManager->SetTrackRenderer(m_pRenderer->CreateTrackRenderer());
	m_pManager->SetModelRenderer(m_pRenderer->CreateModelRenderer());

	// テクスチャ画像読み込み方法の指定
	m_pManager->SetTextureLoader(m_pRenderer->CreateTextureLoader());
	m_pManager->SetModelLoader(m_pRenderer->CreateModelLoader());

	// 座標系の指定(RHで右手系、LHで左手系)
	m_pManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
}

//-------------------------------------------------------------
// データの設定
// 引　数：const float 視野角（ラジアン角）
// 　　　　const float アスペクト比（width / height）
// 　　　　const float 前方クリップ面
// 　　　　const float 後方クリップ面
//-------------------------------------------------------------
void CEffectSystem::InitProjection(
	const float fov, 
	const float aspect,
	const float nearClip, 
	const float farClip
)
{
	// 投影行列
	Effekseer::Matrix44 matProj;

	// 投影行列の設定
	matProj.PerspectiveFovLH(fov, aspect, nearClip, farClip);

	// 投影行列の設定
	m_pRenderer->SetProjectionMatrix(matProj);

	// カメラを設定する
	m_pRenderer->SetCameraMatrix(
		Effekseer::Matrix44().LookAtLH(
			Effekseer::Vector3D(0.0f, 0.0f, 0.0f), // 視点
			Effekseer::Vector3D(0.0f, 0.0f, 0.0f), // 注目点
			Effekseer::Vector3D(0.0f, 1.0f, 0.0f)  // 上方向
		)
	);
}

//-------------------------------------------------------------
// エフェクトの追加
// 引　数：EFK_CHAR*	ファイル名
//-------------------------------------------------------------
void CEffectSystem::AddEffect(const EFK_CHAR * strFileName)
{
	m_pEffect.push_back(Effekseer::Effect::Create(m_pManager, strFileName));
}

//-------------------------------------------------------------
// エフェクトの追加
// 引　数：const int	配列の番目
// 　　　　EFK_CHAR*	ファイル名
//-------------------------------------------------------------
void CEffectSystem::AddEffect(const int index, const EFK_CHAR* strFileName)
{
	m_pEffect[index] = Effekseer::Effect::Create(m_pManager, strFileName);
}

//-------------------------------------------------------------
// エフェクトの再生
// 引　数：D3DXVECTOR3 3次元ベクトル
//-------------------------------------------------------------
void CEffectSystem::Play(const int index, D3DXVECTOR3& VecPos)
{
	m_hHandle = m_pManager->Play(
		m_pEffect[index],
		VecPos.x,
		VecPos.y,
		VecPos.z);
}

//-------------------------------------------------------------
// エフェクトの再生
// 引　数：D3DXVECTOR3 3次元ベクトルのポインタ
//-------------------------------------------------------------
void CEffectSystem::Play(const int index, D3DXVECTOR3* pVecPos)
{
	m_hHandle = m_pManager->Play(
		m_pEffect[index],
		pVecPos->x,
		pVecPos->y,
		pVecPos->z);
}

//-------------------------------------------------------------
// カメラ座標の更新
// 引　数：Camera*	カメラクラスのポインタ
//-------------------------------------------------------------
void CEffectSystem::Update(CCamera * pCamera)
{
	// 左辺代入
	Effekseer::Vector3D vecEye = ConvertD3DXVec3(pCamera->m_vecEye);
	Effekseer::Vector3D vecLookAt = ConvertD3DXVec3(pCamera->m_vecLookAt);
	Effekseer::Vector3D vecUp = ConvertD3DXVec3(pCamera->m_vecUp);

	// カメラ行列の更新
	m_pRenderer->SetCameraMatrix(Effekseer::Matrix44().LookAtLH(vecEye, vecLookAt, vecUp));

	// 再生中のエフェクトの移動等(::Effekseer::Manager経由で様々なパラメーターが設定できます。)
	m_pManager->AddLocation(m_hHandle, ConvertD3DXVec3(D3DXVECTOR3(0.0f, 0.0f, 0.0f)));

	// 全てのエフェクトの更新
	m_pManager->Update();
}

//-------------------------------------------------------------
// エフェクト描画
//-------------------------------------------------------------
void CEffectSystem::Draw()
{
	// レンダリング開始
	m_pRenderer->BeginRendering();

	// レンダリング中
	m_pManager->Draw();

	// レンダリング終了
	m_pRenderer->EndRendering();
}

//-------------------------------------------------------------
// ベクトルクラス変換(EffekVec3 -> D3DXVec3)
// 引　数：D3DXVECTOR3	３次元ベクトルクラス
//-------------------------------------------------------------
Effekseer::Vector3D CEffectSystem::ConvertD3DXVec3(D3DXVECTOR3 Vec3)
{
	Effekseer::Vector3D w;

	w.X = Vec3.x;
	w.Y = Vec3.y;
	w.Z = Vec3.z;

	return Effekseer::Vector3D(w);
}

//-------------------------------------------------------------
// ベクトルクラス変換(D3DXVec3 -> EffekVec3)
// 引　数：Effekseer::Vector3D	3次元ベクトルクラス
//-------------------------------------------------------------
D3DXVECTOR3 CEffectSystem::ConvertEffekVec3(Effekseer::Vector3D Vec3)
{
	D3DXVECTOR3 w;

	w.x = Vec3.X;
	w.y = Vec3.Y;
	w.z = Vec3.Z;

	return D3DXVECTOR3(w);
}

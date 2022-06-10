//*************************************************************
//
// SceneManager.cpp
//
// シーンオブジェクト管理
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "SceneManager.h"
#include "IScene.h"
#include "SceneTitle/SceneTitle.h"
#include "SceneGame/SceneGame.h"
#include "SceneResult/SceneResult.h"

//-------------------------------------------------------------
// 初期化
//-------------------------------------------------------------
void CSceneManager::Initialize(CSystem * pSystem)
{
	m_pSystem = pSystem;
}

//-------------------------------------------------------------
// 更新
//-------------------------------------------------------------
void CSceneManager::Update()
{
	m_pNowScene->Update();
}

//-------------------------------------------------------------
// 描画
//-------------------------------------------------------------
void CSceneManager::Draw()
{
	//デプスクラス変換
	auto shaderDepth = CShaderManager::GetInstance().GetShader(Shader::DEPTH);
	auto depth = dynamic_cast<CShadowDepth*>(shaderDepth);

	//デプスレンダリング
	for (int i = 0; i < 3; i++)
	{
		depth->StartDepthRender(i);
		{
			m_pNowScene->ZDraw();
		}
		depth->EndDepthRender();

		m_pShadowMap[i] = depth->GetTexture(i);
	}

	// シャドウクラス変換
	auto shaderShadow = CShaderManager::GetInstance().GetShader(Shader::SHADOW);
	auto shadow = dynamic_cast<CShadowMap*>(shaderShadow);

	//シャドウテクスチャの設定
	shadow->SetShadowTexture0(m_pShadowMap[0]);
	shadow->SetShadowTexture1(m_pShadowMap[1]);
	shadow->SetShadowTexture2(m_pShadowMap[2]);
	
	{
		m_pNowScene->Draw();
	}
}

//-------------------------------------------------------------
// シーンの設定
// 引　数：const Scene	次のシーンタイプ
//-------------------------------------------------------------
void CSceneManager::SetScene(const Scene type)
{
	m_newScene = type;
}

//-------------------------------------------------------------
// シーン変更
//-------------------------------------------------------------
bool CSceneManager::ChangeScene()
{
	if (m_newScene != m_nowScene)
	{
		//既にあるならば削除
		SafeDelete(m_pNowScene);
		
		switch (m_newScene)
		{
		case SCENE_TITLE:
			m_pNowScene = new CSceneTitle(m_pSystem);
			break;
		case SCENE_GAME:
			m_pNowScene = new CSceneGame(m_pSystem);
			break;
		case SCENE_RESULT:
			m_pNowScene = new CSceneResult(m_pSystem);
			break;
		case SCENE_EXIT:
			return false;
		}

		if (m_pNowScene == nullptr) return true;

		//ファイル読み込み
		m_pNowScene->LoadFile();

		//初期化
		m_pNowScene->Initialize();

		//現在のシーンにする
		m_nowScene = m_newScene;
	}

	return true;
}

//-------------------------------------------------------------
// 分割シャドウの値設定
// 引　数：CCamera*			カメラクラスのポインタ
//-------------------------------------------------------------
void CSceneManager::SetConstantTable(
	CCamera* pCamera
)
{
	m_pLight->SetOrtho();

	//左辺導入
	auto viewMat = pCamera->GetViewMatrix();
	auto projMat = pCamera->GetProjectionMatrix();
	auto lightViewMat = m_pLight->GetViewMatrix();
	auto lightProjMat = m_pLight->GetProjectionMatrix();
	auto lightBiasMat = m_pLight->GetLightBias();
	auto lightEyeVec = m_pLight->GetPosition();

	//カスケーディングシャドウマップのパラメータ設定
	auto shader = CShaderManager::GetInstance().GetShader(DEPTH);
	auto zDepth = static_cast<CShadowDepth*>(shader);

	zDepth->SetLightViewMatrix(&lightViewMat);
	zDepth->SetLightProjMatrix(&lightProjMat);
	zDepth->SetLightPosition(&lightEyeVec);
	zDepth->RecalculateShadowMapMatrices(pCamera);

	m_pMatVP[0] = zDepth->GetViewProjMatrix(0);
	m_pMatVP[1] = zDepth->GetViewProjMatrix(1);
	m_pMatVP[2] = zDepth->GetViewProjMatrix(2);

	//for (int i = 0; i < 3; i++)
	//	D3DXMatrixTranspose(&m_pMatVP[i], &m_pMatVP[i]);

	//D3DXMatrixTranspose(&viewMat, &viewMat);
	//D3DXMatrixTranspose(&projMat, &projMat);

	CShaderManager::GetInstance().SetMatView(viewMat);
	CShaderManager::GetInstance().SetMatProj(projMat);
	CShaderManager::GetInstance().SetMatLViewProj(m_pMatVP);
	CShaderManager::GetInstance().SetMatLBias(lightBiasMat);
	CShaderManager::GetInstance().SetLightEye(&lightEyeVec);
	CShaderManager::GetInstance().SetCameraEye(&pCamera->m_vecEye);
	CShaderManager::GetInstance().Update();
}

//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
CSceneManager::CSceneManager()
	: m_newScene(Scene::SCENE_TITLE)
	, m_nowScene(Scene::SCENE_GAME)
	, m_pLight(new CDirectionLight)
	, m_pMatVP(new Matrix[3])
	, m_pNowScene(nullptr)
{

}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CSceneManager::~CSceneManager()
{
	SafeDelete(m_pLight);
	SafeDelete(m_pNowScene);
}

//-------------------------------------------------------------
// インスタンス生成
//-------------------------------------------------------------
ShareObject CSceneManager::shareData;
CSceneManager* CSceneManager::m_pInstance = nullptr;

//-------------------------------------------------------------
// インスタンスの取得
//-------------------------------------------------------------
CSceneManager* CSceneManager::GetInstance()
{
	return m_pInstance;
}

//-------------------------------------------------------------
// 生成
//-------------------------------------------------------------
void CSceneManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new CSceneManager;
	}
}

//-------------------------------------------------------------
// 破棄
//-------------------------------------------------------------
void CSceneManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

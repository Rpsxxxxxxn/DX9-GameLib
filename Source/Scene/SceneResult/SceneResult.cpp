//*************************************************************
//
// SceneResult.cpp
//
// シーンリザルト
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "SceneResult.h"
#include "../SceneManager.h"

//=============================================================
// クラス定義
//=============================================================

//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
CSceneResult::CSceneResult(CSystem* pSystem)
	: IScene(pSystem)
	, m_pTimer(new CShowTimer)
{
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CSceneResult::~CSceneResult()
{
	this->Release();
}

//-------------------------------------------------------------
// ファイル読み込み
//-------------------------------------------------------------
void CSceneResult::LoadFile()
{
	CSceneManager::shareData.render.LoadFile( m_pSystem );
	CSceneManager::shareData.fadeAni.LoadFile( m_pSystem );

	m_pTimer->LoadFile(m_pSystem);
}

//-------------------------------------------------------------
// 初期化
//-------------------------------------------------------------
void CSceneResult::Initialize()
{
	CSceneManager::shareData.render.Initialize( m_pSystem );
	CSceneManager::shareData.fadeAni.SetFadeMode(FadeMode::F_OUT);

	m_pTimer->Initialize(m_pSystem, CSceneManager::shareData.clearTime);
}

//-------------------------------------------------------------
// 更新
//-------------------------------------------------------------
void CSceneResult::Update()
{
	CSceneManager::shareData.render.Update( m_pSystem );

	m_pTimer->Update(m_pSystem, &CSceneManager::shareData.fadeAni);

	if ( m_pTimer->IsTitle() )
	{
		SCENE_CHANGE( Scene::SCENE_TITLE );
	}

	CONSTANT_TABLE(CSceneManager::shareData.render.GetCamera());
}

//-------------------------------------------------------------
// 描画
//-------------------------------------------------------------
void CSceneResult::ZDraw()
{
	CSceneManager::shareData.render.ZDraw( m_pSystem );
}

//-------------------------------------------------------------
// 描画
//-------------------------------------------------------------
void CSceneResult::Draw()
{
	CSceneManager::shareData.render.Draw( m_pSystem );

	m_pTimer->Draw(m_pSystem);

	CSceneManager::shareData.fadeAni.Draw( m_pSystem );
}

//-------------------------------------------------------------
// 解放
//-------------------------------------------------------------
void CSceneResult::Release()
{
	m_pSystem->ReleaseAllSound();
	m_pSystem->ReleaseAllFile();

	SafeDelete( m_pTimer );
}
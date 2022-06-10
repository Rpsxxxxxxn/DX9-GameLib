//*************************************************************
//
// SceneObject.cpp
//
// シーンオブジェクトクラス(基底)
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "SceneTitle.h"
#include "../SceneManager.h"

//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
CSceneTitle::CSceneTitle(CSystem* pSystem)
	: IScene(pSystem)
	, m_pTitle(new CTitleManager)
{
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CSceneTitle::~CSceneTitle()
{
	this->Release();
}

//-------------------------------------------------------------
// ファイル読み込み
//-------------------------------------------------------------
void CSceneTitle::LoadFile()
{
	CSceneManager::shareData.fadeAni.LoadFile( m_pSystem );
	CSceneManager::shareData.render.LoadFile( m_pSystem );

	m_pTitle->LoadFile( m_pSystem );
}

//-------------------------------------------------------------
// 初期化
//-------------------------------------------------------------
void CSceneTitle::Initialize()
{
	CSceneManager::shareData.fadeAni.Initialize( m_pSystem );
	CSceneManager::shareData.fadeAni.SetFadeMode( FadeMode::F_OUT );
	CSceneManager::shareData.render.Initialize( m_pSystem );

	m_pTitle->Initialize( m_pSystem );
}

//-------------------------------------------------------------
// 更新
//-------------------------------------------------------------
void CSceneTitle::Update()
{
	CSceneManager::shareData.render.Update( m_pSystem );

	m_pTitle->Update( m_pSystem );

	if ( m_pTitle->IsGame( &CSceneManager::shareData.fadeAni ) )
	{
		SCENE_CHANGE(SCENE_GAME);
	}

	if ( m_pTitle->IsExit( &CSceneManager::shareData.fadeAni ) )
	{
		SCENE_CHANGE(SCENE_EXIT);
	}
	
	CONSTANT_TABLE(CSceneManager::shareData.render.GetCamera());
}

//-------------------------------------------------------------
// 描画
//-------------------------------------------------------------
void CSceneTitle::ZDraw()
{
	CSceneManager::shareData.render.ZDraw( m_pSystem );
}

//-------------------------------------------------------------
// 描画
//-------------------------------------------------------------
void CSceneTitle::Draw()
{
	CSceneManager::shareData.render.Draw( m_pSystem );

	m_pTitle->Draw( m_pSystem );

	CSceneManager::shareData.fadeAni.Draw( m_pSystem );
}

//-------------------------------------------------------------
// 解放
//-------------------------------------------------------------
void CSceneTitle::Release()
{
	m_pSystem->ReleaseAllFile();
	m_pSystem->ReleaseAllSound();

	SafeDelete(m_pTitle);
}
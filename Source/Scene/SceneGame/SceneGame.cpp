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
#include "SceneGame.h"
#include "../SceneManager.h"

//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
CSceneGame::CSceneGame(CSystem* pSystem)
	: IScene(pSystem)
	, m_pPlayer(new CPlayer)
	, m_pPlayerCamera(new CPlayerCamera)
	, m_pLockOn(new CLockOnSystem)
	, m_pAIManager(new CAIManager)
	, m_pField(new CFieldManager)
	, m_pProgress(new CGameProgress)
	, m_pSky(new CSkydome)
	, m_pGameCollide(new CGameCollide)
	, m_pShowDamage(new CShowDamage)
	, m_pStatus(new CStatus)
	, m_pGameInfo(new CGameInfo)
	, m_pGuide(new CGuide)
	, m_pStageInfo(new CStageInfo)
	, m_pMinimap(new CMinimap)
	, m_pPause(new CPause)
	, m_pTimer(new CGameTimer)
{
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CSceneGame::~CSceneGame()
{
	this->Release();
}

//-------------------------------------------------------------
// ファイル読み込み
//-------------------------------------------------------------
void CSceneGame::LoadFile()
{
	m_pSky->LoadFile(m_pSystem);
	m_pPause->LoadFile(m_pSystem);
	m_pField->LoadFile(m_pSystem);
	m_pGuide->LoadFile(m_pSystem);
	m_pPlayer->LoadFile(m_pSystem);
	m_pLockOn->LoadFile(m_pSystem);
	m_pStatus->LoadFile(m_pSystem);
	m_pMinimap->LoadFile(m_pSystem);
	m_pGameInfo->LoadFile(m_pSystem);
	m_pProgress->LoadFile(m_pSystem);
	m_pStageInfo->LoadFile(m_pSystem);
	m_pAIManager->LoadFile(m_pSystem);
	m_pShowDamage->LoadFile(m_pSystem);
	m_pPlayerCamera->LoadFile(m_pSystem);

	CSceneManager::shareData.fadeAni.LoadFile(m_pSystem);
}

//-------------------------------------------------------------
// 初期化
//-------------------------------------------------------------
void CSceneGame::Initialize()
{
	m_pSky->Initialize(m_pSystem);
	m_pPause->Initialize(m_pSystem);
	m_pField->Initialize(m_pSystem);
	m_pGuide->Initialize(m_pSystem);
	m_pPlayer->Initialize(m_pSystem);
	m_pLockOn->Initialize(m_pSystem);
	m_pStatus->Initialize(m_pSystem);
	m_pMinimap->Initialize(m_pSystem);
	m_pGameInfo->Initialize(m_pSystem);
	m_pProgress->Initialize(m_pSystem);
	m_pStageInfo->Initialize(m_pSystem);
	m_pAIManager->Initialize(m_pSystem);
	m_pPlayerCamera->Initialize(m_pSystem);

	CSceneManager::shareData.fadeAni.SetFadeMode(FadeMode::F_OUT);
}

//-------------------------------------------------------------
// 更新
//-------------------------------------------------------------
void CSceneGame::Update()
{
	m_pTimer->Update(m_pSystem);
	m_pProgress->Update(m_pSystem, m_pField, m_pPlayer, m_pAIManager, &CSceneManager::shareData.fadeAni);
	m_pPause->Update(m_pSystem, m_pPlayer, m_pAIManager, m_pTimer, &CSceneManager::shareData.fadeAni);
	m_pLockOn->Update(m_pSystem, m_pPlayer, m_pAIManager);
	m_pPlayerCamera->Update(m_pSystem, m_pPlayer, m_pLockOn->GetAngle());
	m_pPlayer->Update(m_pSystem, m_pPlayerCamera->GetAngle());
	m_pStatus->Update(m_pSystem, m_pPlayer, m_pAIManager);
	m_pGameInfo->Update(m_pSystem, m_pProgress);
	m_pStageInfo->Update(m_pSystem, m_pProgress);
	m_pGuide->Update(m_pSystem, m_pPlayer, m_pAIManager);
	m_pField->Update(m_pSystem);
	m_pSky->Update(m_pSystem, m_pPlayerCamera); 
	m_pAIManager->Update(m_pSystem, m_pPlayer);
	m_pShowDamage->Update(m_pSystem, m_pPlayerCamera);
	m_pGameCollide->Update(m_pSystem, m_pField, m_pPlayerCamera, m_pPlayer, m_pAIManager, m_pShowDamage);

	if ( m_pProgress->IsGameClear() )
	{
		CSceneManager::shareData.clearTime = m_pTimer->GetTimer();
		SCENE_CHANGE(Scene::SCENE_RESULT);
	}

	if ( m_pPause->IsTitle() || m_pProgress->IsGameOver() )
	{
		SCENE_CHANGE(Scene::SCENE_TITLE);
	}

	CONSTANT_TABLE(m_pPlayerCamera->GetCamera());
}

//-------------------------------------------------------------
// 描画
//-------------------------------------------------------------
void CSceneGame::ZDraw()
{
	m_pPlayer->ZDraw(m_pSystem);
	m_pAIManager->ZDraw(m_pSystem);
	m_pField->ZDraw(m_pSystem);
}

//-------------------------------------------------------------
// 描画
//-------------------------------------------------------------
void CSceneGame::Draw()
{
	m_pPlayer->Draw(m_pSystem);
	m_pAIManager->Draw(m_pSystem);
	m_pField->Draw(m_pSystem);
	m_pSky->Draw(m_pSystem);
	m_pSystem->DrawEffect();
	m_pShowDamage->Draw(m_pSystem);
	m_pLockOn->Draw(m_pSystem, m_pPlayerCamera->GetCamera());
	m_pGuide->Draw(m_pSystem);
	m_pGameInfo->Draw(m_pSystem);
	m_pStageInfo->Draw(m_pSystem);
	m_pMinimap->Draw(m_pSystem, m_pPlayerCamera, m_pPlayer, m_pAIManager);
	m_pStatus->Draw(m_pSystem);

	CSceneManager::shareData.fadeAni.Draw(m_pSystem);
	
	m_pPause->Draw(m_pSystem);
}

//-------------------------------------------------------------
// 解放
//-------------------------------------------------------------
void CSceneGame::Release()
{
	m_pSystem->ReleaseAllSound();
	m_pSystem->ReleaseAllFile();

	SafeDelete(m_pPlayer);
	SafeDelete(m_pPlayerCamera);
	SafeDelete(m_pField);
	SafeDelete(m_pGuide);
	SafeDelete(m_pSky);
	SafeDelete(m_pAIManager);
	SafeDelete(m_pShowDamage);
	SafeDelete(m_pStatus);
	SafeDelete(m_pMinimap);
	SafeDelete(m_pProgress);
	SafeDelete(m_pGameInfo);
	SafeDelete(m_pStageInfo);
	SafeDelete(m_pLockOn);
	SafeDelete(m_pGameCollide);
	SafeDelete(m_pPause);
	SafeDelete(m_pTimer);
}
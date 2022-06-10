//*************************************************************
//
// SceneObject.h
//
// シーンオブジェクトクラス(基底)
//
// Date: 2019/07/30 更新
//
//*************************************************************

#pragma once

//=============================================================
// インクルード
//=============================================================
#include "../IScene.h"

#include "Camera/PlayerCamera.hpp"
#include "Camera/LockOn.hpp"
#include "Player/Player.hpp"
#include "Progress/GameProgress.hpp"
#include "Collide/GameCollide.hpp"
#include "Field/FieldManager.h"
#include "Skydome/Skydome.hpp"
#include "Hud/Damage/Damage.hpp"
#include "Hud/Status/Status.hpp"
#include "Hud/Minimap/Minimap.hpp"
#include "Hud/Pause/Pause.hpp"
#include "Hud/GameInfo/GameInfo.hpp"
#include "Hud/StageInfo/StageInfo.hpp"
#include "Hud/Guide/Guide.hpp"
#include "AI/AIManager.hpp"
#include "Timer/GameTimer.hpp"

//=============================================================
// クラス定義
//=============================================================
class CSceneGame : public IScene
{
public:

	//-------------------------------------------------------------
	// コンストラクタ
	//-------------------------------------------------------------
	CSceneGame(CSystem* pSystem);

	//-------------------------------------------------------------
	// デストラクタ
	//-------------------------------------------------------------
	~CSceneGame();

	//-------------------------------------------------------------
	// ファイル読み込み
	//-------------------------------------------------------------
	void LoadFile();

	//-------------------------------------------------------------
	// 初期化
	//-------------------------------------------------------------
	void Initialize();

	//-------------------------------------------------------------
	// 更新
	//-------------------------------------------------------------
	void Update();

	//-------------------------------------------------------------
	// 描画
	//-------------------------------------------------------------
	void ZDraw();

	//-------------------------------------------------------------
	// 描画
	//-------------------------------------------------------------
	void Draw();

	//-------------------------------------------------------------
	// 解放
	//-------------------------------------------------------------
	void Release();

private:

	CPlayerCamera* m_pPlayerCamera;
	CPlayer* m_pPlayer;
	CAIManager* m_pAIManager;
	CFieldManager* m_pField;
	CGameProgress* m_pProgress;
	CSkydome* m_pSky;
	CGameCollide* m_pGameCollide;
	CShowDamage* m_pShowDamage;
	CStatus* m_pStatus;
	CMinimap* m_pMinimap;
	CGameInfo* m_pGameInfo;
	CStageInfo* m_pStageInfo;
	CGuide* m_pGuide;
	CPause* m_pPause;
	CGameTimer* m_pTimer;
	CLockOnSystem* m_pLockOn;
};
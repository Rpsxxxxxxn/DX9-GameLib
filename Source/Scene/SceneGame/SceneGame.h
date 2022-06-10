//*************************************************************
//
// SceneObject.h
//
// �V�[���I�u�W�F�N�g�N���X(���)
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

#pragma once

//=============================================================
// �C���N���[�h
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
// �N���X��`
//=============================================================
class CSceneGame : public IScene
{
public:

	//-------------------------------------------------------------
	// �R���X�g���N�^
	//-------------------------------------------------------------
	CSceneGame(CSystem* pSystem);

	//-------------------------------------------------------------
	// �f�X�g���N�^
	//-------------------------------------------------------------
	~CSceneGame();

	//-------------------------------------------------------------
	// �t�@�C���ǂݍ���
	//-------------------------------------------------------------
	void LoadFile();

	//-------------------------------------------------------------
	// ������
	//-------------------------------------------------------------
	void Initialize();

	//-------------------------------------------------------------
	// �X�V
	//-------------------------------------------------------------
	void Update();

	//-------------------------------------------------------------
	// �`��
	//-------------------------------------------------------------
	void ZDraw();

	//-------------------------------------------------------------
	// �`��
	//-------------------------------------------------------------
	void Draw();

	//-------------------------------------------------------------
	// ���
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
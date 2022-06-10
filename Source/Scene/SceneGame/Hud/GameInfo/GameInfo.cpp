//********************************************************************************
//
// GameInfo.hpp
//
// ガイド表示用クラス
//
// Date: 2019/07/30 更新
//
//********************************************************************************

//================================================================================
// インクルード
//================================================================================
#include "GameInfo.hpp"

using namespace FileConst;

//================================================================================
// クラス定義
//================================================================================

//--------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------
CGameInfo::CGameInfo()
	: m_nowStage(1)
{
}

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
CGameInfo::~CGameInfo()
{
	for (auto w : m_pSprite)
	{
		SafeDelete(w);
	}
}

//--------------------------------------------------------------------------------
// ファイル読み込み
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CGameInfo::LoadFile(CSystem * pSystem)
{
	pSystem->LoadTexture(TEXTURE_MISSION_BACK,	"Resource/Textures/Mission/Back.png");
	pSystem->LoadTexture(TEXTURE_MISSION_ENEMY, "Resource/Textures/Mission/Enemy.png");
	pSystem->LoadTexture(TEXTURE_MISSION_BOSS,	"Resource/Textures/Mission/Boss.png");
	pSystem->LoadTexture(TEXTURE_STAGE_BACK	,	"Resource/Textures/Mission/StageInfoBack.png");
	pSystem->LoadTexture(TEXTURE_STAGE_FIRST,	"Resource/Textures/Mission/StageFirst.png");
	pSystem->LoadTexture(TEXTURE_STAGE_SECOND,	"Resource/Textures/Mission/StageSecond.png");
	pSystem->LoadTexture(TEXTURE_STAGE_FINAL,	"Resource/Textures/Mission/StageFinal.png");
}

//--------------------------------------------------------------------------------
// 初期化
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CGameInfo::Initialize(CSystem * pSystem)
{
	m_halfWidth = pSystem->GetWindowSettings().Width / 2.0f;
	m_halfHeight = pSystem->GetWindowSettings().Height / 2.0f;

	m_pSprite[ST_MISSIONBACK] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_MISSION_BACK));
	m_pSprite[ST_MISSIONBACK]->SetPosition(Vector2(5.0f, 100.0f));

	m_pSprite[ST_STAGE_BACK] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_STAGE_BACK));
	m_pSprite[ST_STAGE_BACK]->SetPosition(Vector2(m_halfWidth - 299.0f, m_halfHeight - 54.0f));
	m_pSprite[ST_STAGE_BACK]->SetEndTime(5000);
	m_pSprite[ST_STAGE_BACK]->SetNewAlpha(0);

	m_pSprite[ST_MISSIONINFO] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_MISSION_ENEMY));
	m_pSprite[ST_MISSIONINFO]->SetPosition(Vector2(40.0f, 140.0f));
	
	m_pSprite[ST_STAGE_INFO] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_STAGE_FIRST));
	m_pSprite[ST_STAGE_INFO]->SetPosition(Vector2(m_halfWidth - 192.5f, m_halfHeight - 46.5f));
	m_pSprite[ST_STAGE_INFO]->SetEndTime(5000);
	m_pSprite[ST_STAGE_INFO]->SetNewAlpha(0);
}

//--------------------------------------------------------------------------------
// 更新
// 引　数：CSystem*	システムクラスのポインタ
// 　　　　CGameProgress*	進行状況クラスのポインタ
//--------------------------------------------------------------------------------
void CGameInfo::Update(CSystem * pSystem, CGameProgress* pProgress)
{
	ChangeMissionInfo(pSystem, pProgress);

	ChangeStageInfo(pSystem, pProgress);
}

//--------------------------------------------------------------------------------
// 描画
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CGameInfo::Draw(CSystem * pSystem)
{
	for (auto i = 0; i < ST_MAX; i++)
	{
		m_pSprite[i]->Draw(pSystem);
	}
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// ミッション詳細の生成
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CGameInfo::CreateMission(CSystem* pSystem)
{
	SafeDelete(m_pSprite[ST_MISSIONINFO]);

	m_pSprite[ST_MISSIONINFO] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_MISSION_BOSS));
	m_pSprite[ST_MISSIONINFO]->SetPosition(Vector2(35.0f, 140.0f));
}

//--------------------------------------------------------------------------------
// 現在のステージ情報の表示
// 引　数：CSystem*	システムクラスのポインタ
// 　　　　CGameProgress*	進行状況クラスのポインタ
//--------------------------------------------------------------------------------
void CGameInfo::ChangeMissionInfo(CSystem * pSystem, CGameProgress * pProgress)
{
	auto newStage = pProgress->GetNowStage();

	if (newStage != m_nowStage)
	{
		if (!pProgress->IsStartFade()) return;

		if (newStage < STAGE_3) return;

		CreateMission(pSystem);
	}
}

//--------------------------------------------------------------------------------
// 現在のステージ情報の表示
// 引　数：CSystem*	システムクラスのポインタ
// 　　　　CGameProgress*	進行状況クラスのポインタ
//--------------------------------------------------------------------------------
void CGameInfo::ChangeStageInfo(CSystem* pSystem, CGameProgress * pProgress)
{
	auto newStage = pProgress->GetNowStage();

	if (newStage != m_nowStage)
	{
		if (!pProgress->IsStartFade()) return;

		switch (newStage)
		{
		case STAGE_1: CreateStageInfo(pSystem, TEXTURE_STAGE_FIRST); break;
		case STAGE_2: CreateStageInfo(pSystem, TEXTURE_STAGE_SECOND); break;
		case STAGE_3: CreateStageInfo(pSystem, TEXTURE_STAGE_FINAL); break;
		}

		m_nowStage = newStage;
	}
}

//--------------------------------------------------------------------------------
// ステージ情報の生成
// 引　数：CSystem*			システムクラスのポインタ
//--------------------------------------------------------------------------------
void CGameInfo::CreateStageInfo(CSystem* pSystem, const int fileConst)
{
	SafeDelete(m_pSprite[ST_STAGE_INFO]);

	m_pSprite[ST_STAGE_BACK]->SetAlpha(255);
	m_pSprite[ST_STAGE_BACK]->SetNewAlpha(0);

	m_pSprite[ST_STAGE_INFO] = new CSpriteAnimation(pSystem->GetTextureData(fileConst));
	m_pSprite[ST_STAGE_INFO]->SetPosition(Vector2(m_halfWidth - 192.5f, m_halfHeight - 46.5f));
	m_pSprite[ST_STAGE_INFO]->SetEndTime(5000);
	m_pSprite[ST_STAGE_INFO]->SetNewAlpha(0);
}

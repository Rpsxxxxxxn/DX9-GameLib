//********************************************************************************
//
// StageInfo.hpp
//
// ガイド表示用クラス
//
// Date: 2019/07/30 更新
//
//********************************************************************************

//================================================================================
// インクルード
//================================================================================
#include "StageInfo.hpp"

using namespace FileConst;

const Vector2 SHOW_TEXTURE_POS(339.0f, 66.5f);

//================================================================================
// クラス定義
//================================================================================

//--------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------
CStageInfo::CStageInfo()
{
}

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
CStageInfo::~CStageInfo()
{
	SafeDelete(m_pSprite);
}

//--------------------------------------------------------------------------------
// ファイル読み込み
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CStageInfo::LoadFile(CSystem * pSystem)
{
	pSystem->LoadTexture(TEXTURE_GAME_CLEAR,	"Resource/Textures/GameClear/GameClear.png");
	pSystem->LoadTexture(TEXTURE_GAME_OVER,		"Resource/Textures/GameClear/GameOver.png");
	pSystem->LoadTexture(TEXTURE_STAGE_CLEAR,	"Resource/Textures/GameOver/StageClear.png"); 
}

//--------------------------------------------------------------------------------
// 初期化
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CStageInfo::Initialize(CSystem * pSystem)
{
	m_halfWidth = pSystem->GetWindowSettings().Width / 2.0f;
	m_halfHeight = pSystem->GetWindowSettings().Height / 2.0f;

	m_pSprite = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_STAGE_CLEAR));
	m_pSprite->SetPosition(Vector2(m_halfWidth - SHOW_TEXTURE_POS.x, m_halfHeight - SHOW_TEXTURE_POS.y));
	m_pSprite->SetAlpha(0);
}

//--------------------------------------------------------------------------------
// 更新
// 引　数：CSystem*	システムクラスのポインタ
// 　　　　CGameProgress*	進行状況クラスのポインタ
//--------------------------------------------------------------------------------
void CStageInfo::Update(CSystem * pSystem, CGameProgress* pProgress)
{
	StageManager(pProgress);

	ShowGameClear(pSystem, pProgress);

	ShowGameOver(pSystem, pProgress);
}

//--------------------------------------------------------------------------------
// 描画
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CStageInfo::Draw(CSystem * pSystem)
{
	m_pSprite->Draw(pSystem);
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// ステージ管理
// 引　数：CGameProgress*	進行状況クラスのポインタ
//--------------------------------------------------------------------------------
void CStageInfo::StageManager(CGameProgress * pProgress)
{
	int newStage = pProgress->GetNowStage();

	if (m_nowStage != newStage)
	{
		if (!pProgress->IsStartFade()) return;

		m_pSprite->SetAlpha(0);

		m_nowStage = newStage;
	}
}

//--------------------------------------------------------------------------------
// ゲームクリアの表示
// 引　数：CSystem*		システムクラスのポインタ
// 　　　　CGameProgress*	進行状況クラスのポインタ
//--------------------------------------------------------------------------------
void CStageInfo::ShowGameClear(CSystem* pSystem, CGameProgress * pProgress)
{
	if (!pProgress->IsStageClear()) return;

	if (m_nowStage < STAGE_3)
	{
		ChangeTextureData(pSystem, TEXTURE_STAGE_CLEAR);
	}
	else 
	{
		ChangeTextureData(pSystem, TEXTURE_GAME_CLEAR);
	}
}

//--------------------------------------------------------------------------------
// ゲームオーバーの表示
// 引　数：CSystem*			システムクラスのポインタ
// 　　　　CGameProgress*	進行状況クラスのポインタ
//--------------------------------------------------------------------------------
void CStageInfo::ShowGameOver(CSystem * pSystem, CGameProgress * pProgress)
{
	if (!pProgress->IsPlayerDeath()) return;

	ChangeTextureData(pSystem, TEXTURE_GAME_OVER);
}

//--------------------------------------------------------------------------------
// テクスチャデータの更新
// 引　数：CSystem*		システムクラスのポインタ
// 　　　　const int	ファイル番号
//--------------------------------------------------------------------------------
void CStageInfo::ChangeTextureData(CSystem * pSystem, const int fileConst)
{
	m_pSprite->SetTextureData(pSystem->GetTextureData(fileConst));
	m_pSprite->SetNewAlpha(255);
}
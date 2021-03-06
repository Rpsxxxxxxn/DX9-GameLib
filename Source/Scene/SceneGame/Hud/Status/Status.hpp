//********************************************************************************
//
// CStatus.hpp
//
// ステータス情報の描画
//
// Date: 2019/07/30 更新
//
//********************************************************************************

#pragma once

//================================================================================
// インクルード
//================================================================================
#include "../../../GameManager/GameManager.h"
#include "../../Player/Player.hpp"
#include "../../AI/AIManager.hpp"

//================================================================================
// クラス定義
//================================================================================
class CStatus
{
public:

	//--------------------------------------------------------------------------------
	// コンストラクタ
	//--------------------------------------------------------------------------------
	CStatus();

	//--------------------------------------------------------------------------------
	// デストラクタ
	//--------------------------------------------------------------------------------
	~CStatus();

	//--------------------------------------------------------------------------------
	// ファイル読み込み
	// 引　数：CSystem*	システムクラスのポインタ
	//--------------------------------------------------------------------------------
	void LoadFile(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// 初期化
	// 引　数：CSystem*	システムクラスのポインタ
	//--------------------------------------------------------------------------------
	void Initialize(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// 更新
	// 引　数：CSystem*	システムクラスのポインタ
	//--------------------------------------------------------------------------------
	void Update(CSystem* pSystem, CPlayer* pPlayer, CAIManager* pAI);

	//--------------------------------------------------------------------------------
	// 描画
	// 引　数：CSystem*	システムクラスのポインタ
	//--------------------------------------------------------------------------------
	void Draw(CSystem* pSystem);

private:

	//--------------------------------------------------------------------------------
	// プレイヤー情報の更新
	//--------------------------------------------------------------------------------
	void UpdatePlayerStatus(CPlayer* pPlayer);

	//--------------------------------------------------------------------------------
	// プレイヤーHPとスタミナの状況更新
	//--------------------------------------------------------------------------------
	void UpdateProgress();

	//--------------------------------------------------------------------------------
	// ボスHPの更新と計算
	//--------------------------------------------------------------------------------
	void UpdateBossHp(CAIManager* pAI);

	//--------------------------------------------------------------------------------
	// ダメージ受けたときに赤くなるやつ
	//--------------------------------------------------------------------------------
	void ShowDamage(CPlayer* pPlayer);

	enum SpriteTexture
	{
		STATUS_BACK,
		HP_BAR,
		STAMINA_BAR,
		HP_FRONT,
		ST_FRONT,
		SHOW_DAMAGE,
		BOSS_HP_BAR,
		BOSS_HP_FRONT,
		MAX,
	};

	CSpriteAnimation* m_pSprite[MAX];

	float m_playerHp;
	float m_playerStamina;

	float m_bossHp;
};
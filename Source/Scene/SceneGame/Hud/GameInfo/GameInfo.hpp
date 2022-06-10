//********************************************************************************
//
// GameInfo.hpp
//
// ガイド表示用クラス
//
// Date: 2019/07/30 更新
//
//********************************************************************************

#pragma once

//================================================================================
// インクルード
//================================================================================
#include "../../../GameManager/GameManager.h"
#include "../../Progress/GameProgress.hpp"

//================================================================================
// クラス定義
//================================================================================
class CGameInfo
{
public:

	//--------------------------------------------------------------------------------
	// コンストラクタ
	//--------------------------------------------------------------------------------
	CGameInfo();

	//--------------------------------------------------------------------------------
	// デストラクタ
	//--------------------------------------------------------------------------------
	~CGameInfo();

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
	// 　　　　CGameProgress*	進行状況クラスのポインタ
	//--------------------------------------------------------------------------------
	void Update(CSystem* pSystem, CGameProgress* pProgress);

	//--------------------------------------------------------------------------------
	// 描画
	// 引　数：CSystem*	システムクラスのポインタ
	//--------------------------------------------------------------------------------
	void Draw(CSystem* pSystem);

private:

	//--------------------------------------------------------------------------------
	// 現在のステージ情報の表示
	// 引　数：CSystem*			システムクラスのポインタ
	// 　　　　CGameProgress*	進行状況クラスのポインタ
	//--------------------------------------------------------------------------------
	void ChangeMissionInfo(CSystem* pSystem, CGameProgress* pProgress);

	//--------------------------------------------------------------------------------
	// ミッション詳細の生成
	// 引　数：CSystem*			システムクラスのポインタ
	//--------------------------------------------------------------------------------
	void CreateMission(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// 現在のステージ情報の表示
	// 引　数：CSystem*			システムクラスのポインタ
	// 　　　　CGameProgress*	進行状況クラスのポインタ
	//--------------------------------------------------------------------------------
	void ChangeStageInfo(CSystem* pSystem, CGameProgress* pProgress);

	//--------------------------------------------------------------------------------
	// ステージ情報の生成
	// 引　数：CSystem*			システムクラスのポインタ
	//--------------------------------------------------------------------------------
	void CreateStageInfo(CSystem* pSystem, const int fileConst);

	enum SpriteTexture
	{
		ST_MISSIONBACK,
		ST_MISSIONINFO,
		ST_STAGE_BACK,
		ST_STAGE_INFO,
		ST_MAX,
	};

	enum StageInfo
	{
		STAGE_1 = 1,
		STAGE_2,
		STAGE_3,
	};

	CSpriteAnimation* m_pSprite[ST_MAX];

	float m_halfWidth;
	float m_halfHeight;

	UINT m_nowStage;
};
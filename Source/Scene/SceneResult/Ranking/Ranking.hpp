//********************************************************************************
//
// Ranking.hpp
//
// ランキング処理
//
// Date: 2019/10/10 更新
//
//********************************************************************************

#pragma once

//================================================================================
// インクルード
//================================================================================
#include "../../GameManager/GameManager.h"

//================================================================================
// クラス定義
//================================================================================
class CRanking
{
public:
	static const int RANKING_MAX;

	//--------------------------------------------------------------------------------
	// コンストラクタ
	//--------------------------------------------------------------------------------
	CRanking();

	//--------------------------------------------------------------------------------
	// デストラクタ
	//--------------------------------------------------------------------------------
	~CRanking();

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
	// ランキング中のタイマー取得
	// 引　数：const int ランキングの番号
	//--------------------------------------------------------------------------------
	int GetRankTimer(const int index);

	//--------------------------------------------------------------------------------
	// ランキングに入力したい自己タイム
	// 引　数：const int 自己タイム
	//--------------------------------------------------------------------------------
	void SetTime(const int time);
private:

	//--------------------------------------------------------------------------------
	// ランキング読み込み
	//--------------------------------------------------------------------------------
	void ReadRandking();

	//--------------------------------------------------------------------------------
	// ランキング書き込み
	//--------------------------------------------------------------------------------
	void WriteRanking();

	//--------------------------------------------------------------------------------
	// ランキング書き込み
	//--------------------------------------------------------------------------------
	void InitWriteRanking();

	int m_ranking[5];
};
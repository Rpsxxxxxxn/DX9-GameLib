//*************************************************************
//
// IntervalTimer.h
//
// 間隔管理クラス
//
// Date: 2019/07/30 更新
//
//*************************************************************

#pragma once

//=============================================================
// インクルード
//=============================================================
#include "../Common.h"

//=============================================================
// クラス定義
//=============================================================
class CIntervalTimer
{
public:
	//-------------------------------------------------------------
	// コンストラクタ
	//-------------------------------------------------------------
	CIntervalTimer();

	//-------------------------------------------------------------
	// タイマーの設定
	// 引　数：double	繰り返す間隔の制限時間
	//-------------------------------------------------------------
	void SetTimer(double limit);

	//-------------------------------------------------------------
	// 過ぎたときのタイミング
	// 引　数：double	毎フレームの経過時間
	//-------------------------------------------------------------
	bool IsTimeOut(double dt);

private:
	double m_limiter;
	double m_elapsedTime;
};
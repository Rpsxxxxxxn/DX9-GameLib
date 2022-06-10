//*************************************************************
//
// Timer.h
//
// フレーム管理と時間取得処理
//
// Date: 2019/07/30 更新
//
//*************************************************************

#pragma once

//=============================================================
// インクルード
//=============================================================
#include "../../Common.h"

//=============================================================
// クラス定義
//=============================================================
class CTimer
{
public:

	//-------------------------------------------------------------
	// シングルトン生成
	//-------------------------------------------------------------
	static CTimer* Create();

	//-------------------------------------------------------------
	// 更新
	//-------------------------------------------------------------
	void Update();

	//-------------------------------------------------------------
	// １ループ当たりの時間
	//-------------------------------------------------------------
	double GetDeltaTime() const;

private:

	//-------------------------------------------------------------
	// 1ループ当たりの時間計算
	//-------------------------------------------------------------
	void CalcDeltaTime();

	//-------------------------------------------------------------
	// フレームを60に固定します
	//-------------------------------------------------------------
	void FixFrameRate();

	//-------------------------------------------------------------
	// コンストラクタ
	//-------------------------------------------------------------
	CTimer();

	//-------------------------------------------------------------
	// デストラクタ
	//-------------------------------------------------------------
	~CTimer();

	chrono::system_clock::time_point m_startClock;
	chrono::system_clock::time_point m_endClock;
	int m_startTime;
	int m_counter;
	double m_deltaTime;
	double m_timeScale;
	float m_fps;
	bool m_isFixFpsRate;
	static const int CONST_FPS;
};
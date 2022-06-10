//*************************************************************
//
// Line.cpp
//
// ライン描画用クラス
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "Line.h"

//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
CLine::CLine()
	: m_pLine(nullptr)
{
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CLine::~CLine()
{
	this->ShutDown();
}

//-------------------------------------------------------------
// 初期化
// 引　数：LPDIRECT3DDEVICE9	デバイスのポインタ
//-------------------------------------------------------------
void CLine::Initialize(LPDIRECT3DDEVICE9 pDevice)
{
	D3DXCreateLine(pDevice, &m_pLine);
}

//-------------------------------------------------------------
// 解放
//-------------------------------------------------------------
void CLine::ShutDown()
{
	SafeRelease(m_pLine);
}

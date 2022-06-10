//********************************************************************************
//
// FileBase.cpp
//
// ファイル読み込み用クラス（基底）
//
// Date: 2019/07/30 更新
//
//********************************************************************************

//================================================================================
// インクルード
//================================================================================
#include "FileBase.hpp"

//================================================================================
// クラス定義
//================================================================================

//--------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------
CFileBase::CFileBase(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
{
}

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
CFileBase::~CFileBase()
{
}

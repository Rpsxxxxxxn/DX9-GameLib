//*************************************************************
//
// ShaderBase.cpp
//
// シェーダクラス(基底)
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "ShaderBase.h"

//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
CShaderBase::CShaderBase()
	: m_pDevice(nullptr)
	, m_pEffect(nullptr)
{
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CShaderBase::~CShaderBase()
{
	if (m_pEffect)
	{
		m_pEffect->Release();
		m_pEffect = nullptr;
	}
}

//-------------------------------------------------------------
// 初期化
//-------------------------------------------------------------
void CShaderBase::Initialize(
	LPDIRECT3DDEVICE9 pDevice,
	const std::string path
)
{
	m_pDevice = pDevice;

	//エフェクト読み込み
	D3DXCreateEffectFromFile(
		m_pDevice, 
		(LPCSTR)path.c_str(),
		NULL,
		NULL, 
		D3DXSHADER_DEBUG,
		NULL,
		&m_pEffect,
		NULL);

	if (m_pEffect != nullptr)
		m_pEffect->OnResetDevice();
}

//-------------------------------------------------------------
// エフェクトの取得
// 戻り値：LPD3DXEFFECT			エフェクト
//-------------------------------------------------------------
LPD3DXEFFECT CShaderBase::GetEffect()
{
	return m_pEffect;
}

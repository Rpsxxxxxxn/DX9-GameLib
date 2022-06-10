//*************************************************************
//
// Texture.cpp
//
// Textureファイル読み込み用クラス
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "Texture.h"

//=============================================================
// クラス定義
//=============================================================

//-------------------------------------------------------------
// コンストラクタ
// 引　数：LPDIRECT3DDEVICE9	デバイスのポインタ
// 　　　　string				ファイルパス
//-------------------------------------------------------------
CTexture::CTexture(LPDIRECT3DDEVICE9 pDevice, string strFileName)
	: CFileBase(pDevice)
{
	m_texData.pTexture = nullptr;
	m_texData.uiWidth = 0;
	m_texData.uiHeight = 0;

	this->Initialize(strFileName);
}

//-------------------------------------------------------------
// 初期化
// 引　数：string	ファイルディレクトリ
//-------------------------------------------------------------
void CTexture::Initialize(string strFileName)
{
	D3DXIMAGE_INFO imageInfo;

	//イメージファイルの情報取得
	D3DXGetImageInfoFromFile(
		(LPCSTR)strFileName.c_str(), &imageInfo);

	m_texData.uiWidth = imageInfo.Width;
	m_texData.uiHeight = imageInfo.Height;

	//テクスチャの生成
	if (FAILED(D3DXCreateTextureFromFileExA(
		m_pDevice,
		(LPCSTR)strFileName.c_str(),
		imageInfo.Width,
		imageInfo.Height,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,
		D3DX_FILTER_NONE,
		0,
		NULL,
		NULL,
		&m_texData.pTexture
	)))
	{
		//読み込めなかった
		MessageBox(nullptr, "テクスチャの生成に失敗", "エラー", MB_OK);
	}
}

//-------------------------------------------------------------
// 解放
//-------------------------------------------------------------
void CTexture::Release()
{
	SafeRelease(m_texData.pTexture);
}

//-------------------------------------------------------------
// テクスチャの取得
//-------------------------------------------------------------
LPDIRECT3DTEXTURE9 CTexture::GetTexture()
{
	return m_texData.pTexture;
}

//-------------------------------------------------------------
// テクスチャ構造体
//-------------------------------------------------------------
TEXTURE_DATA * CTexture::GetTextureData()
{
	return &m_texData;
}

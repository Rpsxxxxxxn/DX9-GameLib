//*************************************************************
//
// FileManager.cpp
//
// ファイル管理用クラス
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "FileManager.h"

//-------------------------------------------------------------
// インスタンスの取得
//-------------------------------------------------------------
CFileManager * CFileManager::Create()
{
	static CFileManager instance;
	return &instance;
}

//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
CFileManager::CFileManager(){}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CFileManager::~CFileManager()
{
	this->Release();
}

//-------------------------------------------------------------
// 初期化
//-------------------------------------------------------------
void CFileManager::Initialize(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
}

//-------------------------------------------------------------
// 解放
//-------------------------------------------------------------
void CFileManager::Release()
{
	if (!m_aryFile.empty())
	{
		for (auto itr = m_aryFile.begin(); itr != m_aryFile.end(); itr++)
		{
			SafeDelete(itr->second);
		}

		m_aryFile.clear();
	}
}

//-------------------------------------------------------------
// XFileファイル読み込み
// 引　数：const int 	ファイルの固定番号
// 　　　　const string	ファイル名
//-------------------------------------------------------------
void CFileManager::LoadXFile(const int index, const string strFileName)
{
	//ファイルの検索
	auto itr = m_aryFile.find(index);
	if ( itr != m_aryFile.end() ) return;

	//ファイルの生成
	CFileBase* pFile = new CXFile(m_pDevice, strFileName);
	if (pFile == nullptr) return;

	m_aryFile.insert(make_pair(index, pFile));
}

//-------------------------------------------------------------
// Textureファイル読み込み
// 引　数：const int 	ファイルの固定番号
// 　　　　const string	ファイル名
//-------------------------------------------------------------
void CFileManager::LoadTexture(const int index, const string strFileName)
{
	//ファイルの検索
	auto itr = m_aryFile.find(index);
	if (itr != m_aryFile.end()) return;

	CFileBase* pFile = new CTexture(m_pDevice, strFileName);
	if (pFile == nullptr) return;

	m_aryFile.insert(make_pair(index, pFile));
}

//-------------------------------------------------------------
// ロードしたファイルの解放(省メモリ)
//-------------------------------------------------------------
void CFileManager::EraseData(const int index)
{
	//ファイルの検索
	auto itr = m_aryFile.find(index);
	if (itr == m_aryFile.end()) return;

	SafeDelete(itr->second);
}

//-------------------------------------------------------------
// テクスチャのポインタ取得
// 戻り値：LPDIRECT3DTEXTURE9	テクスチャ
//-------------------------------------------------------------
LPDIRECT3DTEXTURE9 CFileManager::GetTexture(const int index)
{
	//ファイルの検索
	auto itr = m_aryFile.find(index);
	if ( itr == m_aryFile.end() ) return nullptr;

	return dynamic_cast<CTexture*>(itr->second)->GetTexture();
}

//-------------------------------------------------------------
// テクスチャ構造体の取得
// 戻り値：TEXTURE_DATA*		テクスチャ
//-------------------------------------------------------------
TEXTURE_DATA * CFileManager::GetTextureData(const int index)
{
	//ファイルの検索
	auto itr = m_aryFile.find(index);
	if (itr == m_aryFile.end()) return nullptr;

	return dynamic_cast<CTexture*>(itr->second)->GetTextureData();
}

//-------------------------------------------------------------
// XFileクラスのポインタ取得
// 戻り値：CXFile*	XFileモデルクラス
//-------------------------------------------------------------
CXFile * CFileManager::GetXFile(const int index)
{
	//ファイルの検索
	auto itr = m_aryFile.find(index);
	if (itr == m_aryFile.end()) return nullptr;

	return dynamic_cast<CXFile*>(itr->second);
}
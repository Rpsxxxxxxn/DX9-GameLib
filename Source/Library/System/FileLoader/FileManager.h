//*************************************************************
//
// FileManager.h
//
// ファイル管理用クラス
//
// Date: 2019/07/30 更新
//
//*************************************************************

#pragma once

//=============================================================
// インクルード
//=============================================================
#include "../../Common.h"
#include "Texture/Texture.h"
#include "XFile/XFile.h"

//=============================================================
// 列挙体
//=============================================================
enum FileLoader
{
	TEXTURE,
	MODEL,
	BGM,
	SE,
};

//=============================================================
// クラス定義
//=============================================================
class CFileManager
{
public:

	//-------------------------------------------------------------
	// インスタンスの取得
	//-------------------------------------------------------------
	static CFileManager* Create();

	//-------------------------------------------------------------
	// コンストラクタ
	//-------------------------------------------------------------
	CFileManager();

	//-------------------------------------------------------------
	// デストラクタ
	//-------------------------------------------------------------
	~CFileManager();

	//-------------------------------------------------------------
	// 初期化
	//-------------------------------------------------------------
	void Initialize(LPDIRECT3DDEVICE9 pDevice);

	//-------------------------------------------------------------
	// 解放
	//-------------------------------------------------------------
	void Release();

	//-------------------------------------------------------------
	// XFileファイル読み込み
	// 引　数：const int 	ファイルの固定番号
	// 　　　　const string	ファイル名
	//-------------------------------------------------------------
	void LoadXFile(const int index, const string strFileName);

	//-------------------------------------------------------------
	// Textureファイル読み込み
	// 引　数：const int 	ファイルの固定番号
	// 　　　　const string	ファイル名
	//-------------------------------------------------------------
	void LoadTexture(const int index, const string strFileName);

	//-------------------------------------------------------------
	// ロードしたファイルの解放(省メモリ)
	// 引　数：const int 	ファイルの固定番号
	//-------------------------------------------------------------
	void EraseData(const int index);

	//-------------------------------------------------------------
	// テクスチャのポインタ取得
	// 戻り値：LPDIRECT3DTEXTURE9	テクスチャ
	//-------------------------------------------------------------
	LPDIRECT3DTEXTURE9 GetTexture(const int index);

	//-------------------------------------------------------------
	// テクスチャ構造体の取得
	// 戻り値：TEXTURE_DATA*		テクスチャ
	//-------------------------------------------------------------
	TEXTURE_DATA* GetTextureData(const int index);

	//-------------------------------------------------------------
	// XFileクラスのポインタ取得
	// 戻り値：CXFile*	XFileモデルクラス
	//-------------------------------------------------------------
	CXFile* GetXFile(const int index);
private:

	//デバイスのポインタ
	LPDIRECT3DDEVICE9 m_pDevice;

	//ファイル配列
	map<int, CFileBase*> m_aryFile;

};
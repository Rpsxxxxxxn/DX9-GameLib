//*************************************************************
//
// ShaderBase.h
//
// シェーダクラス(基底)
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
class CShaderBase
{
public:

	//-------------------------------------------------------------
	// コンストラクタ
	//-------------------------------------------------------------
	CShaderBase();

	//-------------------------------------------------------------
	// デストラクタ
	//-------------------------------------------------------------
	virtual ~CShaderBase();

	//-------------------------------------------------------------
	// 初期化処理
	// 引　数：LPDIRECT3DDEVICE9	デバイスのポインタ
	// 　　　　string				ファイルパス
	//-------------------------------------------------------------
	virtual void Initialize(LPDIRECT3DDEVICE9 pDevice, const string path);

	//-------------------------------------------------------------
	// 更新処理
	//-------------------------------------------------------------
	virtual void Update() = 0;

	//-------------------------------------------------------------
	// 数値の設定
	// 引　数：string				変数名
	// 　　　：IDirect3DTexture9	テクスチャのポインタ
	//-------------------------------------------------------------
	void SetValue(string name, IDirect3DTexture9* tex) { m_pEffect->SetTexture(name.c_str(), tex); };
	void SetValue(string name, IDirect3DTexture9& tex) { m_pEffect->SetTexture(name.c_str(), &tex); };

	//-------------------------------------------------------------
	// 数値の設定
	// 引　数：string				変数名
	// 　　　：D3DXMATRIX			行列
	//-------------------------------------------------------------
	void SetValue(string name, D3DXMATRIX* mat) { m_pEffect->SetMatrix(name.c_str(), mat); };
	void SetValue(string name, D3DXMATRIX& mat) { m_pEffect->SetMatrix(name.c_str(), &mat); };

	//-------------------------------------------------------------
	// 数値の設定
	// 引　数：string				変数名
	// 　　　：D3DXVECTOR4			４次元ベクトル
	//-------------------------------------------------------------
	void SetValue(string name, D3DXVECTOR4* vec) { m_pEffect->SetVector(name.c_str(), vec); };
	void SetValue(string name, D3DXVECTOR4& vec) { m_pEffect->SetVector(name.c_str(), &vec); };

	//-------------------------------------------------------------
	// 数値の設定
	// 引　数：string				変数名
	// 　　　：Matrix				行列
	// 　　　：UINT					配列数
	//-------------------------------------------------------------
	void SetValue(string name, D3DXMATRIX* matAry, UINT cnt) { m_pEffect->SetMatrixArray(name.c_str(), matAry, cnt); };
	void SetValue(string name, D3DXMATRIX& matAry, UINT cnt) { m_pEffect->SetMatrixArray(name.c_str(), &matAry, cnt); };

	//-------------------------------------------------------------
	// 数値の設定
	// 引　数：string				変数名
	// 　　　：INT					整数
	//-------------------------------------------------------------
	void SetValue(string name, INT* num) { m_pEffect->SetInt(name.c_str(), *num); };
	void SetValue(string name, INT& num) { m_pEffect->SetInt(name.c_str(), num); };

	//-------------------------------------------------------------
	// エフェクトの取得
	// 戻り値：LPD3DXEFFECT			エフェクト
	//-------------------------------------------------------------
	LPD3DXEFFECT GetEffect();

protected:
	LPDIRECT3DDEVICE9 m_pDevice;
	LPD3DXEFFECT m_pEffect;
};
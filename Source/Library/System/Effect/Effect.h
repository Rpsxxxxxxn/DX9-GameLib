//*************************************************************
//
// Effect.h
//
// エフェクトクラス
//
// Date: 2019/07/30 更新
//
//*************************************************************

#pragma once

//=============================================================
// インクルード
//=============================================================
#include "../../Common.h"

#include "../../../../Include/Effekseer/Effekseer.h"
#include "../../../../Include/Effekseer/EffekseerRendererDX9.h"
#include "../../../../Include/Effekseer/EffekseerSoundXAudio2.h"

#if _DEBUG
#pragma comment(lib, "Debug/Effekseer.lib" )
#pragma comment(lib, "Debug/EffekseerRendererDX9.lib" )
#pragma comment(lib, "Debug/EffekseerSoundXAudio2.lib" )
#else
#pragma comment(lib, "Release/Effekseer.lib" )
#pragma comment(lib, "Release/EffekseerRendererDX9.lib" )
#pragma comment(lib, "Release/EffekseerSoundXAudio2.lib" )
#endif

//=============================================================
// 前方宣言
//=============================================================
class CCamera;

//=============================================================
// クラス定義
//=============================================================
class CEffectSystem
{
public:

	//-------------------------------------------------------------
	// シングルトン生成
	//-------------------------------------------------------------
	static CEffectSystem* Create();

	//-------------------------------------------------------------
	// コンストラクタ
	//-------------------------------------------------------------
	CEffectSystem();

	//-------------------------------------------------------------
	// デストラクタ
	//-------------------------------------------------------------
	~CEffectSystem();

	//-------------------------------------------------------------
	// 初期化
	// 引　数：LPDIRECT3DDEVICE9*　	デバイスのポインタ
	// 　　　：UINT					表示最大スプライト数
	//-------------------------------------------------------------
	void Initialize(LPDIRECT3DDEVICE9 pDevice, UINT drawCntMax);

	//-------------------------------------------------------------
	// データの設定
	// 引　数：const float 視野角（ラジアン角）
	// 　　　　const float アスペクト比（width / height）
	// 　　　　const float 前方クリップ面
	// 　　　　const float 後方クリップ面
	//-------------------------------------------------------------
	void InitProjection(
		const float fov,
		const float aspect, 
		const float nearClip, 
		const float farClip);

	//-------------------------------------------------------------
	// エフェクトの追加
	// 引　数：EFK_CHAR*	ファイル名
	//-------------------------------------------------------------
	void AddEffect(const EFK_CHAR* strFileName);

	//-------------------------------------------------------------
	// エフェクトの追加
	// 引　数：const int	配列の番目
	// 　　　　EFK_CHAR*	ファイル名
	//-------------------------------------------------------------
	void AddEffect(const int index, const EFK_CHAR* strFileName);

	//-------------------------------------------------------------
	// エフェクトの再生
	// 引　数：const int	配列の番目
	// 　　　　D3DXVECTOR3	3次元ベクトル
	//-------------------------------------------------------------
	void Play(const int index, D3DXVECTOR3& VecPos);

	//-------------------------------------------------------------
	// エフェクトの再生
	// 引　数：const int	配列の番目
	// 　　　　D3DXVECTOR3*	3次元ベクトルポインタ
	//-------------------------------------------------------------
	void Play(const int index, D3DXVECTOR3* pVecPos);

	//-------------------------------------------------------------
	// カメラ座標の更新
	// 引　数：Camera*	カメラクラスのポインタ
	//-------------------------------------------------------------
	void Update(CCamera* pCamera);

	//-------------------------------------------------------------
	// エフェクト描画
	//-------------------------------------------------------------
	void Draw();

private:

	//-------------------------------------------------------------
	// ベクトルクラス変換(EffekVec3 -> D3DXVec3)
	// 引　数：D3DXVECTOR3	３次元ベクトルクラス
	//-------------------------------------------------------------
	Effekseer::Vector3D ConvertD3DXVec3(D3DXVECTOR3 Vec3);

	//-------------------------------------------------------------
	// ベクトルクラス変換(D3DXVec3 -> EffekVec3)
	// 引　数：Effekseer::Vector3D	3次元ベクトルクラス
	//-------------------------------------------------------------
	D3DXVECTOR3 ConvertEffekVec3(Effekseer::Vector3D Vec3);

	//ハンドル
	Effekseer::Handle m_hHandle;

	//エフェクト配列
	vector<Effekseer::Effect*> m_pEffect;

	//描画管理
	EffekseerRenderer::Renderer* m_pRenderer;

	//エフェクト管理
	Effekseer::Manager* m_pManager;

	//コピー防止
	CEffectSystem& operator=(const CEffectSystem&);
	CEffectSystem(const CEffectSystem&);
};
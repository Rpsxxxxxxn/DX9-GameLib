//*************************************************************
//
// デプスレンダリング
//
// ShadowDepth.h
//
// Cascaded Shadow Maps
//
//*************************************************************

#pragma once

//=============================================================
// インクルード
//=============================================================
#include "../ShaderBase.h"
#include "../../../Graphics/Camera/Camera.h"

//================================================================
// クラス定義
//================================================================
class CShadowDepth
	: public CShaderBase
{
public:
	//分裂数
	static const int CASCADE_SPLIT_COUNT = 3;

	//-------------------------------------------------------------
	// コンストラクタ
	// 引　数：const UINT	横幅
	// 　　　　const UINT	縦幅
	//-------------------------------------------------------------
	CShadowDepth(const UINT width, const UINT height);

	//-------------------------------------------------------------
	// デストラクタ
	//-------------------------------------------------------------
	~CShadowDepth();

	//-------------------------------------------------------------
	// 初期化
	// 引　数：LPDIRECT3DDEVICE9 デバイスのポインタ
	//-------------------------------------------------------------
	void Initialize(LPDIRECT3DDEVICE9 pDevice);

	//-------------------------------------------------------------
	// 更新
	//-------------------------------------------------------------
	void Update();

	//-------------------------------------------------------------
	// デプスレンダリング開始
	// 引　数：int	番号
	//-------------------------------------------------------------
	void StartDepthRender(const int index);

	//-------------------------------------------------------------
	// デプスレンダリング終了
	//-------------------------------------------------------------
	void EndDepthRender();

	//-------------------------------------------------------------
	// シャドウテクスチャの取得
	// 引　数：int	番号
	//-------------------------------------------------------------
	LPDIRECT3DTEXTURE9 GetTexture(const int index) const;

	//-------------------------------------------------------------
	// 視推台の計算
	// 引　数：Camera カメラクラス
	// 　　　　int	  分割番号
	// 　　　　bool	  更新をするか
	//-------------------------------------------------------------
	void CalculateCascadePlanes();

	//-------------------------------------------------------------
	// 分割シャドウマップの深度計算
	//-------------------------------------------------------------
	void CalculateShadowMapSplitDepths();

	//-------------------------------------------------------------
	// シャドウマップ行列の再計算
	// 引　数：Camera		カメラクラス
	//-------------------------------------------------------------
	void RecalculateShadowMapMatrices(CCamera* camera);

	//-------------------------------------------------------------
	// ライトビュー行列
	// 引　数：D3DXMATRIX*	ビュー行列のポインタ
	//-------------------------------------------------------------
	void SetLightViewMatrix(D3DXMATRIX* pMatView);

	//-------------------------------------------------------------
	// ライトプロジェクション行列
	// 引　数：D3DXMATRIX*	プロジェクション行列のポインタ
	//-------------------------------------------------------------
	void SetLightProjMatrix(D3DXMATRIX* pMatProj);

	//-------------------------------------------------------------
	// ライトプロジェクション行列
	// 引　数：D3DXVECTOR3*		ライト位置ベクトルのポインタ
	//-------------------------------------------------------------
	void SetLightPosition(D3DXVECTOR3* pVecLightPos);

	//-------------------------------------------------------------
	// ビュープロジェクションの行列を取得
	// 引　数：const int		分割番号
	//-------------------------------------------------------------
	D3DXMATRIX GetViewProjMatrix(const int index);

private:
	//-------------------------------------------------------------
	// 分割シャドウの行列計算
	// 引　数：Camera カメラクラス
	// 　　　　int	  分割番号
	// 　　　　bool	  更新をするか
	//-------------------------------------------------------------
	void CalculateShadowMatrices(CCamera* camera, int i, bool forceUpdate);

	LPDIRECT3DTEXTURE9 m_pShadow[CASCADE_SPLIT_COUNT];
	LPDIRECT3DSURFACE9 m_pShadowSurf[CASCADE_SPLIT_COUNT];
	LPDIRECT3DSURFACE9 m_pShadowZ[CASCADE_SPLIT_COUNT];

	LPDIRECT3DSURFACE9 m_pOldBackBuffer;
	LPDIRECT3DSURFACE9 m_pOldZBuffer;
	D3DVIEWPORT9 m_oldViewport;

	D3DXMATRIX m_matWorld;
	D3DXMATRIX m_matLView;
	D3DXMATRIX m_matLProj;
	D3DXMATRIX m_matViewProj[CASCADE_SPLIT_COUNT];
	D3DXVECTOR3 m_previousMinValues[CASCADE_SPLIT_COUNT];
	D3DXVECTOR3 m_vecLightPosition;

	FLOAT m_shadowSplitDepths[CASCADE_SPLIT_COUNT + 1];
	FLOAT m_blendDistance;
	FLOAT m_radii[CASCADE_SPLIT_COUNT];
	D3DXPLANE** m_ppCascadePlanes;

	UINT m_width;
	UINT m_height;
};
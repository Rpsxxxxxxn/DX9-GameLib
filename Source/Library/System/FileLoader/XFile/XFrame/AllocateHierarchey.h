//*************************************************************
//
// AllocateHierarchey.hpp
//
// XFile読み込みクラス
//
// Date: 2019/07/30 更新
//
//*************************************************************

#pragma once

//=============================================================
// インクルード
//=============================================================
#include "../../../../Common.h"

//=============================================================
// D3DXFRAMEの拡張
//=============================================================
struct D3DXFRAME_DERIVED : public D3DXFRAME
{
	D3DXMATRIXA16        CombinedTransformationMatrix;
};

//=============================================================
// D3DXMESHCONTAINERの拡張
//=============================================================
struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*  ppDecaleTextures;
	LPDIRECT3DTEXTURE9*	 ppNormalTextures;

	LPD3DXMESH           pOrigMesh;
	LPD3DXATTRIBUTERANGE pAttributeTable;
	DWORD                NumAttributeGroups;
	DWORD                NumInfl;
	LPD3DXBUFFER         pBoneCombinationBuf;
	D3DXMATRIX**         ppBoneMatrixPtrs;
	D3DXMATRIX*          pBoneOffsetMatrices;
	DWORD                NumPaletteEntries;
	bool                 UseSoftwareVP;
	DWORD                iAttributeSW;
	LPDIRECT3DVERTEXDECLARATION9 pDeclaration;
};

//=============================================================
// クラス定義
//=============================================================
class CAllocateHierarchy : public ID3DXAllocateHierarchy
{
public:
	//-------------------------------------------------------------
	// フレームの作成
	//-------------------------------------------------------------
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);

	//-------------------------------------------------------------
	// メッシュコンテナの作成
	//-------------------------------------------------------------
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer);

	//-------------------------------------------------------------
	// フレームの削除
	//-------------------------------------------------------------
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);

	//-------------------------------------------------------------
	// メッシュコンテナの削除
	//-------------------------------------------------------------
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);

	//-------------------------------------------------------------
	// コンストラクタ
	//-------------------------------------------------------------
	CAllocateHierarchy() : m_numBoneMatricesMax(0), m_pBoneMatrices(nullptr) {};

private:
	//-------------------------------------------------------------
	// スキニングメッシュの作成
	//-------------------------------------------------------------
	HRESULT GenerateSkinnedMesh(LPDIRECT3DDEVICE9 pDevice, D3DXMESHCONTAINER_DERIVED *pMeshContainer);

	//ボーンの行列数最大値
	UINT m_numBoneMatricesMax;

	//ボーンの行列
	D3DXMATRIX* m_pBoneMatrices;
};
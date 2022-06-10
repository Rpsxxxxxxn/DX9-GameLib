//*************************************************************
//
// AllocateHierarchey.hpp
//
// XFile�ǂݍ��݃N���X
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

#pragma once

//=============================================================
// �C���N���[�h
//=============================================================
#include "../../../../Common.h"

//=============================================================
// D3DXFRAME�̊g��
//=============================================================
struct D3DXFRAME_DERIVED : public D3DXFRAME
{
	D3DXMATRIXA16        CombinedTransformationMatrix;
};

//=============================================================
// D3DXMESHCONTAINER�̊g��
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
// �N���X��`
//=============================================================
class CAllocateHierarchy : public ID3DXAllocateHierarchy
{
public:
	//-------------------------------------------------------------
	// �t���[���̍쐬
	//-------------------------------------------------------------
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);

	//-------------------------------------------------------------
	// ���b�V���R���e�i�̍쐬
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
	// �t���[���̍폜
	//-------------------------------------------------------------
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);

	//-------------------------------------------------------------
	// ���b�V���R���e�i�̍폜
	//-------------------------------------------------------------
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);

	//-------------------------------------------------------------
	// �R���X�g���N�^
	//-------------------------------------------------------------
	CAllocateHierarchy() : m_numBoneMatricesMax(0), m_pBoneMatrices(nullptr) {};

private:
	//-------------------------------------------------------------
	// �X�L�j���O���b�V���̍쐬
	//-------------------------------------------------------------
	HRESULT GenerateSkinnedMesh(LPDIRECT3DDEVICE9 pDevice, D3DXMESHCONTAINER_DERIVED *pMeshContainer);

	//�{�[���̍s�񐔍ő�l
	UINT m_numBoneMatricesMax;

	//�{�[���̍s��
	D3DXMATRIX* m_pBoneMatrices;
};
//*************************************************************
//
// AllocateHierarchey.hpp
//
// XFile読み込みクラス
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "AllocateHierarchey.h"

//=============================================================
// クラス定義
//=============================================================

//-------------------------------------------------------------
// メッシュの名前
//-------------------------------------------------------------
HRESULT AllocateName(LPCSTR Name, LPSTR *pNewName)
{
	UINT cbLength;

	if (Name != nullptr)
	{
		cbLength = (UINT)strlen(Name) + 1;
		*pNewName = new CHAR[cbLength];
		if (*pNewName == nullptr)
			return E_OUTOFMEMORY;
		memcpy(*pNewName, Name, cbLength * sizeof(CHAR));
	}
	else
	{
		*pNewName = nullptr;
	}

	return S_OK;
}

//-------------------------------------------------------------
// フレームの作成
//-------------------------------------------------------------
HRESULT CAllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	HRESULT hr = S_OK;
	D3DXFRAME_DERIVED *pFrame = new D3DXFRAME_DERIVED;

	*ppNewFrame = nullptr;

	if (pFrame == nullptr)
	{
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}

	hr = AllocateName(Name, &pFrame->Name);
	if (FAILED(hr))
		goto e_Exit;

	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	pFrame->pMeshContainer = nullptr;
	pFrame->pFrameSibling = nullptr;
	pFrame->pFrameFirstChild = nullptr;

	*ppNewFrame = pFrame;
	pFrame = nullptr;

e_Exit:
	delete pFrame;
	return hr;
}

//-------------------------------------------------------------
// メッシュコンテナの作成
//-------------------------------------------------------------
HRESULT CAllocateHierarchy::CreateMeshContainer(
	LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *,
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	HRESULT hr;
	D3DXMESHCONTAINER_DERIVED *pMeshContainer = nullptr;
	UINT NumFaces;
	UINT iMaterial;
	UINT uiBoneNum;
	LPDIRECT3DDEVICE9 pd3dDevice = nullptr;

	LPD3DXMESH pMesh = nullptr;

	*ppNewMeshContainer = nullptr;

	if (pMeshData->Type != D3DXMESHTYPE_MESH)
	{
		hr = E_FAIL;
		goto e_Exit;
	}

	pMesh = pMeshData->pMesh;

	if (pMesh->GetFVF() == 0)
	{
		hr = E_FAIL;
		goto e_Exit;
	}

	pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	if (pMeshContainer == nullptr)
	{
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}
	memset(pMeshContainer, 0, sizeof(D3DXMESHCONTAINER_DERIVED));

	hr = AllocateName(Name, &pMeshContainer->Name);
	if (FAILED(hr))
		goto e_Exit;

	pMesh->GetDevice(&pd3dDevice);
	NumFaces = pMesh->GetNumFaces();

	if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
	{
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		hr = pMesh->CloneMeshFVF(pMesh->GetOptions(),
			pMesh->GetFVF() | D3DFVF_NORMAL,
			pd3dDevice, &pMeshContainer->MeshData.pMesh);
		if (FAILED(hr))
			goto e_Exit;

		pMesh = pMeshContainer->MeshData.pMesh;

		D3DXComputeNormals(pMesh, nullptr);
	}
	else
	{
		pMeshContainer->MeshData.pMesh = pMesh;
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		pMesh->AddRef();
	}

	pMeshContainer->NumMaterials = max(1, NumMaterials);
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[NumFaces * 3];
	pMeshContainer->ppDecaleTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	pMeshContainer->ppNormalTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	if ((pMeshContainer->pAdjacency == nullptr) || (pMeshContainer->pMaterials == nullptr))
	{
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}

	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumFaces * 3);
	memset(pMeshContainer->ppDecaleTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);
	memset(pMeshContainer->ppNormalTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

	// マテリアルがあるかどうか
	if (NumMaterials > 0)
	{
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

		for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
		{
			if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != nullptr)
			{
				pMeshContainer->pMaterials[iMaterial].MatD3D.Ambient = pMeshContainer->pMaterials[iMaterial].MatD3D.Diffuse;

				string texCoord = pMeshContainer->pMaterials[iMaterial].pTextureFilename;

				//-------------------------------------------------------------
				// 通常テクスチャの生成
				string directory = "Resource/Models/DecaleTexture/" + texCoord;

				if (FAILED(D3DXCreateTextureFromFileExA(pd3dDevice,
					(LPCSTR)directory.c_str(),
					0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
					D3DX_DEFAULT, D3DX_DEFAULT, 0, nullptr, nullptr,
					&pMeshContainer->ppDecaleTextures[iMaterial]
				)))
				{
					pMeshContainer->ppDecaleTextures[iMaterial] = nullptr;
				}

				//-------------------------------------------------------------
				// ノーマルマップの生成
				directory = "Resource/Models/NormalTexture/" + texCoord;

				if (FAILED(D3DXCreateTextureFromFileExA(pd3dDevice,
					(LPCSTR)directory.c_str(),
					0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
					D3DX_DEFAULT, D3DX_DEFAULT, 0, nullptr, nullptr,
					&pMeshContainer->ppNormalTextures[iMaterial]
				)))
				{
					pMeshContainer->ppNormalTextures[iMaterial] = nullptr;
				}

				//-------------------------------------------------------------
				//ディレクトリは削除
				pMeshContainer->pMaterials[iMaterial].pTextureFilename = nullptr;
			}
		}
	}
	else
	{
		//マテリアル情報がないとき
		pMeshContainer->pMaterials[0].pTextureFilename = nullptr;
		ZeroMemory(&pMeshContainer->pMaterials[0].MatD3D, sizeof(D3DMATERIAL9));
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Ambient = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}

	//スキンメッシュ情報があるとき
	if (pSkinInfo != nullptr)
	{
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		pMeshContainer->pOrigMesh = pMesh;
		pMesh->AddRef();
		uiBoneNum = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[uiBoneNum];

		if (pMeshContainer->pBoneOffsetMatrices == nullptr)
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		for (UINT i = 0; i < uiBoneNum; i++)
		{
			pMeshContainer->pBoneOffsetMatrices[i] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i));
		}

		hr = GenerateSkinnedMesh(pd3dDevice, pMeshContainer);
		if (FAILED(hr)) goto e_Exit;

	}

	*ppNewMeshContainer = pMeshContainer;
	pMeshContainer = nullptr;

e_Exit:
	SafeRelease(pd3dDevice);

	if (pMeshContainer != nullptr)
	{
		DestroyMeshContainer(pMeshContainer);
	}

	return hr;
}

//-------------------------------------------------------------
// メッシュコンテナの削除
//-------------------------------------------------------------
HRESULT CAllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	UINT iMaterial;
	D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

	SafeDeleteArray(pMeshContainer->Name);
	SafeDeleteArray(pMeshContainer->pAdjacency);
	SafeDeleteArray(pMeshContainer->pMaterials);
	SafeDeleteArray(pMeshContainer->pBoneOffsetMatrices);

	//全てのテクスチャの開放
	if (pMeshContainer->ppDecaleTextures != nullptr)
	{
		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			SafeRelease(pMeshContainer->ppDecaleTextures[iMaterial]);
		}
	}

	SafeDeleteArray(pMeshContainer->ppDecaleTextures);
	SafeDeleteArray(pMeshContainer->ppNormalTextures);
	SafeDeleteArray(pMeshContainer->ppBoneMatrixPtrs);
	SafeRelease(pMeshContainer->pDeclaration);
	SafeRelease(pMeshContainer->pBoneCombinationBuf);
	SafeRelease(pMeshContainer->MeshData.pMesh);
	SafeRelease(pMeshContainer->pSkinInfo);
	SafeRelease(pMeshContainer->pOrigMesh);
	SafeDeleteArray(m_pBoneMatrices);
	SafeDelete(pMeshContainer);
	return S_OK;
}

//-------------------------------------------------------------------
// フレームの削除
//-------------------------------------------------------------------
HRESULT CAllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	SafeDeleteArray(pFrameToFree->Name);
	SafeDelete(pFrameToFree);
	return S_OK;
}

//-------------------------------------------------------------------
// スキニングメッシュの作成
//-------------------------------------------------------------------
HRESULT CAllocateHierarchy::GenerateSkinnedMesh(LPDIRECT3DDEVICE9 pDevice, D3DXMESHCONTAINER_DERIVED *pMeshContainer)
{
	HRESULT hr = S_OK;
	D3DCAPS9 d3dCaps;
	pDevice->GetDeviceCaps(&d3dCaps);

	if (pMeshContainer->pSkinInfo == nullptr) return hr;

	SafeRelease(pMeshContainer->MeshData.pMesh);
	SafeRelease(pMeshContainer->pBoneCombinationBuf);

	UINT MaxMatrices = 50;
	pMeshContainer->NumPaletteEntries = min(MaxMatrices, pMeshContainer->pSkinInfo->GetNumBones());

	DWORD Flags = D3DXMESHOPT_VERTEXCACHE;
	if (d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1, 1))
	{
		pMeshContainer->UseSoftwareVP = false;
		Flags |= D3DXMESH_MANAGED;
	}
	else
	{
		pMeshContainer->UseSoftwareVP = true;
		Flags |= D3DXMESH_SYSTEMMEM;
	}

	SafeRelease(pMeshContainer->MeshData.pMesh);

	hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
	(
		pMeshContainer->pOrigMesh,
		Flags,
		pMeshContainer->NumPaletteEntries,
		pMeshContainer->pAdjacency,
		nullptr, nullptr, nullptr,
		&pMeshContainer->NumInfl,
		&pMeshContainer->NumAttributeGroups,
		&pMeshContainer->pBoneCombinationBuf,
		&pMeshContainer->MeshData.pMesh
	);

	DWORD NewFVF = (pMeshContainer->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
	if (NewFVF != pMeshContainer->MeshData.pMesh->GetFVF())
	{
		LPD3DXMESH pMesh;
		hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(pMeshContainer->MeshData.pMesh->GetOptions(), NewFVF, pDevice, &pMesh);
		if (!FAILED(hr))
		{
			pMeshContainer->MeshData.pMesh->Release();
			pMeshContainer->MeshData.pMesh = pMesh;
			pMesh = nullptr;
		}
	}

	//頂点データを定義する
	D3DVERTEXELEMENT9 VertexPNTBTElements[] =
	{
		{ 0,  0, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0 },
		{ 0, 24, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
		{ 0, 36, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
		{ 0, 48, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	LPD3DXMESH pTempMesh = nullptr;

	//メッシュ情報をコピーする
	if (pMeshContainer->MeshData.pMesh)
	{
		if (FAILED(pMeshContainer->MeshData.pMesh->CloneMesh(pMeshContainer->MeshData.pMesh->GetOptions(), VertexPNTBTElements, pDevice, &pTempMesh)))
		{
			SafeRelease(pTempMesh);
			return E_FAIL;
		}
	}

	//頂点情報を参照し、法線ベクトル、接線ベクトル、従法線ベクトルがあるか調べる
	DWORD Normal = D3DX_DEFAULT;
	DWORD Tangent = D3DX_DEFAULT;
	DWORD Binormal = D3DX_DEFAULT;
	if (pTempMesh)
	{
		for (UINT index = 0; index < D3DXGetDeclLength(VertexPNTBTElements); ++index)
		{
			if (VertexPNTBTElements[index].Usage == D3DDECLUSAGE_NORMAL)
			{
				Normal = D3DDECLUSAGE_NORMAL;
			}
			else if (VertexPNTBTElements[index].Usage == D3DDECLUSAGE_TANGENT)
			{
				Tangent = D3DDECLUSAGE_TANGENT;
			}
			else if (VertexPNTBTElements[index].Usage == D3DDECLUSAGE_BINORMAL)
			{
				Binormal = D3DDECLUSAGE_BINORMAL;
			}
		}
	}

	if (pTempMesh)
	{
		//頂点情報に基づき頂点データを再生成する   
		if (Normal != D3DX_DEFAULT || Tangent != D3DX_DEFAULT || Binormal != D3DX_DEFAULT)
		{
			D3DXComputeTangentFrameEx(
				pTempMesh,
				D3DDECLUSAGE_TEXCOORD, 0,
				D3DDECLUSAGE_TANGENT, 0,
				D3DDECLUSAGE_BINORMAL, 0,
				D3DDECLUSAGE_NORMAL, 0,
				0, nullptr,
				0.01f,    //ボケ具合.値をおおきくするとぼけなくなる
				0.25f,
				0.01f,
				&pMeshContainer->MeshData.pMesh,
				nullptr
			);
		}
		else {
			pTempMesh->CloneMesh(pTempMesh->GetOptions(),
				VertexPNTBTElements,
				pDevice,
				&pMeshContainer->MeshData.pMesh);
		}

		pTempMesh = nullptr;
	}

	pMeshContainer->MeshData.pMesh->UpdateSemantics(VertexPNTBTElements);

	pDevice->CreateVertexDeclaration(VertexPNTBTElements, &pMeshContainer->pDeclaration);

	if (m_numBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones())
	{
		m_numBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();
	}

	return hr;
}
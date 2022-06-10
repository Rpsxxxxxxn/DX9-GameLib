//*************************************************************
//
// XFrame.hpp
//
// �t�@�C���ǂݍ��ݗp�N���X�i���j
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "XFrame.h"
#include "../../../Shader/ShaderManager.h"

//-------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------
CXFrame::CXFrame()
	: m_pFrameRoot(nullptr)
{
}

//-------------------------------------------------------------
// ���b�V���R���e�i�̕`��
//-------------------------------------------------------------
void CXFrame::DrawMeshContainer(
	LPDIRECT3DDEVICE9 pDevice, 
	LPD3DXMESHCONTAINER pMeshContainerBase, 
	LPD3DXFRAME pFrameBase, 
	LPD3DXMATRIX pMatWorld,
	float meshAlpha)
{
	auto pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
	Matrix matStacks[50];
	D3DCAPS9 d3dCaps;

	pDevice->GetDeviceCaps(&d3dCaps);

	auto state = CShaderManager::GetInstance().GetShaderState();
	auto pShader = CShaderManager::GetInstance().GetShader(state);

	if (pMeshContainer->pSkinInfo)
	{
		auto pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());
		for (DWORD iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
		{
			for (DWORD iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry)
			{
				DWORD iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];

				if (iMatrixIndex != UINT_MAX)
				{
					matStacks[iPaletteEntry] = pMeshContainer->pBoneOffsetMatrices[iMatrixIndex] * *pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex];
				}
			}

			pShader->SetValue("DecaleTexture", pMeshContainer->ppDecaleTextures[pBoneComb[iAttrib].AttribId]);
			pShader->SetValue("BumpTexture", pMeshContainer->ppNormalTextures[pBoneComb[iAttrib].AttribId]);
			pShader->SetValue("g_vDiffuse", (Vector4*)&(pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Diffuse));
			pShader->SetValue("g_vAmbient", (Vector4*)&(pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Ambient));
			pShader->SetValue("g_mWStack", matStacks, pMeshContainer->NumPaletteEntries);

			int stack = pMeshContainer->NumPaletteEntries;
			pShader->SetValue("g_iStackNum", stack);
			pShader->SetValue("g_mWorld", pMatWorld);

			pShader->GetEffect()->SetFloat("g_meshAlpha", meshAlpha);
			pShader->GetEffect()->Begin(nullptr, 0);
			pShader->GetEffect()->BeginPass(1);
			pShader->GetEffect()->CommitChanges();
			pDevice->SetVertexDeclaration(pMeshContainer->pDeclaration);
			pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
			pShader->GetEffect()->EndPass();
			pShader->GetEffect()->End();
		}
	}
	else {
		pShader->SetValue("g_mWorld", pMatWorld);

		for (DWORD iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			pShader->SetValue("DecaleTexture", pMeshContainer->ppDecaleTextures[iMaterial]);
			pShader->SetValue("BumpTexture", pMeshContainer->ppNormalTextures[iMaterial]);
			pShader->GetEffect()->SetVector("g_vDiffuse", (Vector4*)&(pMeshContainer->pMaterials[iMaterial].MatD3D.Diffuse));
			pShader->GetEffect()->SetVector("g_vAmbient", (Vector4*)&(pMeshContainer->pMaterials[iMaterial].MatD3D.Ambient));
			pShader->GetEffect()->SetFloat("g_meshAlpha", meshAlpha);
			pShader->GetEffect()->Begin(nullptr, 0);
			pShader->GetEffect()->BeginPass(0);
			pShader->GetEffect()->CommitChanges();
			pDevice->SetVertexDeclaration(pMeshContainer->pDeclaration);
			pMeshContainer->MeshData.pMesh->DrawSubset(iMaterial);
			pShader->GetEffect()->EndPass();
			pShader->GetEffect()->End();
		}
	}
}

//-------------------------------------------------------------
// �t���[���̕`��
//-------------------------------------------------------------
void CXFrame::DrawFrame(LPDIRECT3DDEVICE9 pDevice, LPD3DXFRAME pFrame, LPD3DXMATRIX pMatWorld, float meshAlpha)
{
	auto pMeshContainer = pFrame->pMeshContainer;

	while (pMeshContainer != nullptr)
	{
		DrawMeshContainer(pDevice, pMeshContainer, pFrame, pMatWorld, meshAlpha);

		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	//�e�t���[��������
	if (pFrame->pFrameSibling != nullptr)
	{
		DrawFrame(pDevice, pFrame->pFrameSibling, pMatWorld, meshAlpha);
	}

	//�q�t���[��������
	if (pFrame->pFrameFirstChild != nullptr)
	{
		DrawFrame(pDevice, pFrame->pFrameFirstChild, pMatWorld, meshAlpha);
	}
}

//-------------------------------------------------------------
// �{�[���}�g���b�N�X�̃��b�V��
//-------------------------------------------------------------
HRESULT CXFrame::SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	auto pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

	if (pMeshContainer->pSkinInfo != nullptr)
	{
		auto cBones = pMeshContainer->pSkinInfo->GetNumBones();

		pMeshContainer->ppBoneMatrixPtrs = new Matrix*[cBones];
		if (pMeshContainer->ppBoneMatrixPtrs == nullptr)
		{
			return E_OUTOFMEMORY;
		}

		for (UINT iBone = 0; iBone < cBones; iBone++)
		{
			auto pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(iBone));

			if (pFrame == nullptr)
			{
				return E_FAIL;
			}

			pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;
		}
	}

	return S_OK;
}

//-------------------------------------------------------------
// �{�[���}�g���b�N�X
//-------------------------------------------------------------
HRESULT CXFrame::SetupBoneMatrixPointers(LPD3DXFRAME pFrame)
{
	auto hr = S_OK;

	if (pFrame->pMeshContainer != nullptr)
	{
		hr = SetupBoneMatrixPointersOnMesh(pFrame->pMeshContainer);
		if (FAILED(hr)) return hr;
	}

	if (pFrame->pFrameSibling != nullptr)
	{
		hr = SetupBoneMatrixPointers(pFrame->pFrameSibling);
		if (FAILED(hr)) return hr;
	}

	if (pFrame->pFrameFirstChild != nullptr)
	{
		hr = SetupBoneMatrixPointers(pFrame->pFrameFirstChild);
		if (FAILED(hr)) return hr;
	}

	return hr;
}

//-------------------------------------------------------------
// �t���[���s��̍X�V
//-------------------------------------------------------------
void CXFrame::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	auto pFrame = (D3DXFRAME_DERIVED*)pFrameBase;

	if (pParentMatrix != nullptr) {
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	}
	else {
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	}

	if (pFrame->pFrameSibling != nullptr)
	{
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}

	if (pFrame->pFrameFirstChild != nullptr)
	{
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}
}

//-------------------------------------------------------------
// �{�[���̃}�g���b�N�X�s��擾
//-------------------------------------------------------------
Matrix CXFrame::GetBoneMatrix(string strBoneName, Matrix * pMatWorld)
{
	auto pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pFrameRoot, strBoneName.c_str());
	return pFrame->CombinedTransformationMatrix;
}

//-------------------------------------------------------------
// �t���[���̎擾
//-------------------------------------------------------------
LPD3DXFRAME * CXFrame::GetFrame()
{
	return &m_pFrameRoot;
};

//-------------------------------------------------------------
// �t���[��������t���[�����擾
// �߂�l�F�t���[���̃A�h���X
// ���@���FLPCSTR �t���[����
//-------------------------------------------------------------
LPD3DXFRAME CXFrame::GetFrameName(LPCSTR framename)
{
	return D3DXFrameFind(m_pFrameRoot, framename);
}

//-------------------------------------------------------------
// ���[�g�t���[�����̃��b�V���ւ̃|�C���^���擾
// �߂�l�F���[�g�t���[�����̃��b�V���ւ̃|�C���^
//-------------------------------------------------------------
LPD3DXMESH CXFrame::GetRootMesh() const
{
	return m_pFrameRoot->pMeshContainer->MeshData.pMesh;
};
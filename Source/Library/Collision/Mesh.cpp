//*************************************************************
//
// Mesh.cpp
//
// �����蔻��N���X(���b�V��)
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "Mesh.h"

//=============================================================
// �N���X��`
//=============================================================

//-------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------
CMesh::CMesh()
	: m_pMeshData(nullptr)
{
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
CMesh::~CMesh()
{
	SafeDeleteArray(m_pMeshData);
}

//-------------------------------------------------------------
// ������
//-------------------------------------------------------------
void CMesh::Create(CXFile * m_pFrame)
{
	m_pMesh = m_pFrame;

	//���[�g���b�V���擾
	LPD3DXMESH pMesh = m_pMesh->GetRootMesh();

	//���_���
	DWORD numVtxBytes = pMesh->GetNumBytesPerVertex();

	//�|���S����
	m_numFaces = pMesh->GetNumFaces();

	//���b�V���Ł[���̐���
	m_pMeshData = new Mesh_Data[m_numFaces];

	//�אڃf�[�^�z��̍쐬
	DWORD* pAdjacency = new DWORD[m_numFaces * 3];

	//�אڃf�[�^�̎擾
	pMesh->GenerateAdjacency(0.01f, pAdjacency);

	//���_�o�b�t�@�̃��b�N
	BYTE* pVtx;
	pMesh->LockVertexBuffer(0, (void**)&pVtx);

	//�C���f�b�N�X�o�b�t�@�̃��b�N
	WORD* pIdx;
	pMesh->LockIndexBuffer(0, (void**)&pIdx);

	//�|���S���f�[�^�Z�b�g
	for (DWORD i = 0; i < m_numFaces; i++)
	{
		m_pMeshData[i].vVertex[0] = 
			*(Vector3*)(pVtx + (numVtxBytes * (pIdx)[i * 3 + 0]));
		m_pMeshData[i].vVertex[1] = 
			*(Vector3*)(pVtx + (numVtxBytes * (pIdx)[i * 3 + 1]));
		m_pMeshData[i].vVertex[2] = 
			*(Vector3*)(pVtx + (numVtxBytes * (pIdx)[i * 3 + 2]));

		m_pMeshData[i].vNormal = ComputeNormals(m_pMeshData[i].vVertex);

		m_pMeshData[i].dwAdjacency[0] = pAdjacency[i * 3 + 0];
		m_pMeshData[i].dwAdjacency[1] = pAdjacency[i * 3 + 1];
		m_pMeshData[i].dwAdjacency[2] = pAdjacency[i * 3 + 2];
	}

	pMesh->UnlockVertexBuffer();
	pMesh->UnlockIndexBuffer();

	delete[] pAdjacency;
}

//-------------------------------------------------------------
// �m�F�t���O��������
//-------------------------------------------------------------
void CMesh::ClearCheckFlg()
{
	for (DWORD i = 0; i < m_numFaces; i++) 
	{
		m_pMeshData[i].bChecked = false;
	}
}

//-------------------------------------------------------------
// �|���S���̖@���v�Z
//-------------------------------------------------------------
Vector3 CMesh::ComputeNormals(const Vector3 vtx[3])
{
	Vector3 vN;
	Vector3 v1 = vtx[1] - vtx[0];
	Vector3 v2 = vtx[2] - vtx[0];

	D3DXVec3Cross(&vN, &v1, &v2);
	D3DXVec3Normalize(&vN, &vN);

	return vN;
}

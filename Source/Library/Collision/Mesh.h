//*************************************************************
//
// Mesh.h
//
// �����蔻��N���X(���b�V��)
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

#pragma once

//=============================================================
// �C���N���[�h
//=============================================================
#include "../System/FileLoader/XFile/XFile.h"

//=============================================================
// ���b�V���f�[�^
//=============================================================
struct Mesh_Data
{
	Vector3 vVertex[3];
	Vector3 vNormal;
	DWORD dwAdjacency[3];
	bool bChecked;
};

//=============================================================
// �N���X��`
//=============================================================
class CMesh
{
public:
	//-------------------------------------------------------------
	// �R���X�g���N�^
	//-------------------------------------------------------------
	CMesh();

	//-------------------------------------------------------------
	// �f�X�g���N�^
	//-------------------------------------------------------------
	~CMesh();

	//-------------------------------------------------------------
	// ������
	// ���@���FCXFile*	�ǂݍ���XFile�̃��b�V��
	//-------------------------------------------------------------
	void Create(CXFile* pMesh);

	//-------------------------------------------------------------
	// �m�F�t���O��������
	//-------------------------------------------------------------
	void ClearCheckFlg();

	//-------------------------------------------------------------
	// ���b�V���擾
	// �߂�l�FCXFile*	�����蔻��N���X�������Ă��郁�b�V���|�C���^
	//-------------------------------------------------------------
	CXFile* GetMesh() { return m_pMesh; };

	//-------------------------------------------------------------
	// ���b�V���f�[�^�擾(����p)
	// �߂�l�FMesh_Data*	�ǂݍ���XFile�̃��b�V���f�[�^
	//-------------------------------------------------------------
	Mesh_Data* GetMeshData() { return m_pMeshData; };

	//-------------------------------------------------------------
	// �|���S�����擾
	// �߂�l�FDWORD	�|���S����
	//-------------------------------------------------------------
	DWORD GetNumFaces() { return m_numFaces; };

private:

	//-------------------------------------------------------------
	// �|���S���̖@���v�Z
	// ���@���FVector3	���_�x�N�g��
	//-------------------------------------------------------------
	Vector3 ComputeNormals(const Vector3 vtx[3]);

	CXFile* m_pMesh;
	Mesh_Data* m_pMeshData;
	DWORD m_numFaces;
};
//*************************************************************
//
// FileBase.cpp
//
// XFile�t�@�C���ǂݍ��ݗp�N���X
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "XFile.h"

//=============================================================
// �N���X��`
//=============================================================

//-------------------------------------------------------------
// �R���X�g���N�^
// ���@���FIDirect3DDevice9*	�f�o�C�X�̃|�C���^
// �@�@�@�@string				�f�B���N�g��
//-------------------------------------------------------------
CXFile::CXFile(IDirect3DDevice9* pDevice, string strFileName)
	: CFileBase(pDevice)
	, m_pIK(nullptr)
	, m_pAllocateHierarchy(nullptr)
	, m_pFrame(nullptr)
	, m_isLoaded(false)
	, m_isIK(false)
{
	D3DXMatrixIdentity(&m_matWorld);

	this->Initialize(strFileName);
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
CXFile::~CXFile()
{
	this->Release();
}

//-------------------------------------------------------------
// ������
// ���@���Fstring	�t�@�C���f�B���N�g��
//-------------------------------------------------------------
void CXFile::Initialize(string path)
{
	//�q�G�����L�[�N���X�̐���
	m_pAllocateHierarchy = new CAllocateHierarchy();

	//�t���[���N���X�̐���
	m_pFrame = new CXFrame();

	auto pFrameRoot = m_pFrame->GetFrame();

	//���f���f�[�^�̓ǂݍ���
	auto hr = D3DXLoadMeshHierarchyFromX((LPCSTR)path.c_str(), D3DXMESH_MANAGED, m_pDevice, m_pAllocateHierarchy, NULL, pFrameRoot, &m_pAnimCtrl);

	//�{�[���}�g���b�N�X�̐ݒ�
	m_pFrame->SetupBoneMatrixPointers(*pFrameRoot);

	if (m_pAnimCtrl)
	{
		m_pIK = new CXInverseKinematics(m_pFrame);
		m_isIK = true;
	}

	//���[�h�ς�
	m_isLoaded = true;
}

//-------------------------------------------------------------
// ���
//-------------------------------------------------------------
void CXFile::Release()
{
	D3DXFrameDestroy(*(m_pFrame->GetFrame()), m_pAllocateHierarchy);
	SafeDelete(m_pAllocateHierarchy);
	SafeDelete(m_pFrame);
	SafeDelete(m_pIK);
	SafeRelease(m_pAnimCtrl);
}

//-------------------------------------------------------------
// �`��
//-------------------------------------------------------------
void CXFile::Draw(Matrix* pWorld, float meshAlpha)
{
	m_pFrame->UpdateFrameMatrices(*(m_pFrame->GetFrame()), pWorld);

	if (m_isIK)
	{
		m_pIK->UpdateIK();
	}

	m_pFrame->DrawFrame(m_pDevice, *(m_pFrame->GetFrame()), pWorld, meshAlpha);
}

//-------------------------------------------------------------
// �t���[��������ϊ��}�g���b�N�X���擾
// ���@���Fstring	�t���[���̖��O
// �@�@�@�@Matrix	�s��̃|�C���^�擾
// ���@�l�F�t���[��������}�g���b�N�X�s����擾���܂�
//-------------------------------------------------------------
void CXFile::GetMatrixFromFrameName(string strFrameName, Matrix * pMatWorld)
{
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)m_pFrame->GetFrameName(strFrameName.c_str());

	*pMatWorld = pFrame->CombinedTransformationMatrix;
}

//-------------------------------------------------------------
// �^�[�Q�b�g�x�N�g��
// ���@���Fconst Vector3	�^�[�Q�b�g�x�N�g��
//-------------------------------------------------------------
void CXFile::SetTargetVector(const UINT index, const Vector3 vecTarget)
{
	if (m_isIK) m_pIK->SetTargetVector(index, vecTarget);
}

//-------------------------------------------------------------
// IK�p�{�[�����ݒ�
// ���@���Fconst string �{�[����1
// �@�@�@�@const string �{�[����2
// �@�@�@�@const string �{�[����3
//-------------------------------------------------------------
void CXFile::SetBoneFrame(string bone1, string bone2, string bone3)
{
	if(m_isIK) m_pIK->SetBoneFrame(bone1, bone2, bone3);
}

//-------------------------------------------------------------
// �{�[���̈ʒu�x�N�g���̎擾
//-------------------------------------------------------------
Vector3 CXFile::GetBonePos(const UINT index) const
{
	if (m_isIK)
	{
		return m_pIK->GetBonePos(index);
	}

	return Vector3(0.0f, 0.0f, 0.0f);
}

//-------------------------------------------------------------
// �A�j���[�V�����R���g���[���[�̎擾
// ���@�l�F�A�j���[�V�����̃R���g���[���[���擾���܂�
//-------------------------------------------------------------
ID3DXAnimationController * CXFile::GetAnimationController()
{
	if (!m_pAnimCtrl)
	{
		return nullptr;
	}

	ID3DXAnimationController* pResult;

	auto maxNumAnimOut = m_pAnimCtrl->GetMaxNumAnimationOutputs();
	auto maxNumAnimSets = m_pAnimCtrl->GetMaxNumAnimationSets();
	auto maxNumTracks = m_pAnimCtrl->GetMaxNumTracks();
	auto maxNumEvents = m_pAnimCtrl->GetMaxNumEvents();

	m_pAnimCtrl->CloneAnimationController(maxNumAnimOut, maxNumAnimSets, maxNumTracks, maxNumEvents, &pResult);

	return pResult;
}

//-------------------------------------------------------------
// ���[�g���b�V���̎擾
//-------------------------------------------------------------
LPD3DXMESH CXFile::GetRootMesh()
{
	return m_pFrame->GetRootMesh();
}

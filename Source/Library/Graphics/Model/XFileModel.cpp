//*************************************************************
//
// Audio.cpp
//
// �I�[�f�B�I�Đ��p�N���X
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "XFileModel.h"

//-------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------
CXFileModel::CXFileModel()
	: m_pXFile(nullptr)
	, m_pAnimationController(nullptr)
	, m_nowAnimationSet(0)
	, m_numAnimationSets(0)
	, m_advanceTime(0.0)
	, m_isLoadedSkinMesh(false)
	, m_meshAlpha(1.0f)
{
	D3DXMatrixIdentity(&m_matWorld);
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
CXFileModel::~CXFileModel()
{
	m_vecAnim.clear();
}

//-------------------------------------------------------------
// ���f���̐ݒ�
//-------------------------------------------------------------
void CXFileModel::SetModel(CXFile * pModel)
{
	//�X�L�����b�V���̐ݒ�
	m_pXFile = pModel;

	//�R���g���[���[�̎擾
	m_pAnimationController = m_pXFile->GetAnimationController();

	//�A�j���[�V�����R���g���[���[�̐ݒ�
	if (m_pAnimationController) 
	{
		m_vecAnim.clear();

		m_numAnimationSets = m_pAnimationController->GetNumAnimationSets();

		for (UINT i = 0; i < m_numAnimationSets; i++) {
			m_vecAnim.push_back(make_unique<AnimDesc>());

			m_pAnimationController->GetAnimationSet(i, &(m_vecAnim[i]->pAnimSet));
			m_vecAnim[i]->trackSpeed = static_cast<FLOAT>(m_vecAnim[i]->pAnimSet->GetPeriod());
			m_vecAnim[i]->shiftTime = 0.1f;
		}
	}

	//���[�h�ς�
	m_isLoadedSkinMesh = true;
}

//-------------------------------------------------------------
// ���E�s��̐ݒ�
//-------------------------------------------------------------
void CXFileModel::SetTransform(Matrix * pMatWorld)
{
	m_matWorld = *pMatWorld;
}

//-------------------------------------------------------------
// �`��
// ���@���Fbool	������J�����O�p
// �@�@�@�@bool	�A�j���[�V�����i�s
//-------------------------------------------------------------
void CXFileModel::Draw(CSystem* pSystem, DWORD value, bool bVisible, bool bAnimation)
{
	if (bAnimation)
	{
		this->AdvanceTime();
	}

	if(bVisible)
	{
		pSystem->SetShaderState(value);
		m_pXFile->Draw(&m_matWorld, m_meshAlpha);
	}
}

//-------------------------------------------------------------
// �t���[��������ϊ��}�g���b�N�X���擾
// ���@���Fstd::string  �t���[����
// �@�@�@�@LPD3DXMATRIX �ϊ��}�g���b�N�X�̃|�C���^
//-------------------------------------------------------------
void CXFileModel::GetMatrixFromFrameName(string strFrameName, Matrix * pMatWorld)
{
	m_pXFile->GetMatrixFromFrameName(strFrameName, pMatWorld);
}

//-------------------------------------------------------------
// �A�j���[�V�����̎��Ԑݒ�
// ���@���Fconst double	�P�t���[���Ԃ̎���
//-------------------------------------------------------------
void CXFileModel::SetAdvanceTime(const double deltaTime)
{
	m_advanceTime = deltaTime;
}

//-------------------------------------------------------------
// �A�j���[�V�����̕ύX
// ���@���Fconst UINT	�A�j���[�V�����̔ԍ�
//-------------------------------------------------------------
void CXFileModel::SetChangeAnimation(const UINT animID)
{
	//�A�j���[�V�����̍ő吔�������������H
	if (m_vecAnim.size() <= animID) 
	{
		return;
	}

	//���݂̃A�j���[�V�����Ɠ������ǂ���
	if (m_nowAnimationSet == animID) 
	{
		return;
	}

	//���݂̃A�j���[�V�����Z�b�g�̐ݒ�l���擾
	D3DXTRACK_DESC TrackDesc;
	m_pAnimationController->GetTrackDesc(0, &TrackDesc);

	//���̃A�j���[�V�������g���b�N1�Ɉړ�
	m_pAnimationController->SetTrackAnimationSet(1, m_vecAnim[m_nowAnimationSet]->pAnimSet);
	m_pAnimationController->SetTrackDesc(1, &TrackDesc);

	//�V�����A�j���[�V�����Z�b�g���g���b�N0�ɐݒ�
	m_pAnimationController->SetTrackAnimationSet(0, m_vecAnim[animID]->pAnimSet);

	//�g���b�N�X�s�[�h�̐ݒ�
	m_pAnimationController->SetTrackSpeed(0, m_vecAnim[animID]->trackSpeed);

	//�g���b�N�̍���������
	m_pAnimationController->SetTrackEnable(0, true);
	m_pAnimationController->SetTrackEnable(1, true);

	//�g���b�N�̈ʒu���ŏ��ɂ��ǂ�
	m_pAnimationController->SetTrackPosition(0, 0);

	//�E�F�C�g���Ԃ�������
	m_vecAnim[animID]->curWeightTime = 0.0f;

	//���݂̃A�j���[�V�����ԍ���؂�ւ�
	m_nowAnimationSet = animID;
}

//-------------------------------------------------------------
// �A�j���[�V�������[�v���Ԃ̐ݒ�
// ���@���Fconst UINT	�A�j���[�V�����̔ԍ�
// �@�@�@�@const float	ms
//-------------------------------------------------------------
void CXFileModel::SetLoopTime(const UINT animID, const float time)
{
	if (m_vecAnim.size() <= animID) 
	{
		return;
	}

	//�g���b�N�X�s�[�h�Z�o
	FLOAT defTime = static_cast<FLOAT>(m_vecAnim[animID]->pAnimSet->GetPeriod());
	m_vecAnim[animID]->loopTime = time;
	m_vecAnim[animID]->trackSpeed = defTime / time;
}

//-------------------------------------------------------------
// �A�j���[�V������Ԏ��Ԃ̐ݒ�
// ���@���Fconst UINT	�A�j���[�V�����̔ԍ�
// �@�@�@�@const float	�Ԋu
//-------------------------------------------------------------
void CXFileModel::SetShiftTime(const UINT animID, const float interval)
{
	if (m_vecAnim.size() <= animID) 
	{
		return;
	}

	m_vecAnim[animID]->shiftTime = interval;
}

//-------------------------------------------------------------
// �^�[�Q�b�g�x�N�g��
// ���@���Fconst Vector3	�^�[�Q�b�g�x�N�g��
//-------------------------------------------------------------
void CXFileModel::SetTargetVector(const UINT index, const Vector3 vecTarget)
{
	m_pXFile->SetTargetVector(index, vecTarget);
}

//-------------------------------------------------------------
// �{�[���t���[���̐ݒ�
// ���@���Fconst string �{�[����1
// �@�@�@�@const string �{�[����2
// �@�@�@�@const string �{�[����3
//-------------------------------------------------------------
void CXFileModel::SetBoneFrame(string bone1, string bone2, string bone3)
{
	m_pXFile->SetBoneFrame(bone1, bone2, bone3);
}

//-------------------------------------------------------------
// ���b�V���̓����x��ݒ肵�܂�
// ���@���Ffloat �����x(1.0f ~ 0.0f) 
//-------------------------------------------------------------
void CXFileModel::SetMeshAlpha(const float value)
{
	m_meshAlpha = value;
}

//-------------------------------------------------------------
// ���f�������A�j���[�V�����o�ߎ��Ԃ̎擾
// �߂�l�Fdouble	�^�C���̎擾
//-------------------------------------------------------------
double CXFileModel::GetTime() const
{
	return m_pAnimationController->GetTime();
}

//-------------------------------------------------------------
// �{�[���̈ʒu�x�N�g���̎擾
//-------------------------------------------------------------
Vector3 CXFileModel::GetBonePos(const UINT index) const
{
	return m_pXFile->GetBonePos(index);
}

//*************************************************************
//
// private
//
//*************************************************************

//-------------------------------------------------------------
// �A�j���[�V�����̍Đ�
//-------------------------------------------------------------
void CXFileModel::AdvanceTime()
{
	if (m_pAnimationController)
	{
		m_vecAnim[m_nowAnimationSet]->curWeightTime += static_cast <float>(m_advanceTime);

		if (m_vecAnim[m_nowAnimationSet]->curWeightTime <= m_vecAnim[m_nowAnimationSet]->shiftTime) {

			FLOAT Weight = m_vecAnim[m_nowAnimationSet]->curWeightTime / m_vecAnim[m_nowAnimationSet]->shiftTime;
			m_pAnimationController->SetTrackWeight(0, Weight);
			m_pAnimationController->SetTrackWeight(1, 1 - Weight);
		}
		else {
			m_pAnimationController->SetTrackWeight(0, 1.0f);
			m_pAnimationController->SetTrackEnable(1, false);
		}

		//���Ԃ̍X�V
		m_pAnimationController->AdvanceTime(m_advanceTime, nullptr);
	}
}

//********************************************************************************
//
// Sword.hpp
//
// ���̃N���X
//
// Date: 2019/07/30 �X�V
//
//********************************************************************************

#pragma once

//================================================================================
// �C���N���[�h
//================================================================================
#include "../../../GameManager/GameManager.h"

//================================================================================
// �N���X��`
//================================================================================
class CSword : public CGameObject
{
public:

	//--------------------------------------------------------------------------------
	// �R���X�g���N�^
	//--------------------------------------------------------------------------------
	CSword();

	//--------------------------------------------------------------------------------
	// �f�X�g���N�^
	//--------------------------------------------------------------------------------
	~CSword();

	//--------------------------------------------------------------------------------
	// ������
	// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
	//--------------------------------------------------------------------------------
	void Initialize(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// �`��
	// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
	//--------------------------------------------------------------------------------
	void ZDraw(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// �`��
	// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
	//--------------------------------------------------------------------------------
	void Draw(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// ���E�s��̐ݒ�
	// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
	//--------------------------------------------------------------------------------
	void SetTransform(Matrix* pMatWorld);

	//--------------------------------------------------------------------------------
	// �U���p�����蔻��
	// �߂�l�FCBoundingBox*	�o�E���f�B���O�{�b�N�X
	//--------------------------------------------------------------------------------
	CBoundingBox* GetAttackBox() const { return m_pAttackBox; };

private:
	CBoundingBox* m_pAttackBox;
	CXFileModel* m_pModel;
};
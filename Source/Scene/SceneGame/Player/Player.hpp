//********************************************************************************
//
// Player.hpp
//
// �v���C���[�N���X
//
// Date: 2019/07/30 �X�V
//
//********************************************************************************

#pragma once

//================================================================================
// �C���N���[�h
//================================================================================
#include "../../GameManager/GameManager.h"
#include "Behavior/PlayerAnimation.hpp"
#include "Sword/Sword.hpp"

//================================================================================
// �N���X��`
//================================================================================
class CPlayer : public CGameObject
{
public:

	//--------------------------------------------------------------------------------
	// �R���X�g���N�^
	//--------------------------------------------------------------------------------
	CPlayer();

	//--------------------------------------------------------------------------------
	// �f�X�g���N�^
	//--------------------------------------------------------------------------------
	~CPlayer();

	//--------------------------------------------------------------------------------
	// �t�@�C���ǂݍ���
	// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
	//--------------------------------------------------------------------------------
	void LoadFile(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// ������
	// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
	//--------------------------------------------------------------------------------
	void Initialize(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// �X�V
	// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
	// �@�@�@�@const Vector3	�J�����A���O����3�����x�N�g��
	//--------------------------------------------------------------------------------
	void Update(CSystem* pSystem, const Vector3 vecAngle);

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
	// �G����U�����󂯂��Ƃ��̏���
	// ���@���FCSystem*		�V�X�e���N���X�̃|�C���^
	// �@�@�@�@const float	�U����
	//--------------------------------------------------------------------------------
	void SetHitEnemyAttack(CSystem* pSystem, const float atk);

	//--------------------------------------------------------------------------------
	// ���C�̎擾
	// �߂�l�FCRay*	���C�̎擾
	//--------------------------------------------------------------------------------
	CRay* GetRay() const { return m_pRay; };

	//--------------------------------------------------------------------------------
	// ���C�̎擾
	// �߂�l�FCRay*	���C�̎擾
	//--------------------------------------------------------------------------------
	CRay* GetFootLeft() const { return m_pFootLeft; };

	//--------------------------------------------------------------------------------
	// ���C�̎擾
	// �߂�l�FCRay*	���C�̎擾
	//--------------------------------------------------------------------------------
	CRay* GetFootRight() const { return m_pFootRight; };

	//--------------------------------------------------------------------------------
	// �̂̓����蔻��
	// �߂�l�FCBoundingBox*	�o�E���f�B���O�{�b�N�X
	//--------------------------------------------------------------------------------
	CBoundingBox* GetBodyBox() const { return m_pBodyBox; };

	//--------------------------------------------------------------------------------
	// �U���p�����蔻��
	// �߂�l�FCBoundingBox*	�o�E���f�B���O�{�b�N�X
	//--------------------------------------------------------------------------------
	CBoundingBox* GetAttackBox() const { return m_pSword->GetAttackBox(); };

	//--------------------------------------------------------------------------------
	// ���͂��Ă�������̎擾
	//--------------------------------------------------------------------------------
	Vector3 GetDir() const { return m_vecDirection; };

	//--------------------------------------------------------------------------------
	// �U���͂̎擾
	// �߂�l�Fconst float		�U����
	//--------------------------------------------------------------------------------
	float GetAtk() const { return m_atk; };

	//--------------------------------------------------------------------------------
	// �̗͂̎擾
	// �߂�l�Fconst float		�̗�
	//--------------------------------------------------------------------------------
	float GetHp() const { return m_hp; };

	//--------------------------------------------------------------------------------
	// �X�^�~�i�̎擾
	// �߂�l�Fconst float		�X�^�~�i
	//--------------------------------------------------------------------------------
	float GetStamina() const { return m_stamina; };

	//--------------------------------------------------------------------------------
	// �v���C���[�U���A�j���[�V������
	// �߂�l�Fconst bool	�A�j���[�V������
	//--------------------------------------------------------------------------------
	bool IsNowAttack() const { return m_pAnimation->IsNowAttack(); };

	//--------------------------------------------------------------------------------
	// �_�b�V����
	//--------------------------------------------------------------------------------
	bool IsDash() const { return m_isDash; };

	//--------------------------------------------------------------------------------
	// ���b�N�I��
	//--------------------------------------------------------------------------------
	bool IsLockOn() const { return m_isLockOn; };

	//--------------------------------------------------------------------------------
	// �G�l�~�[����U���󂯂���I
	//--------------------------------------------------------------------------------
	bool IsEnemyHit() const { return m_isEnemyHit; };

	//--------------------------------------------------------------------------------
	// �G�l�~�[����U���󂯂���������Z�b�g
	//--------------------------------------------------------------------------------
	void EnemyHitReset();

private:

	//--------------------------------------------------------------------------------
	// �J�[�\���ʒu�����Œ�
	//--------------------------------------------------------------------------------
	void CenterCursorPos();

	//--------------------------------------------------------------------------------
	// ���S�󋵂̍X�V
	//--------------------------------------------------------------------------------
	void UpdateDeath();

	//--------------------------------------------------------------------------------
	// �ʒu�̍X�V
	//--------------------------------------------------------------------------------
	void UpdatePosition();

	//--------------------------------------------------------------------------------
	// ����IK�ʒu�ݒ�
	//--------------------------------------------------------------------------------
	void UpdateFootIK();

	//--------------------------------------------------------------------------------
	// �v���C���[�̍s��ݒ�
	//--------------------------------------------------------------------------------
	void SetPlayerMatrix();

	//--------------------------------------------------------------------------------
	// ���̍s��ݒ�
	//--------------------------------------------------------------------------------
	void SetSwordMatrix();

	//--------------------------------------------------------------------------------
	// ���͏���
	// ���@���FCSystem*		�V�X�e���N���X�̃|�C���^
	//--------------------------------------------------------------------------------
	void UpdateKeyInput(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// �v���C���[�̑���
	// ���@���FCSystem*			�V�X�e���N���X�̃|�C���^
	// �@�@�@�@const Vector3	��]�A���O��
	//--------------------------------------------------------------------------------
	void UpdateController(CSystem * pSystem, const Vector3  vecAngle);

	//--------------------------------------------------------------------------------
	// ���f����]
	// ���@���Fconst Vector3&	�R���������x�N�g��
	// �@�@�@�@const float			�J�����̃A���O��
	//--------------------------------------------------------------------------------
	void ModelRotate(const Vector3& vecDir, const float camAngle);

	//--------------------------------------------------------------------------------
	// �X�^�~�i�̍X�V
	//--------------------------------------------------------------------------------
	void UpdateStamina();

	//--------------------------------------------------------------------------------
	// �p�����[�^�̎擾
	// ���@���Fconst string �t�@�C����
	//--------------------------------------------------------------------------------
	void LoadParams(const string strFileName);

	//--------------------------------------------------------------------------------
	// �p�����[�^�̕ۑ�
	// ���@���Fconst string �t�@�C����
	//--------------------------------------------------------------------------------
	void InitParams(const string strFileName);

	enum footIK
	{
		RIGHT_FOOT,
		LEFT_FOOT,
	};

	CXFileModel* m_pModel;
	CBoundingBox* m_pBodyBox;
	CPlayerAnimation* m_pAnimation;
	CSword* m_pSword;
	CRay* m_pRay;
	CRay* m_pFootLeft;
	CRay* m_pFootRight;

	Vector3 m_vecInputDirection;
	Vector3 m_vecDirection;
	Vector3 m_vecMovement;
	
	float m_hp;
	float m_atk;
	float m_stamina;
	float m_moveSpeed;
	float m_moveSpeedDelay;
	float m_padSpeed;

	float m_dashSpeed;
	float m_walkSpeed;
	float m_delaySpeed;

	bool m_isLockOn;
	bool m_isDashKey;
	bool m_isDash;
	bool m_isDeath;
	bool m_isAttack;
	bool m_isEnemyHit;
};
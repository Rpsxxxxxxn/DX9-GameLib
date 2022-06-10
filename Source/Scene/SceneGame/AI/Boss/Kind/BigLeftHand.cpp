//********************************************************************************
//
// BigLeftHand.cpp
//
// ���肪�ł����{�X�N���X
//
// Date: 2019/07/30 �X�V
//
//********************************************************************************

//================================================================================
// �C���N���[�h
//================================================================================
#include "BigLeftHand.hpp"

using namespace FileConst;

//================================================================================
// �萔��`
//================================================================================
const Vector3 FORWARD(0.0f, 0.0f, -1.0f);
const float ANIMATION_END = 0.9f;

//================================================================================
// �N���X��`
//================================================================================

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
CBigLeftHand::CBigLeftHand()
	: m_isAttackDist(false)
{
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
CBigLeftHand::~CBigLeftHand()
{
}

//--------------------------------------------------------------------------------
// �t�@�C���ǂݍ���
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CBigLeftHand::LoadFile(CSystem * pSystem)
{
	CBossBase::LoadParams("BigLeftHand");
}

//--------------------------------------------------------------------------------
// ������
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CBigLeftHand::Initialize(CSystem * pSystem)
{
	CBossBase::SetModel(pSystem, MODEL_DEMOBOSS);

	m_timer.SetTimer(m_attackInterval);

	m_pBodyBox->Create(
		pSystem,
		&Vector3(0.5f, 2.0f, 0.5f),
		&Vector3(-0.5f, 0.0f, -0.5f),
		false);

	m_pAttackBox->Create(
		pSystem, 
		&Vector3(0.25f, 0.25, 0.25),
		&Vector3(-0.25f, -0.25, -0.25),
		false);
}

//--------------------------------------------------------------------------------
// �X�V
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
// �@�@�@�@CPlayer*	�v���C���[�N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CBigLeftHand::Update(CSystem * pSystem, CPlayer* pPlayer)
{
	UpdateDiscovery(pPlayer);

	Controller(pSystem, pPlayer);

	UpdateForward(FORWARD);

	NowAttack();

	Death(pSystem);

	UpdateAnimation();

	CBossBase::Update(pSystem, pPlayer);

	CBossBase::SetAttackBox("Right_hand");
}

//--------------------------------------------------------------------------------
// �G�̑���
// ���@���FCPlayer*	�v���C���[�N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CBigLeftHand::Controller(CSystem* pSystem, CPlayer * pPlayer)
{
	if (m_isDeath) return;

	//����
	if (m_isDiscovery)
	{
		AttackDistance();
		Rotate(pPlayer->GetPosition());

		if (!m_isAttackDist)
		{
			Move(2.0f);
		}
		else {
			RandomAttack(pSystem);
			StartAttack();
		}
	}
	else {
		Rotate(Vector3(0.0f, 0.0f, 0.0f));
		Move(2.0f);
	}
}

//--------------------------------------------------------------------------------
// �v���C���[���͈͓��ɓ����Ă��邩����
// ���@���FCPlayer*	�v���C���[�N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CBigLeftHand::UpdateDiscovery(CPlayer * pPlayer)
{
	//2�_�����̌v�Z
	Vector3 toPos = pPlayer->GetPosition() - m_position;

	//�����v�Z
	m_distance = VectorCalculate::Magnitude(toPos);

	//�������͈͂ɓ���Δ����B
	if (m_distance < 50.0f)
	{
		m_isDiscovery = true;
	}
	else {
		m_isDiscovery = false;
	}
}

//--------------------------------------------------------------------------------
// ����
// ���@���Fconst float	�ߋ������E
//--------------------------------------------------------------------------------
void CBigLeftHand::Move(const float distance)
{
	if (m_isNowAttack) return;

	if (m_distance >= distance)
	{
		m_position += m_vecForward * m_walkSpeed;

		m_nowAnimation = WALK;
	}
}

//--------------------------------------------------------------------------------
// ��]
// ���@���FVector3&	�^�[�Q�b�g�ƂȂ�ʒu�x�N�g��
//--------------------------------------------------------------------------------
void CBigLeftHand::Rotate(Vector3& vecPos)
{
	if (m_nowAnimation == ATTACK1 ||
		m_nowAnimation == ATTACK2 ||
		m_nowAnimation == ATTACK3) return;

	auto toPos = vecPos - m_position;
	auto direction = VectorCalculate::Normalize(toPos);
	auto dot = VectorCalculate::Dot(m_vecForward, direction);
	auto cross = VectorCalculate::Cross(m_vecForward, direction);

	if (dot > 0.3f)
	{
		if (dot < 0.98)
		{
			ModelRotate(cross);
		}
	}
	else {
		ModelRotate(cross);
		m_position -= m_vecForward * m_walkSpeed;
		m_nowAnimation = WALK;
	}

}

//--------------------------------------------------------------------------------
// �A�j���[�V�����X�V
//--------------------------------------------------------------------------------
void CBigLeftHand::ModelRotate(const Vector3& pCross)
{
	if (pCross.y < D3DXToRadian(-2.0f))
	{
		m_rotation.y -= D3DXToRadian(2.0f); //���։�
	}

	if (pCross.y > D3DXToRadian(2.0f))
	{
		m_rotation.y += D3DXToRadian(2.0f); //�E�։�
	}
}

//--------------------------------------------------------------------------------
// �U������
//--------------------------------------------------------------------------------
void CBigLeftHand::AttackDistance()
{
	if (m_distance < 2.5f)
	{
		m_isAttackDist = true;
	}
	else {
		m_isAttackDist = false;
	}

	if (m_nowAnimation == ATTACK1 ||
		m_nowAnimation == ATTACK2 ||
		m_nowAnimation == ATTACK3) return;

	if (m_isAttackDist)
	{
		m_nowAnimation = IDLE;
	}
}

//--------------------------------------------------------------------------------
// �����_���U��
//--------------------------------------------------------------------------------
void CBigLeftHand::RandomAttack(CSystem* pSystem)
{
	if (m_timer.IsTimeOut(pSystem->GetDeltaTime()))
	{
		int random = Calculate::Random(0, 2);

		switch (random)
		{
		case 0: m_nowAnimation = ATTACK1; break;
		case 1: m_nowAnimation = ATTACK2; break;
		case 2: m_nowAnimation = ATTACK3; break;
		}
	}
}

//--------------------------------------------------------------------------------
// �U�����J�n����
//--------------------------------------------------------------------------------
void CBigLeftHand::StartAttack()
{
	if (m_isNowAttack) return;

	if (m_nowAnimation == ATTACK1 ||
		m_nowAnimation == ATTACK2 ||
		m_nowAnimation == ATTACK3)
	{
		m_isNowAttack = true;
		m_attackStartTime = m_pModel->GetTime();
	}
}

//--------------------------------------------------------------------------------
// �U�����̏���
//--------------------------------------------------------------------------------
void CBigLeftHand::NowAttack()
{
	if (!m_isNowAttack) return;

	double nowTime = m_pModel->GetTime();
	double tookTime = nowTime - m_attackStartTime;

	if (tookTime >= 0.15 && tookTime < 0.65)
	{
		//�v���C���[�Ɉ�x���q�b�g���Ă��Ȃ��Ƃ�
		if (!m_isHitPlayer)
		{
			m_isAttackFlg = true;
		}
		else {
			//�v���C���[�q�b�g�����Ƃ��B
			//���̏����͈�x�̂ݍU����^���邽�߂̏���
			m_isAttackFlg = false;
		}
	}
	else if (tookTime >= 0.65)
	{
		m_nowAnimation = IDLE;
		m_isHitPlayer = false;
		m_isAttackFlg = false;
		m_isNowAttack = false;
	}
}

//--------------------------------------------------------------------------------
// ���S����
//--------------------------------------------------------------------------------
void CBigLeftHand::Death(CSystem* pSystem)
{
	if (!m_isDeath) return;

	m_isAttackFlg = false;
	m_nowAnimation = DEATH;

	double nowTime = m_pModel->GetTime();
	double tookTime = nowTime - m_deathStartTime;

	if (tookTime > ANIMATION_END)
	{
		m_isAlive = false;
		m_pModel->SetAdvanceTime(0);
	}
}

//--------------------------------------------------------------------------------
// �A�j���[�V�����X�V
//--------------------------------------------------------------------------------
void CBigLeftHand::UpdateAnimation()
{
	m_pModel->SetChangeAnimation(m_nowAnimation);
}
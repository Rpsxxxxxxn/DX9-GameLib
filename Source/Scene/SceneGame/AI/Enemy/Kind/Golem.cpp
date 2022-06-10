//********************************************************************************
//
// Golem.cpp
//
// �S�[����
//
// Date: 2019/07/30 �X�V
//
//********************************************************************************

//================================================================================
// �C���N���[�h
//================================================================================
#include "Golem.hpp"

using namespace FileConst;

//================================================================================
// �萔��`
//================================================================================
const Vector3 FORWARD(0.0f, 0.0f, 1.0f);
const float ANIMATION_END = 0.9f;

//================================================================================
// �N���X��`
//================================================================================

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
CGolem::CGolem()
	: m_isAttackDist(false)
	, m_isDeathAlpha(false)
	, m_speed(0.0f)
{
	auto randomSize = Calculate::Random(1.5f, 2.0f);
	m_scaling = Vector3(randomSize, randomSize, randomSize);

	CEnemyBase::LoadParams("Golem");
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
CGolem::~CGolem()
{
}

//--------------------------------------------------------------------------------
// ������
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGolem::Initialize(CSystem * pSystem)
{
	m_pModel->SetModel(pSystem->GetXFile(MODEL_ENEMY));
	m_pModel->SetAdvanceTime(1.0 / 60.0);
	m_timer.SetTimer(m_attackInterval);

	CreateBodyBox(
		pSystem,
		Vector3(0.5f, 1.0f, 0.5f), 
		Vector3(-0.5f, 0.0f, -0.5f));

	CreateAttackBox(
		pSystem,
		Vector3(0.3f, 1.0f, 1.0f), 
		Vector3(-0.3f, -0.0f, 0.0f));
}

//--------------------------------------------------------------------------------
// �X�V
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGolem::Update(CSystem * pSystem, CPlayer* pPlayer)
{
	if (m_isSleep == false)
	{
		UpdateForward(FORWARD);
		UpdateDiscovery(pPlayer);
		UpdateMoveSpeed();
		UpdateController(pSystem, pPlayer);
		UpdateDeath(pSystem);
		UpdateMeshAlpha();
		NowAttack();
		CEnemyBase::Update(pSystem, pPlayer);
	}

	UpdateAnimation();
}

//--------------------------------------------------------------------------------
// �G�̑���
//--------------------------------------------------------------------------------
void CGolem::UpdateController(CSystem* pSystem, CPlayer* pPlayer)
{
	if (m_isDeath) return;

	if (m_isDiscovery)
	{
		AttackDistance();
		UpdateRotate(pPlayer->GetPosition());

		if (!m_isAttackDist)
		{
			UpdateMove(1.5f);
		}
		else {
			RandomAttack(pSystem);
			StartAttack();
		}
	}
	else {
		UpdateFreeWalk();
	}
}

//--------------------------------------------------------------------------------
// �G�̑���
//--------------------------------------------------------------------------------
void CGolem::UpdateDiscovery(CPlayer * pPlayer)
{
	//2�_�����̌v�Z
	Vector3 toPos = pPlayer->GetPosition() - m_position;

	//�����v�Z
	m_distance = VectorCalculate::Magnitude(toPos);

	//�������͈͂ɓ���Δ����B
	if (m_distance < 5.0f)
	{
		m_isDiscovery = true;
	}
	else {
		m_isDiscovery = false;
	}
}

//--------------------------------------------------------------------------------
// ���R�ɕ���
//--------------------------------------------------------------------------------
void CGolem::UpdateFreeWalk()
{
	auto distance = m_vecBasePos - m_position;
	auto length = VectorCalculate::Magnitude(distance);

	if (length < 5.0f)
	{
		m_position += m_vecForward * m_speed;
		m_nowAnimation = WALK;
	}
	else {
		// �͈͊O
		auto direction = VectorCalculate::Normalize(distance);
		auto dot = VectorCalculate::Dot(m_vecForward, direction);
		auto cross = VectorCalculate::Cross(m_vecForward, direction);

		if (dot < 0.98)
		{
			auto rotate = Mathf::ToRadian(2.0f);

			if (cross.y < -rotate)
			{ 
				m_rotation.y -= rotate;
			}

			if (cross.y > rotate)
			{ 
				m_rotation.y += rotate;
			}
		}

		m_position += m_vecForward * m_speed;
	}
}

//--------------------------------------------------------------------------------
// ����
// ���@���Fconst float	�ߋ������E
//--------------------------------------------------------------------------------
void CGolem::UpdateMove(const float distance)
{
	if (m_distance >= distance)
	{
		m_position += m_vecForward * m_speed;

		m_nowAnimation = WALK;
	}
}

//--------------------------------------------------------------------------------
// ��]
// ���@���FVector3&	�^�[�Q�b�g�ƂȂ�ʒu�x�N�g��
//--------------------------------------------------------------------------------
void CGolem::UpdateRotate(Vector3& vecPos)
{
	if (m_nowAnimation == ATTACK1 || m_nowAnimation == ATTACK2) return;

	auto distance = vecPos - m_position;

	auto direction = VectorCalculate::Normalize(distance);

	auto dot = VectorCalculate::Dot(m_vecForward, direction);

	auto vecCross = VectorCalculate::Cross(m_vecForward, direction);

	if (dot < 0.98)
	{
		ModelRotate(vecCross);
	}
}

//--------------------------------------------------------------------------------
// ����̃��b�V������
//--------------------------------------------------------------------------------
void CGolem::UpdateMeshAlpha()
{
	if (!m_isDeathAlpha) return;

	m_meshAlpha -= 0.01f;
	if (m_meshAlpha <= 0.2f)
	{
		m_isAlive = false;
	}
}

//--------------------------------------------------------------------------------
// ���쑬�x�̍X�V
//--------------------------------------------------------------------------------
void CGolem::UpdateMoveSpeed()
{
	if (m_isDiscovery)
	{
		m_speed = m_dashSpeed;
	}
	else {
		m_speed = m_walkSpeed;
	}
}

//--------------------------------------------------------------------------------
// �A�j���[�V�����X�V
//--------------------------------------------------------------------------------
void CGolem::ModelRotate(const Vector3& pCross)
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
void CGolem::AttackDistance()
{
	if (m_distance < 1.5f)
	{
		m_isAttackDist = true;
	}
	else {
		m_isAttackDist = false;
	}

	if (m_nowAnimation == ATTACK1 || m_nowAnimation == ATTACK2) return;

	if (m_isAttackDist)
	{
		m_nowAnimation = IDLE;
	}
}

//--------------------------------------------------------------------------------
// �����_���U��
//--------------------------------------------------------------------------------
void CGolem::RandomAttack(CSystem* pSystem)
{
	if (m_timer.IsTimeOut(pSystem->GetDeltaTime()))
	{
		int random = Calculate::Random(0, 1);

		switch (random)
		{
		case 0: m_nowAnimation = ATTACK1; break;
		case 1: m_nowAnimation = ATTACK2; break;
		}
	}
}

//--------------------------------------------------------------------------------
// �U�����J�n����
//--------------------------------------------------------------------------------
void CGolem::StartAttack()
{
	if (m_isNowAttack) return;

	if (m_nowAnimation == ATTACK1 || m_nowAnimation == ATTACK2)
	{
		m_isNowAttack = true;
		m_attackStartTime = m_pModel->GetTime();
	}
}

//--------------------------------------------------------------------------------
// �U������
//--------------------------------------------------------------------------------
void CGolem::NowAttack()
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
void CGolem::UpdateDeath(CSystem* pSystem)
{
	if (!m_isDeath) return;

	m_isAttackFlg = false;
	m_nowAnimation = DEATH;

	double nowTime = m_pModel->GetTime();
	double tookTime = nowTime - m_deathStartTime;

	if (tookTime > ANIMATION_END && !m_isDeathAlpha)
	{
		m_isDeathAlpha = true;
		m_pModel->SetAdvanceTime(0);
		pSystem->PlayEffect(3, m_position);
	}
}

//--------------------------------------------------------------------------------
// �A�j���[�V�����X�V
//--------------------------------------------------------------------------------
void CGolem::UpdateAnimation()
{
	m_pModel->SetChangeAnimation(m_nowAnimation);
}

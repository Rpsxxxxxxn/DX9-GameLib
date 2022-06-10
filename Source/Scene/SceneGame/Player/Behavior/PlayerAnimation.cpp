//********************************************************************************
//
// PlayerAnimation.hpp
//
// �v���C���[�A�j���[�V�����̃N���X
//
// Date: 2019/10/01 �X�V
//
//********************************************************************************

//================================================================================
// �C���N���[�h
//================================================================================
#include "PlayerAnimation.hpp"

using namespace PlayerAnimation;

const double SLEEP_TIME = 0;
const double START_ANIMATION_TIME = 0.15;
const double END_ANIMATION_TIME = 0.75;

//================================================================================
// �N���X��`
//================================================================================

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
CPlayerAnimation::CPlayerAnimation(CXFileModel* pModel)
	: m_nowAction(PlayerAnimation::Action::IDLE)
	, m_newAction(m_nowAction)
	, m_nowAni(PlayerAnimation::Animation::IDLE)
	, m_pModel(pModel)
	, m_attackStartTime(0.0)
	, m_guardStartTime(0.0)
	, m_deathStartTime(0.0)
	, m_nowAttackCnt(0)
	, m_isLockAni(false)
	, m_isDeathLock(false)
	, m_enableAttack(false)
	, m_enableGuard(false)
	, m_enableDeath(false)
	, m_isNowAttack(false)
	, m_isDeath(false)
{
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
CPlayerAnimation::~CPlayerAnimation()
{
	if(m_pModel) m_pModel = nullptr;
}

//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void CPlayerAnimation::Update()
{
	//�A�j���[�V�����ύX�̍X�V
	UpdateChangeAni();

	//���S�ȊO
	if (!m_isDeathLock)
	{
		//�A�j���[�V���������̕ύX
		switch (m_nowAction)
		{
		case ::Action::IDLE: m_nowAni = ::Animation::IDLE; break;
		case ::Action::WALK: m_nowAni = ::Animation::WALK; break;
		case ::Action::DASH: m_nowAni = ::Animation::DASH; break;
		case ::Action::ATTACK: StartAttack(); break;
		case ::Action::GUARD: StartGuard(); break;
		}
	}

	//�A�j���[�V�����̍X�V
	UpdateAni();
}

//--------------------------------------------------------------------------------
// ���S�����ݏ���
//--------------------------------------------------------------------------------
void CPlayerAnimation::SetDeath()
{
	m_isLockAni = true;
	m_isDeathLock = true;
	m_nowAction = m_newAction = PlayerAnimation::Action:: DEATH;
	m_nowAni = PlayerAnimation::Animation::DEATH;
	m_deathStartTime = m_pModel->GetTime();
}

//--------------------------------------------------------------------------------
// ���݂̃A�j���[�V�����ύX
// ���@���FPlayerAnimation::Action	�A�N�V�����񋓑�
//--------------------------------------------------------------------------------
void CPlayerAnimation::SetChangeAni(PlayerAnimation::Action action)
{
	if (m_nowAction == m_newAction && m_isLockAni) return;
	
	m_newAction = action;
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// �A�j���[�V�����ύX�̍X�V
//--------------------------------------------------------------------------------
void CPlayerAnimation::UpdateChangeAni()
{
	if (m_nowAction == m_newAction && m_isLockAni) return;

	m_nowAction = m_newAction;
}

//--------------------------------------------------------------------------------
// �A�j���[�V�����̍X�V
//--------------------------------------------------------------------------------
void CPlayerAnimation::UpdateAni()
{
	NowDeath();

	NowAttack();

	NowGuard();

	m_pModel->SetChangeAnimation(m_nowAni);
}

//--------------------------------------------------------------------------------
// �U������
//--------------------------------------------------------------------------------
void CPlayerAnimation::StartAttack()
{
	if ( m_isLockAni ) return;

	//���b�N��������
	m_isLockAni = true;

	// �U�������̕ύX
	switch ( m_nowAttackCnt )
	{
		case 0: m_nowAni = PlayerAnimation::Animation::ATTACK1; break;
		case 1: m_nowAni = PlayerAnimation::Animation::ATTACK2; break;
		case 2: m_nowAni = PlayerAnimation::Animation::ATTACK3; break;
		case 3: m_nowAni = PlayerAnimation::Animation::ATTACK4; break;
		case 4: m_nowAni = PlayerAnimation::Animation::ATTACK5; break;
	}

	m_attackStartTime = m_pModel->GetTime(); //�X�^�[�g���Ԃ̎擾
	m_nowAttackCnt ++; //�U���̕ύX
	m_nowAttackCnt %= 5;
}

//--------------------------------------------------------------------------------
// �K�[�h����
//--------------------------------------------------------------------------------
void CPlayerAnimation::StartGuard()
{
	if (m_isLockAni) return;

	//���b�N��������
	m_isLockAni = true;
}

//--------------------------------------------------------------------------------
// �U�����̏���
//--------------------------------------------------------------------------------
void CPlayerAnimation::NowAttack()
{
	if (m_nowAction != Action::ATTACK) return;

	double nowTime = m_pModel->GetTime();
	double tookTime = nowTime - m_attackStartTime;

	if (tookTime > END_ANIMATION_TIME)
	{
		m_isLockAni = false; //���b�N����
		m_isNowAttack = false;
	}
	else if (tookTime > START_ANIMATION_TIME && tookTime < END_ANIMATION_TIME)
	{
		m_isNowAttack = true;
	}
}

//--------------------------------------------------------------------------------
// �K�[�h���̏���
//--------------------------------------------------------------------------------
void CPlayerAnimation::NowGuard()
{
	if (m_nowAction != Action::GUARD) return;

	double nowTime = m_pModel->GetTime();
	double tookTime = nowTime - m_guardStartTime;

	if (tookTime > END_ANIMATION_TIME)
	{
		m_isLockAni = false; //���b�N����
	}
}

//--------------------------------------------------------------------------------
// ���S���̏���
//--------------------------------------------------------------------------------
void CPlayerAnimation::NowDeath()
{
	if (m_nowAction != Action::DEATH) return;

	double nowTime = m_pModel->GetTime();
	double tookTime = nowTime - m_deathStartTime;

	if (tookTime > END_ANIMATION_TIME)
	{
		m_pModel->SetAdvanceTime(SLEEP_TIME);
		m_isDeath = true;
	}
}

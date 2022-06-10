//********************************************************************************
//
// LockOn.hpp
//
// ���b�N�I���J����
//
// Date: 2019/07/30 �X�V
//
//********************************************************************************

//================================================================================
// �C���N���[�h
//================================================================================
#include "LockOn.hpp"

using namespace FileConst;

//================================================================================
// �萔��`
//================================================================================
const Vector3 LOCKON_ADJUST = Vector3(0.0f, 1.0f, 0.0f);

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
CLockOnSystem::CLockOnSystem()
	: m_distance(0.0f)
	, m_vecPosDelay(Vector3(0.0f, 0.0f, 0.0f))
	, m_vecPlayer(Vector3(0.0f, 0.0f, 0.0f))
	, m_angle(0.0f)
	, m_size(100.0f)
	, m_isLockOn(false)
{
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
CLockOnSystem::~CLockOnSystem()
{
}

//--------------------------------------------------------------------------------
// �t�@�C���ǂݍ���
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CLockOnSystem::LoadFile(CSystem * pSystem)
{
	pSystem->LoadTexture(TEXTURE_LOCKON, "Resource/Textures/Game/LockOn.png");
}

//--------------------------------------------------------------------------------
// ������
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CLockOnSystem::Initialize(CSystem * pSystem)
{
	m_pTexture = pSystem->GetTexture(TEXTURE_LOCKON);
}

//--------------------------------------------------------------------------------
// �X�V
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
// �@�@�@�@CPlayer*	�v���C���[�N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CLockOnSystem::Update(CSystem * pSystem, CPlayer * pPlayer, CAIManager* pAI)
{
	m_isLockOn = pPlayer->IsLockOn();

	CalcAIAndPlayerDistance(pPlayer, pAI);

	CalcLockDirection();
}

//--------------------------------------------------------------------------------
// �`��
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CLockOnSystem::Draw(CSystem * pSystem, CCamera* pCamera)
{
	if (m_isLockOn)
	{
		RECT rSour, rDest;

		Vector3 lockOn = m_vecPosDelay + LOCKON_ADJUST;

		Vector3 Screen = Calculate::WorldToScreen(
			pSystem->GetDevice(),
			&pCamera->GetViewMatrix(),
			&pCamera->GetProjectionMatrix(),
			&lockOn);

		SetRect(&rSour, 0, 0, 650, 650);
		SetRect(&rDest,
			(int)(Screen.x - (m_size / 2)),
			(int)(Screen.y - (m_size / 2)),
			(int)(Screen.x + (m_size / 2)),
			(int)(Screen.y + (m_size / 2)));

		pSystem->SpriteBegin();
		pSystem->DrawSprite(&rSour, m_pTexture, &rDest);
		pSystem->SpriteClose();
	}
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// �G�ƃv���C���[�̋����v�Z
//--------------------------------------------------------------------------------
void CLockOnSystem::CalcAIAndPlayerDistance(CPlayer * pPlayer, CAIManager * pAI)
{
	auto distance = 100.0f;
	m_vecPlayer = pPlayer->GetPosition();

	for (auto w : pAI->GetEnemyList())
	{
		auto enemy = w->GetPosition();
		auto toPos = m_vecPlayer - enemy;
		auto magnitude = VectorCalculate::Magnitude(toPos);
		if (magnitude < distance)
		{
			distance = magnitude;
			m_distance = magnitude;
			m_position = enemy;
		}
	}

	if (pAI->IsAddedBoss())
	{
		auto boss = pAI->GetBoss()->GetPosition();
		auto toPos = m_vecPlayer - boss;
		auto magnitude = VectorCalculate::Magnitude(toPos);
		if (magnitude < 100.0f)
		{
			distance = magnitude;
			m_distance = magnitude;
			m_position = boss;
		}
	}

	m_vecPosDelay = VectorCalculate::Lerp(m_vecPosDelay, m_position, 0.07f);
}

//--------------------------------------------------------------------------------
// ���b�N�I�������̌v�Z
//--------------------------------------------------------------------------------
void CLockOnSystem::CalcLockDirection()
{
	auto toPos = m_vecPlayer - m_vecPosDelay;
	toPos = VectorCalculate::Normalize(toPos);
	m_angle = atan2f(toPos.x, toPos.z);
}
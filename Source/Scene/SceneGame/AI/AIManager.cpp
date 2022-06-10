//********************************************************************************
//
// AIManager.cpp
//
// �G�̊Ǘ��N���X
//
// Date: 2019/10/11 �X�V
//
//********************************************************************************

//================================================================================
// �C���N���[�h
//================================================================================
#include "AIManager.hpp"

using namespace FileConst;

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
CAIManager::CAIManager()
	: m_pBossObject(nullptr)
	, m_isAddedBoss(false)
{
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
CAIManager::~CAIManager()
{
	Release();
}

//--------------------------------------------------------------------------------
// �t�@�C���ǂݍ���
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CAIManager::LoadFile(CSystem * pSystem)
{
	pSystem->LoadXFile(MODEL_DEMOBOSS,	"Resource/Models/Boss/DemoBoss.x");
	pSystem->LoadXFile(MODEL_ENEMY,		"Resource/Models/Enemy/golem.x");
}

//--------------------------------------------------------------------------------
// ������
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CAIManager::Initialize(CSystem * pSystem) {}

//--------------------------------------------------------------------------------
// �X�V
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
// �@�@�@�@CPlayer*	�v���C���[�N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CAIManager::Update(CSystem * pSystem, CPlayer* pPlayer)
{
	ObjectErase(pSystem);

	for (auto obj : m_lstEnemy)
	{
		obj->SetSleep(m_isSleep);

		obj->Update(pSystem, pPlayer);
	}

	if (m_pBossObject)
	{
		m_pBossObject->SetSleep(m_isSleep);

		m_pBossObject->Update(pSystem, pPlayer);
	}
}

//--------------------------------------------------------------------------------
// �V���h�E�p
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CAIManager::ZDraw(CSystem * pSystem)
{
	for (auto obj : m_lstEnemy)
	{
		obj->ZDraw(pSystem);
	}

	if (m_pBossObject)
	{
		m_pBossObject->ZDraw(pSystem);
	}
}

//--------------------------------------------------------------------------------
// �`��
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CAIManager::Draw(CSystem * pSystem)
{
	for (auto obj : m_lstEnemy)
	{
		obj->Draw(pSystem);
	}

	if (m_pBossObject)
	{
		m_pBossObject->Draw(pSystem);
	}
}

//--------------------------------------------------------------------------------
// �G�l�~�[�̒ǉ�����
//--------------------------------------------------------------------------------
void CAIManager::AddEnemyMultiple(CSystem* pSystem, const int num)
{
	CEnemyBase* pWork = nullptr;

	for (int i = 0; i < num; i++)
	{
		pWork = new CGolem();

		if (!pWork)
		{
			pSystem->DebugWorn("Error_Enemy_Object");
			return;
		}

		pWork->SpawnRandomPos(20.0f);

		pWork->Initialize(pSystem);

		m_lstEnemy.push_back(pWork);

		pSystem->DebugInfo("Enemy_Added");
	}
}

//--------------------------------------------------------------------------------
// �{�X�̏o��
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CAIManager::SpawnBoss(CSystem * pSystem)
{
	m_pBossObject = new CBigLeftHand();

	if (!m_pBossObject) return;

	m_pBossObject->LoadFile(pSystem);

	m_pBossObject->Initialize(pSystem);

	pSystem->DebugInfo("Boss_Added");

	m_isAddedBoss = true;
}

//--------------------------------------------------------------------------------
// �z����̍폜
//--------------------------------------------------------------------------------
void CAIManager::ObjectErase(CSystem* pSystem)
{
	if (!m_lstEnemy.empty())
	{
		m_lstEnemy.remove_if(
			[](CEnemyBase* pEnemy) -> bool {
				if (!pEnemy->IsAlive())
				{
					SafeDelete(pEnemy);
					return true;
				}
				return false;
			}
		);
	}
}

//--------------------------------------------------------------------------------
// �G�z��̎擾
// �߂�l�Flist<CEnemyBase*> �G�z��
//--------------------------------------------------------------------------------
list<CEnemyBase*> CAIManager::GetEnemyList() const
{
	return m_lstEnemy;
}

//--------------------------------------------------------------------------------
// �{�X�N���X�̎擾
// �߂�l�FCBossBase*		�{�X�N���X
//--------------------------------------------------------------------------------
CBossBase * CAIManager::GetBoss() const
{
	return m_pBossObject;
}

//--------------------------------------------------------------------------------
// �{�X��ǉ��������ǂ���
// �߂�l�Fbool	�o���������Ȃ��TRUE
//--------------------------------------------------------------------------------
bool CAIManager::IsAddedBoss() const
{
	return m_isAddedBoss;
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// ���
//--------------------------------------------------------------------------------
void CAIManager::Release()
{
	if (!m_lstEnemy.empty())
	{
		for (auto obj : m_lstEnemy)
		{
			SafeDelete(obj);
		}

		m_lstEnemy.clear();
	}

	SafeDelete(m_pBossObject);
}
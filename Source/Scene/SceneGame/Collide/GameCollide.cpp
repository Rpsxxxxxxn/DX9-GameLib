//********************************************************************************
//
// GameCollide.hpp
//
// �Q�[�����I�u�W�F�N�g���m�̓����蔻��
//
// Date: 2019/07/30 �X�V
//
//********************************************************************************

//================================================================================
// �C���N���[�h
//================================================================================
#include "GameCollide.hpp"

using namespace FileConst;

//================================================================================
// �N���X��`
//================================================================================

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
CGameCollide::CGameCollide()
{
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
CGameCollide::~CGameCollide()
{
}

//--------------------------------------------------------------------------------
// �X�V
// ���@���FCSystem*			�V�X�e���N���X�̃|�C���^
// �@�@�@�@CFieldManager*	�t�B�[���h�Ǘ��N���X�̃|�C���^
// �@�@�@�@CPlayerCamera*	�J�����N���X�̃|�C���^
// �@�@�@�@CPlayer*			�v���C���[�N���X�̃|�C���^
// �@�@�@�@CAIManager*		�G�Ǘ��N���X�̃|�C���^
// �@�@�@�@CShowDamage*		�_���[�W�\���p�N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGameCollide::Update(
	CSystem* pSystem,
	CFieldManager* pField,
	CPlayerCamera* pCamera,
	CPlayer* pPlayer, 
	CAIManager* pAI,
	CShowDamage* pDamage
)
{
	CollideFieldAndCamera(pField, pCamera);

	for (auto enemy : pAI->GetEnemyList())
	{
		if (CCollisionCheck::IsFrustumAndBox(pCamera->GetCamera(), enemy->GetBodyBox(), enemy))
		{
			CollideEnemyAndPlayer(pPlayer, pAI, enemy);

			CollideFieldAndEnemy(pField, enemy);

			CollidePlayerWeaponAndEnemy(pSystem, pPlayer, enemy, pDamage);

			CollideEnemyAttackAndPlayer(pSystem, pPlayer, enemy);
		}
	}

	CollideFieldAndPlayer(pField, pPlayer);

	auto isAdded = pAI->IsAddedBoss();

	if (isAdded)
	{
		auto boss = pAI->GetBoss();

		if (CCollisionCheck::IsFrustumAndBox(pCamera->GetCamera(), boss->GetBodyBox(), boss))
		{
			CollideFieldAndBoss(pField, pAI->GetBoss());

			CollideBossAndPlayer(pPlayer, boss);

			CollideBossAttackAndPlayer(pSystem, pPlayer, pAI->GetBoss());

			CollidePlayerWeaponAndBoss(pSystem, pPlayer, pAI->GetBoss(), pDamage);
		}
	}
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// �t�B�[���h�ƃJ�����Ƃ̓����蔻��
// ���@���FCFieldManager*		�t�B�[���h�N���X�̃|�C���^
// �@�@�@�@CPlayerCamera*		�J�����Ƃ̃N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGameCollide::CollideFieldAndCamera(CFieldManager * pField, CPlayerCamera * pCamera)
{
	CCollisionCheck::IsField(pCamera->GetRay(), pField->GetMesh());

	pCamera->GetCamera()->UpdateViewMatrix();
}

//--------------------------------------------------------------------------------
// �t�B�[���h�ƃv���C���[�Ƃ̓����蔻��
// ���@���FCFieldManager*		�t�B�[���h�N���X�̃|�C���^
// �@�@�@�@CPlayer*			�v���C���[�N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGameCollide::CollideFieldAndPlayer(CFieldManager * pField, CPlayer * pPlayer)
{
	CCollisionCheck::IsField(pPlayer->GetRay(), pField->GetMesh());

	CCollisionCheck::IsField(pPlayer->GetFootRight(), pField->GetMesh());

	CCollisionCheck::IsField(pPlayer->GetFootLeft(), pField->GetMesh());

	auto pos = pPlayer->GetPosition();

	pos = VectorCalculate::Clamp(pos, -pField->GetFieldLimit(), pField->GetFieldLimit());

	pPlayer->SetPosition(pos);
}

//--------------------------------------------------------------------------------
// �t�B�[���h�ƃG�l�~�[�Ƃ̓����蔻��
// ���@���FCFieldManager*		�t�B�[���h�N���X�̃|�C���^
// �@�@�@�@CEnemyBase*		�G�l�~�[�N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGameCollide::CollideFieldAndEnemy(CFieldManager * pField, CEnemyBase * pEnemy)
{
	CCollisionCheck::IsField(pEnemy->GetRay(), pField->GetMesh());

	auto pos = pEnemy->GetPosition();

	pos = VectorCalculate::Clamp(pos, -pField->GetFieldLimit(), pField->GetFieldLimit());

	pEnemy->SetPosition(pos);
}

//--------------------------------------------------------------------------------
// �G�ƃv���C���[�Ƃ̓����蔻��
// ���@���FCPlayer*			�v���C���[�N���X�̃|�C���^
// �@�@�@�@CAIManager*		�G�Ǘ��N���X�̃|�C���^
// �@�@�@�@CEnemyBase*		�Q�[���I�u�W�F�N�g�N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGameCollide::CollideEnemyAndPlayer(CPlayer* pPlayer, CAIManager* pAI, CEnemyBase* pObj)
{
	for (auto enemy : pAI->GetEnemyList())
	{
		RigidbodyCollision(enemy, pObj);
	}

	RigidbodyCollision(pPlayer, pObj);
}

//--------------------------------------------------------------------------------
// �v���C���[����ƓG�̓����蔻��
// ���@���FCSystem*			�V�X�e���N���X�̃|�C���^
// �@�@�@�@CPlayer*			�v���C���[�N���X�̃|�C���^
// �@�@�@�@CEnemyBase*		�Q�[���I�u�W�F�N�g�N���X�̃|�C���^
// �@�@�@�@CShowDamage*		�_���[�W�\���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGameCollide::CollidePlayerWeaponAndEnemy(
	CSystem * pSystem,
	CPlayer * pPlayer, 
	CEnemyBase * pEnemy,
	CShowDamage* pDamage
)
{
	auto nowAttack = pPlayer->IsNowAttack();

	if (CCollisionCheck::IsObb(pPlayer->GetAttackBox(), pEnemy->GetBodyBox()) && nowAttack)
	{
		pEnemy->SetHitPlayerAttack(pSystem, pDamage, pPlayer->GetAtk());
	}

	if (!nowAttack)
	{
		pEnemy->SetHitReset();
	}
}

//--------------------------------------------------------------------------------
// �v���C���[����ƓG�̓����蔻��
// ���@���FCSystem*			�V�X�e���N���X�̃|�C���^
// �@�@�@�@CPlayer*			�v���C���[�N���X�̃|�C���^
// �@�@�@�@CEnemyBase*		�Q�[���I�u�W�F�N�g�N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGameCollide::CollideEnemyAttackAndPlayer(
	CSystem * pSystem, 
	CPlayer * pPlayer, 
	CEnemyBase * pEnemy
)
{
	auto isEnemyAttack = pEnemy->IsAttackAni();

	if (CCollisionCheck::IsObb(pEnemy->GetAttackBox(), pPlayer->GetBodyBox()) && isEnemyAttack)
	{
		pEnemy->SetPlayerHit();
		pPlayer->SetHitEnemyAttack(pSystem, pEnemy->GetAtk());
	}
}

//--------------------------------------------------------------------------------
// �t�B�[���h�ƃ{�X�̓����蔻��
// ���@���FCFieldManager*		�v���C���[�N���X�̃|�C���^
// �@�@�@�@CBossBase*			�{�X�N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGameCollide::CollideFieldAndBoss(CFieldManager * pField, CBossBase * pBoss)
{
	CCollisionCheck::IsField(pBoss->GetRay(), pField->GetMesh());

	auto pos = pBoss->GetPosition();

	pos = VectorCalculate::Clamp(pos, -pField->GetFieldLimit(), pField->GetFieldLimit());

	pBoss->SetPosition(pos);
}

//--------------------------------------------------------------------------------
// �v���C���[�ƃ{�X�̓����蔻��
// ���@���FCPlayer*			�v���C���[�N���X�̃|�C���^
// �@�@�@�@CBossBase*		�{�X�N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGameCollide::CollideBossAndPlayer(CPlayer * pPlayer, CBossBase * pBoss)
{
	RigidbodyCollision(pPlayer, pBoss);
}

//--------------------------------------------------------------------------------
// �{�X�U���ƃv���C���[�̓����蔻��
// ���@���FCSystem*			�V�X�e���N���X�̃|�C���^
// �@�@�@�@CPlayer*			�v���C���[�N���X�̃|�C���^
// �@�@�@�@CBossBase*		�{�X�N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGameCollide::CollideBossAttackAndPlayer(
	CSystem* pSystem, 
	CPlayer * pPlayer, 
	CBossBase * pBoss
)
{
	auto isBossAttack = pBoss->IsAttackAni();

	if (CCollisionCheck::IsObb(pBoss->GetAttackBox(), pPlayer->GetBodyBox()) && isBossAttack)
	{
		pBoss->SetPlayerHit();
		pPlayer->SetHitEnemyAttack(pSystem, pBoss->GetAtk());
	}
}

//--------------------------------------------------------------------------------
// �v���C���[����ƃ{�X�̓����蔻��
// ���@���FCSystem*			�V�X�e���N���X�̃|�C���^
// �@�@�@�@CPlayer*			�v���C���[�N���X�̃|�C���^
// �@�@�@�@CBossBase*		�{�X�N���X�̃|�C���^
// �@�@�@�@CShowDamage*		�_���[�W�\���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGameCollide::CollidePlayerWeaponAndBoss(
	CSystem* pSystem, 
	CPlayer * pPlayer, 
	CBossBase * pBoss, 
	CShowDamage* pDamage
)
{
	auto nowAttack = pPlayer->IsNowAttack();

	if (CCollisionCheck::IsObb(pPlayer->GetAttackBox(), pBoss->GetBodyBox()) && nowAttack)
	{
		pBoss->SetHitPlayerAttack(pSystem, pDamage, pPlayer->GetAtk());
	}

	if (!nowAttack)
	{
		pBoss->SetHitReset();
	}
}

//--------------------------------------------------------------------------------
// ���̓����蔻��
// ���@���FCGameObject*	�Q�[���I�u�W�F�N�g�̃|�C���^
// �@�@�@�@CGameObject*	�Q�[���I�u�W�F�N�g�̃|�C���^
//--------------------------------------------------------------------------------
void CGameCollide::RigidbodyCollision(CGameObject * pObj1, CGameObject * pObj2)
{
	Vector3 obj1Pos = pObj1->GetPosition();
	Vector3 obj2Pos = pObj2->GetPosition();

	Vector3 toPos = obj2Pos - obj1Pos;
	float mass = 10.0f;
	float radius = pObj1->GetRadius() + pObj2->GetRadius();

	float distance = VectorCalculate::Magnitude(toPos);
	if (distance <= 0) return; //������0�ȉ��Ȃ�ΊO���
	float inverseDist = 1 / distance; //���˕Ԃ�p

	Vector3 nPos = toPos * inverseDist;

	float penetration = radius - distance;
	if (penetration <= 0) return;

	Vector3 pPos = penetration * nPos;

	//���˕Ԃ�p�d�ʂ����߂�
	float inverseMass = 1.0f / mass;
	float impulse1 = mass * inverseMass;
	float impulse2 = mass * inverseMass;

	obj1Pos -= pPos * impulse1;
	obj2Pos += pPos * impulse2;

	pObj1->SetPosition(obj1Pos);
	pObj2->SetPosition(obj2Pos);
}
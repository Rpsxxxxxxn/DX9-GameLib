//********************************************************************************
//
// GameCollide.hpp
//
// ゲーム内オブジェクト同士の当たり判定
//
// Date: 2019/07/30 更新
//
//********************************************************************************

//================================================================================
// インクルード
//================================================================================
#include "GameCollide.hpp"

using namespace FileConst;

//================================================================================
// クラス定義
//================================================================================

//--------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------
CGameCollide::CGameCollide()
{
}

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
CGameCollide::~CGameCollide()
{
}

//--------------------------------------------------------------------------------
// 更新
// 引　数：CSystem*			システムクラスのポインタ
// 　　　　CFieldManager*	フィールド管理クラスのポインタ
// 　　　　CPlayerCamera*	カメラクラスのポインタ
// 　　　　CPlayer*			プレイヤークラスのポインタ
// 　　　　CAIManager*		敵管理クラスのポインタ
// 　　　　CShowDamage*		ダメージ表示用クラスのポインタ
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
// フィールドとカメラとの当たり判定
// 引　数：CFieldManager*		フィールドクラスのポインタ
// 　　　　CPlayerCamera*		カメラとのクラスのポインタ
//--------------------------------------------------------------------------------
void CGameCollide::CollideFieldAndCamera(CFieldManager * pField, CPlayerCamera * pCamera)
{
	CCollisionCheck::IsField(pCamera->GetRay(), pField->GetMesh());

	pCamera->GetCamera()->UpdateViewMatrix();
}

//--------------------------------------------------------------------------------
// フィールドとプレイヤーとの当たり判定
// 引　数：CFieldManager*		フィールドクラスのポインタ
// 　　　　CPlayer*			プレイヤークラスのポインタ
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
// フィールドとエネミーとの当たり判定
// 引　数：CFieldManager*		フィールドクラスのポインタ
// 　　　　CEnemyBase*		エネミークラスのポインタ
//--------------------------------------------------------------------------------
void CGameCollide::CollideFieldAndEnemy(CFieldManager * pField, CEnemyBase * pEnemy)
{
	CCollisionCheck::IsField(pEnemy->GetRay(), pField->GetMesh());

	auto pos = pEnemy->GetPosition();

	pos = VectorCalculate::Clamp(pos, -pField->GetFieldLimit(), pField->GetFieldLimit());

	pEnemy->SetPosition(pos);
}

//--------------------------------------------------------------------------------
// 敵とプレイヤーとの当たり判定
// 引　数：CPlayer*			プレイヤークラスのポインタ
// 　　　　CAIManager*		敵管理クラスのポインタ
// 　　　　CEnemyBase*		ゲームオブジェクトクラスのポインタ
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
// プレイヤー武器と敵の当たり判定
// 引　数：CSystem*			システムクラスのポインタ
// 　　　　CPlayer*			プレイヤークラスのポインタ
// 　　　　CEnemyBase*		ゲームオブジェクトクラスのポインタ
// 　　　　CShowDamage*		ダメージ表示クラスのポインタ
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
// プレイヤー武器と敵の当たり判定
// 引　数：CSystem*			システムクラスのポインタ
// 　　　　CPlayer*			プレイヤークラスのポインタ
// 　　　　CEnemyBase*		ゲームオブジェクトクラスのポインタ
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
// フィールドとボスの当たり判定
// 引　数：CFieldManager*		プレイヤークラスのポインタ
// 　　　　CBossBase*			ボスクラスのポインタ
//--------------------------------------------------------------------------------
void CGameCollide::CollideFieldAndBoss(CFieldManager * pField, CBossBase * pBoss)
{
	CCollisionCheck::IsField(pBoss->GetRay(), pField->GetMesh());

	auto pos = pBoss->GetPosition();

	pos = VectorCalculate::Clamp(pos, -pField->GetFieldLimit(), pField->GetFieldLimit());

	pBoss->SetPosition(pos);
}

//--------------------------------------------------------------------------------
// プレイヤーとボスの当たり判定
// 引　数：CPlayer*			プレイヤークラスのポインタ
// 　　　　CBossBase*		ボスクラスのポインタ
//--------------------------------------------------------------------------------
void CGameCollide::CollideBossAndPlayer(CPlayer * pPlayer, CBossBase * pBoss)
{
	RigidbodyCollision(pPlayer, pBoss);
}

//--------------------------------------------------------------------------------
// ボス攻撃とプレイヤーの当たり判定
// 引　数：CSystem*			システムクラスのポインタ
// 　　　　CPlayer*			プレイヤークラスのポインタ
// 　　　　CBossBase*		ボスクラスのポインタ
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
// プレイヤー武器とボスの当たり判定
// 引　数：CSystem*			システムクラスのポインタ
// 　　　　CPlayer*			プレイヤークラスのポインタ
// 　　　　CBossBase*		ボスクラスのポインタ
// 　　　　CShowDamage*		ダメージ表示クラスのポインタ
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
// 剛体当たり判定
// 引　数：CGameObject*	ゲームオブジェクトのポインタ
// 　　　　CGameObject*	ゲームオブジェクトのポインタ
//--------------------------------------------------------------------------------
void CGameCollide::RigidbodyCollision(CGameObject * pObj1, CGameObject * pObj2)
{
	Vector3 obj1Pos = pObj1->GetPosition();
	Vector3 obj2Pos = pObj2->GetPosition();

	Vector3 toPos = obj2Pos - obj1Pos;
	float mass = 10.0f;
	float radius = pObj1->GetRadius() + pObj2->GetRadius();

	float distance = VectorCalculate::Magnitude(toPos);
	if (distance <= 0) return; //距離が0以下ならば外れる
	float inverseDist = 1 / distance; //跳ね返り用

	Vector3 nPos = toPos * inverseDist;

	float penetration = radius - distance;
	if (penetration <= 0) return;

	Vector3 pPos = penetration * nPos;

	//跳ね返り用重量を求める
	float inverseMass = 1.0f / mass;
	float impulse1 = mass * inverseMass;
	float impulse2 = mass * inverseMass;

	obj1Pos -= pPos * impulse1;
	obj2Pos += pPos * impulse2;

	pObj1->SetPosition(obj1Pos);
	pObj2->SetPosition(obj2Pos);
}
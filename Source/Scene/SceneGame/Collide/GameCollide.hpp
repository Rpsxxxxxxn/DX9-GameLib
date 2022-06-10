//********************************************************************************
//
// GameCollide.hpp
//
// ゲーム内オブジェクト同士の当たり判定
//
// Date: 2019/07/30 更新
//
//********************************************************************************

#pragma once

//================================================================================
// インクルード
//================================================================================
#include "../../GameManager/GameManager.h"
#include "../Camera/PlayerCamera.hpp"
#include "../AI/AIManager.hpp"
#include "../Player/Player.hpp"
#include "../Field/FieldManager.h"
#include "../Hud/Damage/Damage.hpp"

//================================================================================
// クラス定義
//================================================================================
class CGameCollide
{
public:

	//--------------------------------------------------------------------------------
	// コンストラクタ
	//--------------------------------------------------------------------------------
	CGameCollide();

	//--------------------------------------------------------------------------------
	// デストラクタ
	//--------------------------------------------------------------------------------
	~CGameCollide();

	//--------------------------------------------------------------------------------
	// 更新
	// 引　数：CSystem*			システムクラスのポインタ
	// 　　　　CFieldManager*	フィールド管理クラスのポインタ
	// 　　　　CPlayerCamera*	カメラクラスのポインタ
	// 　　　　CPlayer*			プレイヤークラスのポインタ
	// 　　　　CAIManager*		敵管理クラスのポインタ
	// 　　　　CShowDamage*		ダメージ表示用クラスのポインタ
	//--------------------------------------------------------------------------------
	void Update(
		CSystem* pSystem,
		CFieldManager* pField,
		CPlayerCamera* pCamera,
		CPlayer* pPlayer, 
		CAIManager* pAI, 
		CShowDamage* pDamage);

private:

	//--------------------------------------------------------------------------------
	// フィールドとカメラとの当たり判定
	// 引　数：CFieldManager*	フィールドクラスのポインタ
	// 　　　　CPlayerCamera*	カメラとのクラスのポインタ
	//--------------------------------------------------------------------------------
	void CollideFieldAndCamera(CFieldManager* pField, CPlayerCamera* pCamera);

	//--------------------------------------------------------------------------------
	// フィールドとプレイヤーとの当たり判定
	// 引　数：CFieldManager*	フィールドクラスのポインタ
	// 　　　　CPlayer*			プレイヤークラスのポインタ
	//--------------------------------------------------------------------------------
	void CollideFieldAndPlayer(CFieldManager* pField, CPlayer* pPlayer);

	//--------------------------------------------------------------------------------
	// フィールドとエネミーとの当たり判定
	// 引　数：CFieldManager*	フィールドクラスのポインタ
	// 　　　　CEnemyBase*		エネミークラスのポインタ
	//--------------------------------------------------------------------------------
	void CollideFieldAndEnemy(CFieldManager* pField, CEnemyBase* pEnemy);

	//--------------------------------------------------------------------------------
	// 敵とプレイヤーとの当たり判定
	// 引　数：CPlayer*			プレイヤークラスのポインタ
	// 　　　　CAIManager*		敵管理クラスのポインタ
	// 　　　　CEnemyBase*		ゲームオブジェクトクラスのポインタ
	//--------------------------------------------------------------------------------
	void CollideEnemyAndPlayer(CPlayer* pPlayer, CAIManager* pAI, CEnemyBase* pObj);

	//--------------------------------------------------------------------------------
	// プレイヤー武器と敵の当たり判定
	// 引　数：CSystem*			システムクラスのポインタ
	// 　　　　CPlayer*			プレイヤークラスのポインタ
	// 　　　　CEnemyBase*		ゲームオブジェクトクラスのポインタ
	// 　　　　CShowDamage*		ダメージ表示クラスのポインタ
	//--------------------------------------------------------------------------------
	void CollidePlayerWeaponAndEnemy(CSystem* pSystem, CPlayer* pPlayer, CEnemyBase* pEnemy, CShowDamage* pDamage);

	//--------------------------------------------------------------------------------
	// 敵の攻撃とプレイヤーの当たり判定
	// 引　数：CSystem*			システムクラスのポインタ
	// 　　　　CPlayer*			プレイヤークラスのポインタ
	// 　　　　CEnemyBase*		ゲームオブジェクトクラスのポインタ
	//--------------------------------------------------------------------------------
	void CollideEnemyAttackAndPlayer(CSystem* pSystem, CPlayer* pPlayer, CEnemyBase* pEnemy);

	//--------------------------------------------------------------------------------
	// フィールドとボスの当たり判定
	// 引　数：CFieldManager*	プレイヤークラスのポインタ
	// 　　　　CBossBase*		ボスクラスのポインタ
	//--------------------------------------------------------------------------------
	void CollideFieldAndBoss(CFieldManager* pField, CBossBase* pBoss);

	//--------------------------------------------------------------------------------
	// プレイヤーとボスの当たり判定
	// 引　数：CPlayer*			プレイヤークラスのポインタ
	// 　　　　CBossBase*		ボスクラスのポインタ
	//--------------------------------------------------------------------------------
	void CollideBossAndPlayer(CPlayer* pPlayer, CBossBase* pBoss);

	//--------------------------------------------------------------------------------
	// ボス攻撃とプレイヤーの当たり判定
	// 引　数：CSystem*			システムクラスのポインタ
	// 　　　　CPlayer*			プレイヤークラスのポインタ
	// 　　　　CBossBase*		ボスクラスのポインタ
	//--------------------------------------------------------------------------------
	void CollideBossAttackAndPlayer(CSystem* pSystem, CPlayer* pPlayer, CBossBase* pBoss);

	//--------------------------------------------------------------------------------
	// プレイヤー武器とボスの当たり判定
	// 引　数：CSystem*			システムクラスのポインタ
	// 　　　　CPlayer*			プレイヤークラスのポインタ
	// 　　　　CBossBase*		ボスクラスのポインタ
	// 　　　　CShowDamage*		ダメージ表示クラスのポインタ
	//--------------------------------------------------------------------------------
	void CollidePlayerWeaponAndBoss(CSystem* pSystem, CPlayer* pPlayer, CBossBase* pBoss, CShowDamage* pDamage);

	//--------------------------------------------------------------------------------
	// 剛体当たり判定
	// 引　数：CGameObject*	ゲームオブジェクトのポインタ
	// 　　　　CGameObject*	ゲームオブジェクトのポインタ
	//--------------------------------------------------------------------------------
	static void RigidbodyCollision(CGameObject* pObj1, CGameObject* pObj2);
};
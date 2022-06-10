//********************************************************************************
//
// Golem.cpp
//
// ゴーレム
//
// Date: 2019/07/30 更新
//
//********************************************************************************

//================================================================================
// インクルード
//================================================================================
#include "Golem.hpp"

using namespace FileConst;

//================================================================================
// 定数定義
//================================================================================
const Vector3 FORWARD(0.0f, 0.0f, 1.0f);
const float ANIMATION_END = 0.9f;

//================================================================================
// クラス定義
//================================================================================

//--------------------------------------------------------------------------------
// コンストラクタ
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
// デストラクタ
//--------------------------------------------------------------------------------
CGolem::~CGolem()
{
}

//--------------------------------------------------------------------------------
// 初期化
// 引　数：CSystem*	システムクラスのポインタ
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
// 更新
// 引　数：CSystem*	システムクラスのポインタ
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
// 敵の操作
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
// 敵の操作
//--------------------------------------------------------------------------------
void CGolem::UpdateDiscovery(CPlayer * pPlayer)
{
	//2点距離の計算
	Vector3 toPos = pPlayer->GetPosition() - m_position;

	//距離計算
	m_distance = VectorCalculate::Magnitude(toPos);

	//距離が範囲に入れば発見。
	if (m_distance < 5.0f)
	{
		m_isDiscovery = true;
	}
	else {
		m_isDiscovery = false;
	}
}

//--------------------------------------------------------------------------------
// 自由に歩く
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
		// 範囲外
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
// 歩く
// 引　数：const float	近距離限界
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
// 回転
// 引　数：Vector3&	ターゲットとなる位置ベクトル
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
// 死後のメッシュ透過
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
// 動作速度の更新
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
// アニメーション更新
//--------------------------------------------------------------------------------
void CGolem::ModelRotate(const Vector3& pCross)
{
	if (pCross.y < D3DXToRadian(-2.0f))
	{
		m_rotation.y -= D3DXToRadian(2.0f); //左へ回す
	}

	if (pCross.y > D3DXToRadian(2.0f))
	{
		m_rotation.y += D3DXToRadian(2.0f); //右へ回す
	}
}

//--------------------------------------------------------------------------------
// 攻撃距離
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
// ランダム攻撃
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
// 攻撃を開始する
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
// 攻撃する
//--------------------------------------------------------------------------------
void CGolem::NowAttack()
{
	if (!m_isNowAttack) return;

	double nowTime = m_pModel->GetTime();
	double tookTime = nowTime - m_attackStartTime;

	if (tookTime >= 0.15 && tookTime < 0.65)
	{
		//プレイヤーに一度もヒットしていないとき
		if (!m_isHitPlayer)
		{
			m_isAttackFlg = true;
		}
		else {
			//プレイヤーヒットしたとき。
			//この処理は一度のみ攻撃を与えるための処理
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
// 死亡する
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
// アニメーション更新
//--------------------------------------------------------------------------------
void CGolem::UpdateAnimation()
{
	m_pModel->SetChangeAnimation(m_nowAnimation);
}

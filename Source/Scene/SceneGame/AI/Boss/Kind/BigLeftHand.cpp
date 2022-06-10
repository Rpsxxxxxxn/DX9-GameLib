//********************************************************************************
//
// BigLeftHand.cpp
//
// 左手がでかいボスクラス
//
// Date: 2019/07/30 更新
//
//********************************************************************************

//================================================================================
// インクルード
//================================================================================
#include "BigLeftHand.hpp"

using namespace FileConst;

//================================================================================
// 定数定義
//================================================================================
const Vector3 FORWARD(0.0f, 0.0f, -1.0f);
const float ANIMATION_END = 0.9f;

//================================================================================
// クラス定義
//================================================================================

//--------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------
CBigLeftHand::CBigLeftHand()
	: m_isAttackDist(false)
{
}

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
CBigLeftHand::~CBigLeftHand()
{
}

//--------------------------------------------------------------------------------
// ファイル読み込み
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CBigLeftHand::LoadFile(CSystem * pSystem)
{
	CBossBase::LoadParams("BigLeftHand");
}

//--------------------------------------------------------------------------------
// 初期化
// 引　数：CSystem*	システムクラスのポインタ
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
// 更新
// 引　数：CSystem*	システムクラスのポインタ
// 　　　　CPlayer*	プレイヤークラスのポインタ
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
// 敵の操作
// 引　数：CPlayer*	プレイヤークラスのポインタ
//--------------------------------------------------------------------------------
void CBigLeftHand::Controller(CSystem* pSystem, CPlayer * pPlayer)
{
	if (m_isDeath) return;

	//発見
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
// プレイヤーが範囲内に入っているか検索
// 引　数：CPlayer*	プレイヤークラスのポインタ
//--------------------------------------------------------------------------------
void CBigLeftHand::UpdateDiscovery(CPlayer * pPlayer)
{
	//2点距離の計算
	Vector3 toPos = pPlayer->GetPosition() - m_position;

	//距離計算
	m_distance = VectorCalculate::Magnitude(toPos);

	//距離が範囲に入れば発見。
	if (m_distance < 50.0f)
	{
		m_isDiscovery = true;
	}
	else {
		m_isDiscovery = false;
	}
}

//--------------------------------------------------------------------------------
// 歩く
// 引　数：const float	近距離限界
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
// 回転
// 引　数：Vector3&	ターゲットとなる位置ベクトル
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
// アニメーション更新
//--------------------------------------------------------------------------------
void CBigLeftHand::ModelRotate(const Vector3& pCross)
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
// ランダム攻撃
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
// 攻撃を開始する
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
// 攻撃中の処理
//--------------------------------------------------------------------------------
void CBigLeftHand::NowAttack()
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
// アニメーション更新
//--------------------------------------------------------------------------------
void CBigLeftHand::UpdateAnimation()
{
	m_pModel->SetChangeAnimation(m_nowAnimation);
}
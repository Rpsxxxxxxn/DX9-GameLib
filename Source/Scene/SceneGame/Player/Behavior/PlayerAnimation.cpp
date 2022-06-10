//********************************************************************************
//
// PlayerAnimation.hpp
//
// プレイヤーアニメーションのクラス
//
// Date: 2019/10/01 更新
//
//********************************************************************************

//================================================================================
// インクルード
//================================================================================
#include "PlayerAnimation.hpp"

using namespace PlayerAnimation;

const double SLEEP_TIME = 0;
const double START_ANIMATION_TIME = 0.15;
const double END_ANIMATION_TIME = 0.75;

//================================================================================
// クラス定義
//================================================================================

//--------------------------------------------------------------------------------
// コンストラクタ
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
// デストラクタ
//--------------------------------------------------------------------------------
CPlayerAnimation::~CPlayerAnimation()
{
	if(m_pModel) m_pModel = nullptr;
}

//--------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------
void CPlayerAnimation::Update()
{
	//アニメーション変更の更新
	UpdateChangeAni();

	//死亡以外
	if (!m_isDeathLock)
	{
		//アニメーション処理の変更
		switch (m_nowAction)
		{
		case ::Action::IDLE: m_nowAni = ::Animation::IDLE; break;
		case ::Action::WALK: m_nowAni = ::Animation::WALK; break;
		case ::Action::DASH: m_nowAni = ::Animation::DASH; break;
		case ::Action::ATTACK: StartAttack(); break;
		case ::Action::GUARD: StartGuard(); break;
		}
	}

	//アニメーションの更新
	UpdateAni();
}

//--------------------------------------------------------------------------------
// 死亡割込み処理
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
// 現在のアニメーション変更
// 引　数：PlayerAnimation::Action	アクション列挙体
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
// アニメーション変更の更新
//--------------------------------------------------------------------------------
void CPlayerAnimation::UpdateChangeAni()
{
	if (m_nowAction == m_newAction && m_isLockAni) return;

	m_nowAction = m_newAction;
}

//--------------------------------------------------------------------------------
// アニメーションの更新
//--------------------------------------------------------------------------------
void CPlayerAnimation::UpdateAni()
{
	NowDeath();

	NowAttack();

	NowGuard();

	m_pModel->SetChangeAnimation(m_nowAni);
}

//--------------------------------------------------------------------------------
// 攻撃処理
//--------------------------------------------------------------------------------
void CPlayerAnimation::StartAttack()
{
	if ( m_isLockAni ) return;

	//ロックをかける
	m_isLockAni = true;

	// 攻撃処理の変更
	switch ( m_nowAttackCnt )
	{
		case 0: m_nowAni = PlayerAnimation::Animation::ATTACK1; break;
		case 1: m_nowAni = PlayerAnimation::Animation::ATTACK2; break;
		case 2: m_nowAni = PlayerAnimation::Animation::ATTACK3; break;
		case 3: m_nowAni = PlayerAnimation::Animation::ATTACK4; break;
		case 4: m_nowAni = PlayerAnimation::Animation::ATTACK5; break;
	}

	m_attackStartTime = m_pModel->GetTime(); //スタート時間の取得
	m_nowAttackCnt ++; //攻撃の変更
	m_nowAttackCnt %= 5;
}

//--------------------------------------------------------------------------------
// ガード処理
//--------------------------------------------------------------------------------
void CPlayerAnimation::StartGuard()
{
	if (m_isLockAni) return;

	//ロックをかける
	m_isLockAni = true;
}

//--------------------------------------------------------------------------------
// 攻撃中の処理
//--------------------------------------------------------------------------------
void CPlayerAnimation::NowAttack()
{
	if (m_nowAction != Action::ATTACK) return;

	double nowTime = m_pModel->GetTime();
	double tookTime = nowTime - m_attackStartTime;

	if (tookTime > END_ANIMATION_TIME)
	{
		m_isLockAni = false; //ロック解除
		m_isNowAttack = false;
	}
	else if (tookTime > START_ANIMATION_TIME && tookTime < END_ANIMATION_TIME)
	{
		m_isNowAttack = true;
	}
}

//--------------------------------------------------------------------------------
// ガード中の処理
//--------------------------------------------------------------------------------
void CPlayerAnimation::NowGuard()
{
	if (m_nowAction != Action::GUARD) return;

	double nowTime = m_pModel->GetTime();
	double tookTime = nowTime - m_guardStartTime;

	if (tookTime > END_ANIMATION_TIME)
	{
		m_isLockAni = false; //ロック解除
	}
}

//--------------------------------------------------------------------------------
// 死亡中の処理
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

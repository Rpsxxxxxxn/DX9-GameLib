//********************************************************************************
//
// LockOn.hpp
//
// ロックオンカメラ
//
// Date: 2019/07/30 更新
//
//********************************************************************************

//================================================================================
// インクルード
//================================================================================
#include "LockOn.hpp"

using namespace FileConst;

//================================================================================
// 定数定義
//================================================================================
const Vector3 LOCKON_ADJUST = Vector3(0.0f, 1.0f, 0.0f);

//--------------------------------------------------------------------------------
// コンストラクタ
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
// デストラクタ
//--------------------------------------------------------------------------------
CLockOnSystem::~CLockOnSystem()
{
}

//--------------------------------------------------------------------------------
// ファイル読み込み
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CLockOnSystem::LoadFile(CSystem * pSystem)
{
	pSystem->LoadTexture(TEXTURE_LOCKON, "Resource/Textures/Game/LockOn.png");
}

//--------------------------------------------------------------------------------
// 初期化
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CLockOnSystem::Initialize(CSystem * pSystem)
{
	m_pTexture = pSystem->GetTexture(TEXTURE_LOCKON);
}

//--------------------------------------------------------------------------------
// 更新
// 引　数：CSystem*	システムクラスのポインタ
// 　　　　CPlayer*	プレイヤークラスのポインタ
//--------------------------------------------------------------------------------
void CLockOnSystem::Update(CSystem * pSystem, CPlayer * pPlayer, CAIManager* pAI)
{
	m_isLockOn = pPlayer->IsLockOn();

	CalcAIAndPlayerDistance(pPlayer, pAI);

	CalcLockDirection();
}

//--------------------------------------------------------------------------------
// 描画
// 引　数：CSystem*	システムクラスのポインタ
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
// 敵とプレイヤーの距離計算
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
// ロックオン方向の計算
//--------------------------------------------------------------------------------
void CLockOnSystem::CalcLockDirection()
{
	auto toPos = m_vecPlayer - m_vecPosDelay;
	toPos = VectorCalculate::Normalize(toPos);
	m_angle = atan2f(toPos.x, toPos.z);
}
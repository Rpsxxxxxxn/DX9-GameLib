//********************************************************************************
//
// AIManager.cpp
//
// 敵の管理クラス
//
// Date: 2019/10/11 更新
//
//********************************************************************************

//================================================================================
// インクルード
//================================================================================
#include "AIManager.hpp"

using namespace FileConst;

//--------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------
CAIManager::CAIManager()
	: m_pBossObject(nullptr)
	, m_isAddedBoss(false)
{
}

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
CAIManager::~CAIManager()
{
	Release();
}

//--------------------------------------------------------------------------------
// ファイル読み込み
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CAIManager::LoadFile(CSystem * pSystem)
{
	pSystem->LoadXFile(MODEL_DEMOBOSS,	"Resource/Models/Boss/DemoBoss.x");
	pSystem->LoadXFile(MODEL_ENEMY,		"Resource/Models/Enemy/golem.x");
}

//--------------------------------------------------------------------------------
// 初期化
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CAIManager::Initialize(CSystem * pSystem) {}

//--------------------------------------------------------------------------------
// 更新
// 引　数：CSystem*	システムクラスのポインタ
// 　　　　CPlayer*	プレイヤークラスのポインタ
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
// シャドウ用
// 引　数：CSystem*	システムクラスのポインタ
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
// 描画
// 引　数：CSystem*	システムクラスのポインタ
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
// エネミーの追加複数
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
// ボスの出現
// 引　数：CSystem*	システムクラスのポインタ
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
// 配列内の削除
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
// 敵配列の取得
// 戻り値：list<CEnemyBase*> 敵配列
//--------------------------------------------------------------------------------
list<CEnemyBase*> CAIManager::GetEnemyList() const
{
	return m_lstEnemy;
}

//--------------------------------------------------------------------------------
// ボスクラスの取得
// 戻り値：CBossBase*		ボスクラス
//--------------------------------------------------------------------------------
CBossBase * CAIManager::GetBoss() const
{
	return m_pBossObject;
}

//--------------------------------------------------------------------------------
// ボスを追加したかどうか
// 戻り値：bool	出現させたならばTRUE
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
// 解放
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
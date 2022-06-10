//********************************************************************************
//
// FieldManager.cpp
//
// フィールド管理
//
// Date: 2019/10/07 更新
//
//********************************************************************************

//================================================================================
// インクルード
//================================================================================
#include "FieldManager.h"

using namespace FileConst;

//================================================================================
// クラス定義
//================================================================================

//--------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------
CFieldManager::CFieldManager()
	: m_pField(new CField)
{
}

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
CFieldManager::~CFieldManager()
{
	SafeDelete(m_pField);
}

//--------------------------------------------------------------------------------
// ファイル読み込み
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CFieldManager::LoadFile(CSystem * pSystem)
{
	LoadStage(pSystem);
}

//--------------------------------------------------------------------------------
// 初期化
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CFieldManager::Initialize(CSystem * pSystem)
{
	m_pField->Initialize(pSystem);
}

//--------------------------------------------------------------------------------
// 更新
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CFieldManager::Update(CSystem * pSystem)
{
	m_pField->Update(pSystem);
}

//--------------------------------------------------------------------------------
// 描画
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CFieldManager::ZDraw(CSystem * pSystem)
{
	m_pField->DrawZDepth(pSystem);
}

//--------------------------------------------------------------------------------
// 描画
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CFieldManager::Draw(CSystem * pSystem)
{
	m_pField->DrawShader(pSystem);
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// ステージの読み込み
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CFieldManager::LoadStage(CSystem* pSystem)
{
	pSystem->LoadXFile(MODEL_FIELD1, "Resource/Models/Stage/Field4.x");
	pSystem->LoadXFile(MODEL_FIELD2, "Resource/Models/Stage/Field2.x");
	pSystem->LoadXFile(MODEL_FIELD3, "Resource/Models/Stage/Field3.x");

	Create(pSystem, MODEL_FIELD1);
}

//--------------------------------------------------------------------------------
// ステージの更新
// 引　数：CSystem*	システムクラスのポインタ
// 　　　　const int ステージ数
//--------------------------------------------------------------------------------
void CFieldManager::UpdateStage(CSystem* pSystem, const int stageNum)
{
	switch (stageNum)
	{
	case 1: m_pField->Create(pSystem, MODEL_FIELD1); break;
	case 2: m_pField->Create(pSystem, MODEL_FIELD2); break;
	case 3: m_pField->Create(pSystem, MODEL_FIELD3); break;
	}
}

//--------------------------------------------------------------------------------
// 当たり判定の生成
// 引　数：CSystem*	システムクラスのポインタ
// 　　　　const int 読み込んだファイル番号
//--------------------------------------------------------------------------------
void CFieldManager::Create(CSystem* pSystem, const int field)
{
	m_pField->Create(pSystem, field);
}

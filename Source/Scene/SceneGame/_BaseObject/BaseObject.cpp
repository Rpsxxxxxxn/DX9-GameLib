//********************************************************************************
//
// BaseObject.hpp
//
// オブジェクトクラスのテンプレート
//
// Date: 2019/07/30 更新
//
//********************************************************************************

//================================================================================
// インクルード
//================================================================================
#include "BaseObject.hpp"

using namespace FileConst;

//================================================================================
// クラス定義
//================================================================================

//================================================================================
// コンストラクタ
//================================================================================
BaseObject::BaseObject()
{
}

//================================================================================
// デストラクタ
//================================================================================
BaseObject::~BaseObject()
{
}

//================================================================================
// ファイル読み込み
// 引　数：CSystem*	システムクラスのポインタ
//================================================================================
void BaseObject::LoadFile(CSystem * pSystem)
{
}

//================================================================================
// 初期化
// 引　数：CSystem*	システムクラスのポインタ
//================================================================================
void BaseObject::Initialize(CSystem * pSystem)
{
}

//================================================================================
// 更新
// 引　数：CSystem*	システムクラスのポインタ
//================================================================================
void BaseObject::Update(CSystem * pSystem)
{
}

//================================================================================
// 描画
// 引　数：CSystem*	システムクラスのポインタ
//================================================================================
void BaseObject::Draw(CSystem * pSystem)
{
}

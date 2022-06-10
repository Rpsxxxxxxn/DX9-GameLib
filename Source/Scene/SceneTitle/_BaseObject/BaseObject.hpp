//********************************************************************************
//
// BaseObject.hpp
//
// オブジェクトクラスのテンプレート
//
// Date: 2019/10/07 更新
//
//********************************************************************************

#pragma once

//================================================================================
// インクルード
//================================================================================
#include "../../GameManager/GameManager.hpp"

//================================================================================
// クラス定義
//================================================================================
class BaseObject
{
public:

	//================================================================================
	// コンストラクタ
	//================================================================================
	BaseObject();

	//================================================================================
	// デストラクタ
	//================================================================================
	~BaseObject();

	//================================================================================
	// ファイル読み込み
	// 引　数：CSystem*	システムクラスのポインタ
	//================================================================================
	void LoadFile(CSystem* pSystem);

	//================================================================================
	// 初期化
	// 引　数：CSystem*	システムクラスのポインタ
	//================================================================================
	void Initialize(CSystem* pSystem);

	//================================================================================
	// 更新
	// 引　数：CSystem*	システムクラスのポインタ
	//================================================================================
	void Update(CSystem* pSystem);

	//================================================================================
	// 描画
	// 引　数：CSystem*	システムクラスのポインタ
	//================================================================================
	void Draw(CSystem* pSystem);

private:

};
//*************************************************************
//
// GameObject.h
//
// オブジェクトクラスのテンプレート
//
// Date: 2019/07/30 更新
//
//*************************************************************

#pragma once

//=============================================================
// インクルード
//=============================================================
#include "../../Library/System/System.h"
#include "../../Library/Utility/Math/Math.h"
#include "../../Library/Utility/Math/Matrix.h"
#include "../../Library/Utility/Math/Vector.h"

//=============================================================
// クラス定義
//=============================================================
class CGameObject
{
public:

	//-------------------------------------------------------------
	// コンストラクタ
	//-------------------------------------------------------------
	CGameObject()
		: m_position(VectorCalculate::Zero)
		, m_rotation(VectorCalculate::Zero)
		, m_scaling(VectorCalculate::One)
		, m_radius(0.0f)
		, m_mass(0.0f)
		, m_isAlive(true)
		, m_isVisible(true)
		, m_isSleep(false)
		, m_matWorld(MatrixCalculate::Identity) {};

	//-------------------------------------------------------------
	// デストラクタ
	//-------------------------------------------------------------
	virtual ~CGameObject() {};

	//-------------------------------------------------------------
	// ファイル読み込み
	// 引　数：CSystem*	システムクラスのポインタ
	//-------------------------------------------------------------
	virtual void LoadFile(CSystem* pSystem) {};

	//-------------------------------------------------------------
	// 初期化
	// 引　数：CSystem*	システムクラスのポインタ
	//-------------------------------------------------------------
	virtual void Initialize(CSystem* pSystem) {};

	//-------------------------------------------------------------
	// 更新
	// 引　数：CSystem*	システムクラスのポインタ
	//-------------------------------------------------------------
	virtual void Update(CSystem* pSystem)
	{
		m_matWorld = MatrixCalculate::Transform(m_scaling, m_rotation, m_position);
	}

	//-------------------------------------------------------------
	// シャドウサンプリング用
	// 引　数：CSystem*	システムクラスのポインタ
	//-------------------------------------------------------------
	virtual void ZDraw(CSystem* pSystem) {};

	//-------------------------------------------------------------
	// 描画
	// 引　数：CSystem*	システムクラスのポインタ
	//-------------------------------------------------------------
	virtual void Draw(CSystem* pSystem) {};

	//-------------------------------------------------------------
	// ゲッター
	//-------------------------------------------------------------
	Vector3 GetPosition() const { return m_position; };
	Vector3 GetRotate() const { return m_rotation; };
	Vector3 GetScale() const { return m_scaling; };
	float GetRadius() const { return m_radius; };
	bool IsAlive() const { return m_isAlive; };
	bool IsVisible() const { return m_isVisible; };
	bool IsSleep() const { return m_isSleep; };

	//-------------------------------------------------------------
	// セッター
	//-------------------------------------------------------------
	void SetPosition(const Vector3& value) { m_position = value; };
	void SetRotate(const Vector3& value) { m_rotation = value; };
	void SetScale(const Vector3& value) { m_scaling = value; };
	void SetAlive(const bool value) { m_isAlive = value; };
	void SetVisible(const bool value) { m_isVisible = value; };
	void SetSleep(const bool value) { m_isSleep = value; };

protected:
	Vector3 m_position;
	Vector3 m_rotation;
	Vector3 m_scaling;
	Matrix m_matWorld;
	float m_radius;
	float m_mass;
	bool m_isAlive;
	bool m_isVisible;
	bool m_isSleep;
};
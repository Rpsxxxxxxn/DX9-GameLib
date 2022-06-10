//*************************************************************
//
// Vector.hpp
//
// ベクトルクラス
//
// Date: 2019/07/30 更新
//
//*************************************************************

#pragma once

//=============================================================
// インクルード
//=============================================================
#include "Vector.h"
#include "Math.h"

//-------------------------------------------------------------
// 内積の取得
// 引　数：Vector3	1つ目のベクトル
// 　　　：Vector3	2つ目のベクトル
// 戻り値：float	内積
//-------------------------------------------------------------
float VectorCalculate::Dot(Vector3 v1, Vector3 v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

//-------------------------------------------------------------
// 外積の取得
// 引　数：Vector3 １つ目のベクトル
// 　　　：Vector3 ２つ目のベクトル
// 戻り値：Vector3 外積
//-------------------------------------------------------------
Vector3 VectorCalculate::Cross(Vector3 v1, Vector3 v2)
{
	Vector3 w;

	w.x = v1.y * v2.z - v1.z * v2.y;
	w.y = v1.z * v2.x - v1.x * v2.z;
	w.z = v1.x * v2.y - v1.y * v2.x;

	return w;
}

//-------------------------------------------------------------
// ２点の距離を引く
// 引　数：Vector2 １つ目のベクトル
// 　　　：Vector2 ２つ目のベクトル
// 戻り値：Vector2   ２点の距離を引く
//-------------------------------------------------------------
Vector2 VectorCalculate::Subtract(Vector2 v1, Vector2 v2)
{
	return v1 - v2;
}

//-------------------------------------------------------------
// ２点の距離を引く
// 引　数：Vector3 １つ目のベクトル
// 　　　：Vector3 ２つ目のベクトル
// 戻り値：Vector3   ２点の距離を引く
//-------------------------------------------------------------
Vector3 VectorCalculate::Subtract(Vector3 v1, Vector3 v2)
{
	return v1 - v2;
}

//-------------------------------------------------------------
// 長さの取得
// 引　数：Vector2 １つ目のベクトル
// 　　　：Vector2 ２つ目のベクトル
// 戻り値：float   長さ
//-------------------------------------------------------------
float VectorCalculate::Length(Vector2 v1, Vector2 v2)
{
	Vector2 w = Subtract(v2, v1);

	return sqrtf(w.x * w.x + w.y * w.y);
}

//-------------------------------------------------------------
// 長さの取得
// 引　数：Vector3 １つ目のベクトル
// 　　　：Vector3 ２つ目のベクトル
// 戻り値：float   長さ
//-------------------------------------------------------------
float VectorCalculate::Length(Vector3 v1, Vector3 v2)
{
	Vector3 w = Subtract(v2, v1);

	return sqrtf(w.x * w.x + w.y * w.y + w.z * w.z);
}


//-------------------------------------------------------------
// minとmaxの間にする
// 引　数：float 数値
// 　　　：float 最小
// 　　　：float 最大
// 戻り値：float minとmaxの間
//-------------------------------------------------------------
float VectorCalculate::Clamp(float val, float min, float max)
{
	return Mathf::Max(Mathf::Min(val, max), min);
}

//-------------------------------------------------------------
// minとmaxの間にする
// 引　数：Vector2 数値
// 　　　：Vector2 最小
// 　　　：Vector2 最大
// 戻り値：Vector2 minとmaxの間
//-------------------------------------------------------------
Vector2 VectorCalculate::Clamp(Vector2 val, Vector2 vMin, Vector2 vMax)
{
	Vector2 w;

	w.x = Mathf::Max(Mathf::Min(val.x, vMax.x), vMin.x);
	w.y = Mathf::Max(Mathf::Min(val.y, vMax.y), vMin.y);

	return w;
}

//-------------------------------------------------------------
// minとmaxの間にする
// 引　数：Vector3 数値
// 　　　：Vector3 最小
// 　　　：Vector3 最大
// 戻り値：Vector3 minとmaxの間
//-------------------------------------------------------------
Vector3 VectorCalculate::Clamp(Vector3 val, Vector3 vMin, Vector3 vMax)
{
	Vector3 w;

	w.x = Mathf::Max(Mathf::Min(val.x, vMax.x), vMin.x);
	w.y = Mathf::Max(Mathf::Min(val.y, vMax.y), vMin.y);
	w.z = Mathf::Max(Mathf::Min(val.z, vMax.z), vMin.z);

	return w;
}

//-------------------------------------------------------------
// 二つのベクトルを足す
// 引　数：Vector2 一つ目
// 　　　：Vector2 二つ目
// 戻り値：Vector2 二つを足す
//-------------------------------------------------------------
Vector2 VectorCalculate::Add(Vector2 v1, Vector2 v2)
{
	return v1 + v2;
}

//-------------------------------------------------------------
// 二つのベクトルを足す
// 引　数：Vector3 一つ目
// 　　　：Vector3 二つ目
// 戻り値：Vector3 二つを足す
//-------------------------------------------------------------
Vector3 VectorCalculate::Add(Vector3 v1, Vector3 v2)
{
	return v1 + v2;
}

//-------------------------------------------------------------
// ベクトルを正規化
// 引　数：Vector3 一つ目
// 　　　：Vector3 二つ目
// 戻り値：Vector3 二つを足す
//-------------------------------------------------------------
Vector3 VectorCalculate::Normalize(Vector3 v)
{
	Vector3 w;

	D3DXVec3Normalize(&w, &v);

	return w;
}

//-------------------------------------------------------------
// ベクトルの長さ計算
// 引　数：Vector3	平方根
// 戻り値：float	長さを取得
//-------------------------------------------------------------
float VectorCalculate::Magnitude(const Vector2 value)
{
	return sqrtf(value.x * value.x + value.y * value.y);
}

//-------------------------------------------------------------
// ベクトルの長さ計算
// 引　数：const Vector3	平方根
// 戻り値：float			長さを取得
//-------------------------------------------------------------
float VectorCalculate::Magnitude(const Vector3 value)
{
	return sqrtf(value.x * value.x + value.y * value.y + value.z * value.z);
}

//-------------------------------------------------------------
// 線形補間(FLOAT)
// 引　数：const float	現在の数値
// 　　　　const float	目標の数値
// 　　　　const float	遷移速度
// 戻り値：float		現在と目標の間の速度で掛けた数値
//-------------------------------------------------------------
float VectorCalculate::Lerp(const float f1, const float f2, const float t)
{
	return (f1 + (f2 - f1) * t);
}

//-------------------------------------------------------------
// 線形補間(Vector2)
// 引　数：const Vector2	現在の数値
// 　　　　const Vector2	目標の数値
// 　　　　const Vector2	遷移速度
// 戻り値：Vector2			現在と目標の間の速度で掛けた数値
//-------------------------------------------------------------
Vector2 VectorCalculate::Lerp(const Vector2 f1, const Vector2 f2, const float t)
{
	return (f1 + (f2 - f1) * t);
}

//-------------------------------------------------------------
// 線形補間(Vector3)
// 引　数：const Vector3	現在の数値
// 　　　　const Vector3	目標の数値
// 　　　　const Vector3	遷移速度
// 戻り値：float			現在と目標の間の速度で掛けた数値
//-------------------------------------------------------------
Vector3 VectorCalculate::Lerp(const Vector3 f1, const Vector3 f2, const float t)
{
	return (f1 + (f2 - f1) * t);
}

const Vector3 VectorCalculate::Zero = Vector3(0.0f, 0.0f, 0.0f);
const Vector3 VectorCalculate::One = Vector3(1.0f, 1.0f, 1.0f);
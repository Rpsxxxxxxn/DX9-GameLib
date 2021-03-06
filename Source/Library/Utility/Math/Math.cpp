//********************************************************************************
//
// Math.cpp (計算用)
//
//********************************************************************************

//================================================================================
// インクルード
//================================================================================
#include "Math.h"

//--------------------------------------------------------------------------------
// π
//--------------------------------------------------------------------------------
const float Mathf::PI = 3.14159f;

//--------------------------------------------------------------------------------
// 大きいほうを排出
//--------------------------------------------------------------------------------
float Mathf::Max(const float v, const float max)
{
	return v > max ? v : max;
}

//--------------------------------------------------------------------------------
// 小さいほうを排出
//--------------------------------------------------------------------------------
float Mathf::Min(const float v, const float min)
{
	return v < min ? v : min;
}

//--------------------------------------------------------------------------------
// MaxとMinの間に収める
//--------------------------------------------------------------------------------
float Mathf::Clamp(float& v, float max, float min)
{
	v = Mathf::Max(v, max);
	v = Mathf::Min(v, min);
	return v;
}

//--------------------------------------------------------------------------------
// 線形補間
//--------------------------------------------------------------------------------
float Mathf::Lerp(float& from, float& to, float& speed)
{
	return (to - from) + from * speed;
}

//--------------------------------------------------------------------------------
// ラジアン角
//--------------------------------------------------------------------------------
float Mathf::ToRadian(const float degree)
{
	return (degree) * (Mathf::PI / 180.0f);
}

//--------------------------------------------------------------------------------
// デグリー角
//--------------------------------------------------------------------------------
float Mathf::ToDegree(const float radian)
{
	return (radian) * (180.0f / Mathf::PI);
}
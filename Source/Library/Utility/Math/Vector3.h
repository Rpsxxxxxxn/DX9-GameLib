//*************************************************************
//
// Vector3.h
//
//*************************************************************

#pragma once

#include <cmath>
#include <random>

// ユーティリティ ---------------------------------------------
namespace Utility
{
	class Vector3
	{
	public:
		float x;
		float y;
		float z;

		//-------------------------------------------------------------
		// コンストラクタ
		//-------------------------------------------------------------
		Vector3();
		Vector3(float v);
		Vector3(float _x, float _y, float _z);
		~Vector3();

		//-------------------------------------------------------------
		// 定数
		//-------------------------------------------------------------
		static const Vector3 Zero;
		static const Vector3 One;
		static const Vector3 Up;
		static const Vector3 Down;
		static const Vector3 Left;
		static const Vector3 Right;
		static const Vector3 Forward;
		static const Vector3 Back;

		//-------------------------------------------------------------
		// オペレータ
		//-------------------------------------------------------------
		Vector3 operator +(const Vector3 v);
		Vector3 operator +(const float v);
		Vector3 operator -(const Vector3 v);
		Vector3 operator -(const float v);
		Vector3 operator *(const Vector3 v);
		Vector3 operator *(const float v);
		Vector3 operator /(const Vector3 v);
		Vector3 operator /(const float v);

		Vector3& operator +=(const Vector3 v);
		Vector3& operator +=(const float v);
		Vector3& operator -=(const Vector3 v);
		Vector3& operator -=(const float v);
		Vector3& operator *=(const Vector3 v);
		Vector3& operator *=(const float v);
		Vector3& operator /=(const Vector3 v);
		Vector3& operator /=(const float v);

		bool operator ==(const Vector3& v);
		bool operator !=(const Vector3& v);

		//-------------------------------------------------------------
		// 計算
		//-------------------------------------------------------------
		bool	IsNull();
		float	Magnitude();
		float	SquareMagnitude();
		Vector3 Normalized();
		void	Normalize();
		float	Dot(const Vector3& v);
		Vector3 Cross(const Vector3& v);
		static Vector3 Random();
		static Vector3 Random(const float min, const float max);
	};
};
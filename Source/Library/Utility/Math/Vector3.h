//*************************************************************
//
// Vector3.h
//
//*************************************************************

#pragma once

#include <cmath>
#include <random>

// ���[�e�B���e�B ---------------------------------------------
namespace Utility
{
	class Vector3
	{
	public:
		float x;
		float y;
		float z;

		//-------------------------------------------------------------
		// �R���X�g���N�^
		//-------------------------------------------------------------
		Vector3();
		Vector3(float v);
		Vector3(float _x, float _y, float _z);
		~Vector3();

		//-------------------------------------------------------------
		// �萔
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
		// �I�y���[�^
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
		// �v�Z
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
//*************************************************************
//
// Vector.hpp
//
// �x�N�g���N���X
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

#pragma once

//=============================================================
// �C���N���[�h
//=============================================================
#include "Vector.h"
#include "Math.h"

//-------------------------------------------------------------
// ���ς̎擾
// ���@���FVector3	1�ڂ̃x�N�g��
// �@�@�@�FVector3	2�ڂ̃x�N�g��
// �߂�l�Ffloat	����
//-------------------------------------------------------------
float VectorCalculate::Dot(Vector3 v1, Vector3 v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

//-------------------------------------------------------------
// �O�ς̎擾
// ���@���FVector3 �P�ڂ̃x�N�g��
// �@�@�@�FVector3 �Q�ڂ̃x�N�g��
// �߂�l�FVector3 �O��
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
// �Q�_�̋���������
// ���@���FVector2 �P�ڂ̃x�N�g��
// �@�@�@�FVector2 �Q�ڂ̃x�N�g��
// �߂�l�FVector2   �Q�_�̋���������
//-------------------------------------------------------------
Vector2 VectorCalculate::Subtract(Vector2 v1, Vector2 v2)
{
	return v1 - v2;
}

//-------------------------------------------------------------
// �Q�_�̋���������
// ���@���FVector3 �P�ڂ̃x�N�g��
// �@�@�@�FVector3 �Q�ڂ̃x�N�g��
// �߂�l�FVector3   �Q�_�̋���������
//-------------------------------------------------------------
Vector3 VectorCalculate::Subtract(Vector3 v1, Vector3 v2)
{
	return v1 - v2;
}

//-------------------------------------------------------------
// �����̎擾
// ���@���FVector2 �P�ڂ̃x�N�g��
// �@�@�@�FVector2 �Q�ڂ̃x�N�g��
// �߂�l�Ffloat   ����
//-------------------------------------------------------------
float VectorCalculate::Length(Vector2 v1, Vector2 v2)
{
	Vector2 w = Subtract(v2, v1);

	return sqrtf(w.x * w.x + w.y * w.y);
}

//-------------------------------------------------------------
// �����̎擾
// ���@���FVector3 �P�ڂ̃x�N�g��
// �@�@�@�FVector3 �Q�ڂ̃x�N�g��
// �߂�l�Ffloat   ����
//-------------------------------------------------------------
float VectorCalculate::Length(Vector3 v1, Vector3 v2)
{
	Vector3 w = Subtract(v2, v1);

	return sqrtf(w.x * w.x + w.y * w.y + w.z * w.z);
}


//-------------------------------------------------------------
// min��max�̊Ԃɂ���
// ���@���Ffloat ���l
// �@�@�@�Ffloat �ŏ�
// �@�@�@�Ffloat �ő�
// �߂�l�Ffloat min��max�̊�
//-------------------------------------------------------------
float VectorCalculate::Clamp(float val, float min, float max)
{
	return Mathf::Max(Mathf::Min(val, max), min);
}

//-------------------------------------------------------------
// min��max�̊Ԃɂ���
// ���@���FVector2 ���l
// �@�@�@�FVector2 �ŏ�
// �@�@�@�FVector2 �ő�
// �߂�l�FVector2 min��max�̊�
//-------------------------------------------------------------
Vector2 VectorCalculate::Clamp(Vector2 val, Vector2 vMin, Vector2 vMax)
{
	Vector2 w;

	w.x = Mathf::Max(Mathf::Min(val.x, vMax.x), vMin.x);
	w.y = Mathf::Max(Mathf::Min(val.y, vMax.y), vMin.y);

	return w;
}

//-------------------------------------------------------------
// min��max�̊Ԃɂ���
// ���@���FVector3 ���l
// �@�@�@�FVector3 �ŏ�
// �@�@�@�FVector3 �ő�
// �߂�l�FVector3 min��max�̊�
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
// ��̃x�N�g���𑫂�
// ���@���FVector2 ���
// �@�@�@�FVector2 ���
// �߂�l�FVector2 ��𑫂�
//-------------------------------------------------------------
Vector2 VectorCalculate::Add(Vector2 v1, Vector2 v2)
{
	return v1 + v2;
}

//-------------------------------------------------------------
// ��̃x�N�g���𑫂�
// ���@���FVector3 ���
// �@�@�@�FVector3 ���
// �߂�l�FVector3 ��𑫂�
//-------------------------------------------------------------
Vector3 VectorCalculate::Add(Vector3 v1, Vector3 v2)
{
	return v1 + v2;
}

//-------------------------------------------------------------
// �x�N�g���𐳋K��
// ���@���FVector3 ���
// �@�@�@�FVector3 ���
// �߂�l�FVector3 ��𑫂�
//-------------------------------------------------------------
Vector3 VectorCalculate::Normalize(Vector3 v)
{
	Vector3 w;

	D3DXVec3Normalize(&w, &v);

	return w;
}

//-------------------------------------------------------------
// �x�N�g���̒����v�Z
// ���@���FVector3	������
// �߂�l�Ffloat	�������擾
//-------------------------------------------------------------
float VectorCalculate::Magnitude(const Vector2 value)
{
	return sqrtf(value.x * value.x + value.y * value.y);
}

//-------------------------------------------------------------
// �x�N�g���̒����v�Z
// ���@���Fconst Vector3	������
// �߂�l�Ffloat			�������擾
//-------------------------------------------------------------
float VectorCalculate::Magnitude(const Vector3 value)
{
	return sqrtf(value.x * value.x + value.y * value.y + value.z * value.z);
}

//-------------------------------------------------------------
// ���`���(FLOAT)
// ���@���Fconst float	���݂̐��l
// �@�@�@�@const float	�ڕW�̐��l
// �@�@�@�@const float	�J�ڑ��x
// �߂�l�Ffloat		���݂ƖڕW�̊Ԃ̑��x�Ŋ|�������l
//-------------------------------------------------------------
float VectorCalculate::Lerp(const float f1, const float f2, const float t)
{
	return (f1 + (f2 - f1) * t);
}

//-------------------------------------------------------------
// ���`���(Vector2)
// ���@���Fconst Vector2	���݂̐��l
// �@�@�@�@const Vector2	�ڕW�̐��l
// �@�@�@�@const Vector2	�J�ڑ��x
// �߂�l�FVector2			���݂ƖڕW�̊Ԃ̑��x�Ŋ|�������l
//-------------------------------------------------------------
Vector2 VectorCalculate::Lerp(const Vector2 f1, const Vector2 f2, const float t)
{
	return (f1 + (f2 - f1) * t);
}

//-------------------------------------------------------------
// ���`���(Vector3)
// ���@���Fconst Vector3	���݂̐��l
// �@�@�@�@const Vector3	�ڕW�̐��l
// �@�@�@�@const Vector3	�J�ڑ��x
// �߂�l�Ffloat			���݂ƖڕW�̊Ԃ̑��x�Ŋ|�������l
//-------------------------------------------------------------
Vector3 VectorCalculate::Lerp(const Vector3 f1, const Vector3 f2, const float t)
{
	return (f1 + (f2 - f1) * t);
}

const Vector3 VectorCalculate::Zero = Vector3(0.0f, 0.0f, 0.0f);
const Vector3 VectorCalculate::One = Vector3(1.0f, 1.0f, 1.0f);
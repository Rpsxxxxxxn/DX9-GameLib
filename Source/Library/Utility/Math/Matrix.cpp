//********************************************************************************
//
// Matrix.h
//
//********************************************************************************

//================================================================================
// �C���N���[�h
//================================================================================
#include "Matrix.h"

//--------------------------------------------------------------------------------
// �X�P�[���s��̌v�Z
//--------------------------------------------------------------------------------
Matrix MatrixCalculate::Scaling(const Vector3& scaling)
{
	Matrix result;
	D3DXMatrixScaling(&result, scaling.x, scaling.y, scaling.z);
	return result;
}

//--------------------------------------------------------------------------------
// ��]�s��̌v�Z
//--------------------------------------------------------------------------------
Matrix MatrixCalculate::RotationYawPitchRoll(const Vector3& euler)
{
	Matrix result;
	D3DXMatrixRotationYawPitchRoll(&result, euler.y, euler.x, euler.z);
	return result;
}

//--------------------------------------------------------------------------------
// ���s�ړ��s��̌v�Z
//--------------------------------------------------------------------------------
Matrix MatrixCalculate::Translation(const Vector3& trans)
{
	Matrix result;
	D3DXMatrixTranslation(&result, trans.x, trans.y, trans.z);
	return result;
}

//--------------------------------------------------------------------------------
// �X�P�[��*��]*�ړ��s��̌v�Z
// ���@���FVector3	�X�P�[��
// �@�@�@�@Vector3	��]
// �@�@�@�@Vector3	�ʒu
// �߂�l�FMatrix	���E�s��
//--------------------------------------------------------------------------------
Matrix MatrixCalculate::Transform(
	const Vector3& scaling, 
	const Vector3& rotation, 
	const Vector3& position)
{
	Matrix result = Scaling(scaling);
	result *= RotationYawPitchRoll(rotation);
	result *= Translation(position);
	return result;
}

//--------------------------------------------------------------------------------
// ������
//--------------------------------------------------------------------------------
const Matrix MatrixCalculate::Identity = Matrix(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
);

//--------------------------------------------------------------------------------
// Zero
//--------------------------------------------------------------------------------
const Matrix MatrixCalculate::Zero = Matrix(
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f
);
//********************************************************************************
//
// Matrix.h
//
//********************************************************************************

//================================================================================
// インクルード
//================================================================================
#include "Matrix.h"

//--------------------------------------------------------------------------------
// スケール行列の計算
//--------------------------------------------------------------------------------
Matrix MatrixCalculate::Scaling(const Vector3& scaling)
{
	Matrix result;
	D3DXMatrixScaling(&result, scaling.x, scaling.y, scaling.z);
	return result;
}

//--------------------------------------------------------------------------------
// 回転行列の計算
//--------------------------------------------------------------------------------
Matrix MatrixCalculate::RotationYawPitchRoll(const Vector3& euler)
{
	Matrix result;
	D3DXMatrixRotationYawPitchRoll(&result, euler.y, euler.x, euler.z);
	return result;
}

//--------------------------------------------------------------------------------
// 平行移動行列の計算
//--------------------------------------------------------------------------------
Matrix MatrixCalculate::Translation(const Vector3& trans)
{
	Matrix result;
	D3DXMatrixTranslation(&result, trans.x, trans.y, trans.z);
	return result;
}

//--------------------------------------------------------------------------------
// スケール*回転*移動行列の計算
// 引　数：Vector3	スケール
// 　　　　Vector3	回転
// 　　　　Vector3	位置
// 戻り値：Matrix	世界行列
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
// 初期化
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
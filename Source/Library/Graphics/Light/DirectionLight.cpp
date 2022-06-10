//*************************************************************
//
// DirectionLight.cpp
//
// 方向ライトクラス
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "DirectionLight.h"

//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
CDirectionLight::CDirectionLight()
	: m_vecLookAt(Vector3(0.0f, 0.0f, 0.0f))
	, m_vecUp(Vector3(0.0f, 1.0f, 0.0f))
	, m_vecPos(Vector3(40.0f, 40.0f, 40.0f))
	, m_vecDir(Vector3(0.0f, 0.0f, 0.0f))
	, m_diffuse(Vector4(1.0f, 1.0f, 1.0f, 0.0f))
	, m_ambient(Vector4(1.0f, 1.0f, 1.0f, 0.0f))
	, m_specular(Vector4(1.0f, 1.0f, 1.0f, 0.0f))
{
	Vector3 dir = m_vecDir - m_vecPos;
	D3DXVec3Normalize(&m_vecDir, &dir);

	m_matBias = D3DXMATRIX(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.00001f, 1.0f);
}

//-------------------------------------------------------------
// ライト位置設定
// 引　数：const Vector3	位置の設定
//-------------------------------------------------------------
void CDirectionLight::SetPosition(const Vector3 vecPos)
{
	m_vecPos = vecPos;
}

//-------------------------------------------------------------
// ライト方向の設定
// 引　数：const Vector3	ライトの方向
//-------------------------------------------------------------
void CDirectionLight::SetDirection(const Vector3 vecDir)
{
	m_vecDir = vecDir;
}

//-------------------------------------------------------------
// ライト位置の取得
// 戻り値：const Vector3	位置の設定
//-------------------------------------------------------------
void CDirectionLight::SetProjection()
{
	m_vecLookAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_matView, &m_vecPos, &m_vecLookAt, &m_vecUp);
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(45), 1.0f, 0.1f, 200.0f);
}

//-------------------------------------------------------------
// ライト位置の取得
// 戻り値：const Vector3	位置の設定
//-------------------------------------------------------------
void CDirectionLight::SetOrtho()
{
	m_vecLookAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_matView, &m_vecPos, &m_vecLookAt, &m_vecUp);
	D3DXMatrixOrthoLH(&m_matProj, 32, 32, 0.1f, 200.0f);
}

//-------------------------------------------------------------
// ライト位置の取得
// 戻り値：const Vector3	位置の設定
//-------------------------------------------------------------
Vector3 CDirectionLight::GetPosition() const
{
	return m_vecPos;
}

//-------------------------------------------------------------
// ライト方向の取得
// 戻り値：const Vector3	ライトの方向
//-------------------------------------------------------------
Vector3 CDirectionLight::GetDirection() const
{
	return m_vecDir;
}

//-------------------------------------------------------------
// シャドウ調整用行列の取得
// 戻り値：D3DXMATRIX	ライト調整用
//-------------------------------------------------------------
Matrix CDirectionLight::GetLightBias() const
{
	return m_matBias;
}

//-------------------------------------------------------------
// シャドウ用ビュー行列の取得
// 戻り値：D3DXMATRIX	ライト調整用
//-------------------------------------------------------------
Matrix CDirectionLight::GetViewMatrix() const
{
	return m_matView;
}

//-------------------------------------------------------------
// シャドウ用プロジェクション行列の取得
// 戻り値：D3DXMATRIX	ライト調整用
//-------------------------------------------------------------
Matrix CDirectionLight::GetProjectionMatrix() const
{
	return m_matProj;
}

//*************************************************************
//
// Camera.cpp
//
// カメラ専用クラス
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "Camera.h"

//=============================================================
// クラス定義
//=============================================================

//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
CCamera::CCamera()
	: m_pDevice(nullptr)
	, m_vecEye(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, m_vecLookAt(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, m_vecUp(D3DXVECTOR3(0.0f, 1.0f, 0.0f))
	, m_vecForward(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, m_vecRight(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, m_fov(0.0f)
	, m_aspect(0.0f)
	, m_near(0.0f)
	, m_far(0.0f)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProjection);
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CCamera::~CCamera()
{
}

//-------------------------------------------------------------
// 初期化
// 引　数：LPDIRECT3DDEVICE9	デバイスのポインタ
// 　　　　const float			視野角
// 　　　　const float			アスペクト比
// 　　　　const float			クリップ面(近い)
// 　　　　const float			クリップ面(遠い)
//-------------------------------------------------------------
void CCamera::Initialize(
	LPDIRECT3DDEVICE9 pDevice, 
	const float fov, 
	const float aspect, 
	const float clipNear, 
	const float clipFar
)
{
	m_pDevice = pDevice;

	m_fov = fov;
	m_aspect = aspect;
	m_near = clipNear;
	m_far = clipFar;
}

//-------------------------------------------------------------
// 解放
//-------------------------------------------------------------
void CCamera::ShutDown()
{
}

//-------------------------------------------------------------
// サードパーソンカメラの設定
// 引　数：const Vector3	注目点
// 　　　　const Vector3	回転角度
// 　　　　const float		注目点から横に動かした距離
// 　　　　const float		注目点から後ろに引き離した距離
//-------------------------------------------------------------
void CCamera::SetThirdPerson(
	const Vector3 vecLookAt,
	const Vector3 vecAngle,
	const float lookAtHori,
	const float lookAtBehind
)
{
	Matrix matRotate; Vector3 vecBehind, vecLookAtRotate;
	Vector3 vecLookAtBehind(0.0f, 0.0f, lookAtBehind);
	Vector3 vecLookAtHori(0.0f, 0.0f, lookAtHori);

	D3DXMatrixRotationYawPitchRoll(&matRotate, vecAngle.y, vecAngle.x, vecAngle.z);
	D3DXVec3TransformCoord(&vecBehind, &vecLookAtBehind, &matRotate);
	vecLookAtRotate = vecBehind;

	float atAngle = vecAngle.y - D3DX_PI * 0.5f;
	D3DXMatrixRotationY(&matRotate, atAngle);
	D3DXVec3TransformCoord(&vecBehind, &vecLookAtHori, &matRotate);
	Vector3 vecLookAtAngle = vecBehind;

	m_vecEye = vecLookAt + vecLookAtAngle + vecLookAtRotate;
	m_vecLookAt = vecLookAt + vecLookAtAngle;
	m_vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	RecalculateUpDirection();
	RecalculateFrustumPlane();
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

//-------------------------------------------------------------
// ビュー行列の取得
// 戻り値：Matrix	ビュー行列
// 備　考：シェーダ用
//-------------------------------------------------------------
Matrix CCamera::GetViewMatrix() const
{
	return m_matView;
}

//-------------------------------------------------------------
// 逆ビュー行列の取得
// 戻り値：Matrix	逆ビュー行列
// 備　考：ビルボード用
//-------------------------------------------------------------
Matrix CCamera::GetInvViewMatrix() const
{
	Matrix inverse = m_matView;

	D3DXMatrixInverse(&inverse, NULL, &inverse);

	return inverse;
}

//-------------------------------------------------------------
// 投影行列の取得
// 戻り値：Matrix	投影行列
// 備　考：シェーダ用
//-------------------------------------------------------------
Matrix CCamera::GetProjectionMatrix() const
{
	return m_matProjection;
}

//-------------------------------------------------------------
// 視錘台の６点板の取得
// 戻り値：D3DXPLANE	６つの板の一枚
// 備　考：カリング用
//-------------------------------------------------------------
D3DXPLANE CCamera::GetFrustumPlane(const int index) const
{
	return m_frustum[index];
}

//-------------------------------------------------------------
// 視野角の取得
// 戻り値：float		視野角
//-------------------------------------------------------------
float CCamera::GetFov() const
{
	return m_fov;
}

//-------------------------------------------------------------
// 視野角の設定
// 引　数：const float	視野角(ラジアン)
//-------------------------------------------------------------
void CCamera::SetFov(const float fov)
{
	m_fov = fov;
}

//-------------------------------------------------------------
// 上方向の再計算
//-------------------------------------------------------------
void CCamera::RecalculateUpDirection()
{
	// 前方向計算
	m_vecForward = m_vecLookAt - m_vecEye;
	D3DXVec3Normalize(&m_vecForward, &m_vecForward);

	// 右方向計算
	D3DXVec3Cross(&m_vecRight, &m_vecForward, &m_vecUp);
	D3DXVec3Normalize(&m_vecRight, &m_vecRight);

	// 上方向計算
	D3DXVec3Cross(&m_vecUp, &m_vecRight, &m_vecForward);
	D3DXVec3Normalize(&m_vecUp, &m_vecUp);
}

//-------------------------------------------------------------
// 視錘台の再計算
//-------------------------------------------------------------
void CCamera::RecalculateFrustumPlane()
{
	Matrix mViewProj;
	D3DXMatrixMultiply(&mViewProj, &m_matView, &m_matProjection);

	m_frustum[PLANE_LEFT].a = mViewProj._14 + mViewProj._11;
	m_frustum[PLANE_LEFT].b = mViewProj._24 + mViewProj._21;
	m_frustum[PLANE_LEFT].c = mViewProj._34 + mViewProj._31;
	m_frustum[PLANE_LEFT].d = mViewProj._44 + mViewProj._41;

	m_frustum[PLANE_RIGHT].a = mViewProj._14 - mViewProj._11;
	m_frustum[PLANE_RIGHT].b = mViewProj._24 - mViewProj._21;
	m_frustum[PLANE_RIGHT].c = mViewProj._34 - mViewProj._31;
	m_frustum[PLANE_RIGHT].d = mViewProj._44 - mViewProj._41;

	m_frustum[PLANE_UP].a = mViewProj._14 - mViewProj._12;
	m_frustum[PLANE_UP].b = mViewProj._24 - mViewProj._22;
	m_frustum[PLANE_UP].c = mViewProj._34 - mViewProj._32;
	m_frustum[PLANE_UP].d = mViewProj._44 - mViewProj._42;

	m_frustum[PLANE_DOWN].a = mViewProj._14 + mViewProj._12;
	m_frustum[PLANE_DOWN].b = mViewProj._24 + mViewProj._22;
	m_frustum[PLANE_DOWN].c = mViewProj._34 + mViewProj._32;
	m_frustum[PLANE_DOWN].d = mViewProj._44 + mViewProj._42;

	m_frustum[PLANE_NEAR].a = mViewProj._13;
	m_frustum[PLANE_NEAR].b = mViewProj._23;
	m_frustum[PLANE_NEAR].c = mViewProj._33;
	m_frustum[PLANE_NEAR].d = mViewProj._43;

	m_frustum[PLANE_FAR].a = mViewProj._14 - mViewProj._13;
	m_frustum[PLANE_FAR].b = mViewProj._24 - mViewProj._23;
	m_frustum[PLANE_FAR].c = mViewProj._34 - mViewProj._33;
	m_frustum[PLANE_FAR].d = mViewProj._44 - mViewProj._43;

	//正規化
	for (int i = 0; i < 6; i++)
	{
		D3DXPlaneNormalize(&m_frustum[i], &m_frustum[i]);
	}
}

//-------------------------------------------------------------
// ビュー行列の更新とデバイスへのセット
//-------------------------------------------------------------
void CCamera::UpdateViewMatrix()
{
	D3DXMatrixLookAtLH(&m_matView, &m_vecEye, &m_vecLookAt, &m_vecUp);
	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
}

//-------------------------------------------------------------
// 射影行列の更新とデバイスへのセット
//-------------------------------------------------------------
void CCamera::UpdateProjectionMatrix()
{
	D3DXMatrixPerspectiveFovLH(&m_matProjection, m_fov, m_aspect, m_near, m_far);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProjection);
}

//*************************************************************
//
// Camera.cpp
//
// �J������p�N���X
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "Camera.h"

//=============================================================
// �N���X��`
//=============================================================

//-------------------------------------------------------------
// �R���X�g���N�^
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
// �f�X�g���N�^
//-------------------------------------------------------------
CCamera::~CCamera()
{
}

//-------------------------------------------------------------
// ������
// ���@���FLPDIRECT3DDEVICE9	�f�o�C�X�̃|�C���^
// �@�@�@�@const float			����p
// �@�@�@�@const float			�A�X�y�N�g��
// �@�@�@�@const float			�N���b�v��(�߂�)
// �@�@�@�@const float			�N���b�v��(����)
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
// ���
//-------------------------------------------------------------
void CCamera::ShutDown()
{
}

//-------------------------------------------------------------
// �T�[�h�p�[�\���J�����̐ݒ�
// ���@���Fconst Vector3	���ړ_
// �@�@�@�@const Vector3	��]�p�x
// �@�@�@�@const float		���ړ_���牡�ɓ�����������
// �@�@�@�@const float		���ړ_������Ɉ�������������
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
// �r���[�s��̎擾
// �߂�l�FMatrix	�r���[�s��
// ���@�l�F�V�F�[�_�p
//-------------------------------------------------------------
Matrix CCamera::GetViewMatrix() const
{
	return m_matView;
}

//-------------------------------------------------------------
// �t�r���[�s��̎擾
// �߂�l�FMatrix	�t�r���[�s��
// ���@�l�F�r���{�[�h�p
//-------------------------------------------------------------
Matrix CCamera::GetInvViewMatrix() const
{
	Matrix inverse = m_matView;

	D3DXMatrixInverse(&inverse, NULL, &inverse);

	return inverse;
}

//-------------------------------------------------------------
// ���e�s��̎擾
// �߂�l�FMatrix	���e�s��
// ���@�l�F�V�F�[�_�p
//-------------------------------------------------------------
Matrix CCamera::GetProjectionMatrix() const
{
	return m_matProjection;
}

//-------------------------------------------------------------
// ������̂U�_�̎擾
// �߂�l�FD3DXPLANE	�U�̔̈ꖇ
// ���@�l�F�J�����O�p
//-------------------------------------------------------------
D3DXPLANE CCamera::GetFrustumPlane(const int index) const
{
	return m_frustum[index];
}

//-------------------------------------------------------------
// ����p�̎擾
// �߂�l�Ffloat		����p
//-------------------------------------------------------------
float CCamera::GetFov() const
{
	return m_fov;
}

//-------------------------------------------------------------
// ����p�̐ݒ�
// ���@���Fconst float	����p(���W�A��)
//-------------------------------------------------------------
void CCamera::SetFov(const float fov)
{
	m_fov = fov;
}

//-------------------------------------------------------------
// ������̍Čv�Z
//-------------------------------------------------------------
void CCamera::RecalculateUpDirection()
{
	// �O�����v�Z
	m_vecForward = m_vecLookAt - m_vecEye;
	D3DXVec3Normalize(&m_vecForward, &m_vecForward);

	// �E�����v�Z
	D3DXVec3Cross(&m_vecRight, &m_vecForward, &m_vecUp);
	D3DXVec3Normalize(&m_vecRight, &m_vecRight);

	// ������v�Z
	D3DXVec3Cross(&m_vecUp, &m_vecRight, &m_vecForward);
	D3DXVec3Normalize(&m_vecUp, &m_vecUp);
}

//-------------------------------------------------------------
// ������̍Čv�Z
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

	//���K��
	for (int i = 0; i < 6; i++)
	{
		D3DXPlaneNormalize(&m_frustum[i], &m_frustum[i]);
	}
}

//-------------------------------------------------------------
// �r���[�s��̍X�V�ƃf�o�C�X�ւ̃Z�b�g
//-------------------------------------------------------------
void CCamera::UpdateViewMatrix()
{
	D3DXMatrixLookAtLH(&m_matView, &m_vecEye, &m_vecLookAt, &m_vecUp);
	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
}

//-------------------------------------------------------------
// �ˉe�s��̍X�V�ƃf�o�C�X�ւ̃Z�b�g
//-------------------------------------------------------------
void CCamera::UpdateProjectionMatrix()
{
	D3DXMatrixPerspectiveFovLH(&m_matProjection, m_fov, m_aspect, m_near, m_far);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProjection);
}

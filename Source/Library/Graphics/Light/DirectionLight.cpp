//*************************************************************
//
// DirectionLight.cpp
//
// �������C�g�N���X
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "DirectionLight.h"

//-------------------------------------------------------------
// �R���X�g���N�^
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
// ���C�g�ʒu�ݒ�
// ���@���Fconst Vector3	�ʒu�̐ݒ�
//-------------------------------------------------------------
void CDirectionLight::SetPosition(const Vector3 vecPos)
{
	m_vecPos = vecPos;
}

//-------------------------------------------------------------
// ���C�g�����̐ݒ�
// ���@���Fconst Vector3	���C�g�̕���
//-------------------------------------------------------------
void CDirectionLight::SetDirection(const Vector3 vecDir)
{
	m_vecDir = vecDir;
}

//-------------------------------------------------------------
// ���C�g�ʒu�̎擾
// �߂�l�Fconst Vector3	�ʒu�̐ݒ�
//-------------------------------------------------------------
void CDirectionLight::SetProjection()
{
	m_vecLookAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_matView, &m_vecPos, &m_vecLookAt, &m_vecUp);
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(45), 1.0f, 0.1f, 200.0f);
}

//-------------------------------------------------------------
// ���C�g�ʒu�̎擾
// �߂�l�Fconst Vector3	�ʒu�̐ݒ�
//-------------------------------------------------------------
void CDirectionLight::SetOrtho()
{
	m_vecLookAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_matView, &m_vecPos, &m_vecLookAt, &m_vecUp);
	D3DXMatrixOrthoLH(&m_matProj, 32, 32, 0.1f, 200.0f);
}

//-------------------------------------------------------------
// ���C�g�ʒu�̎擾
// �߂�l�Fconst Vector3	�ʒu�̐ݒ�
//-------------------------------------------------------------
Vector3 CDirectionLight::GetPosition() const
{
	return m_vecPos;
}

//-------------------------------------------------------------
// ���C�g�����̎擾
// �߂�l�Fconst Vector3	���C�g�̕���
//-------------------------------------------------------------
Vector3 CDirectionLight::GetDirection() const
{
	return m_vecDir;
}

//-------------------------------------------------------------
// �V���h�E�����p�s��̎擾
// �߂�l�FD3DXMATRIX	���C�g�����p
//-------------------------------------------------------------
Matrix CDirectionLight::GetLightBias() const
{
	return m_matBias;
}

//-------------------------------------------------------------
// �V���h�E�p�r���[�s��̎擾
// �߂�l�FD3DXMATRIX	���C�g�����p
//-------------------------------------------------------------
Matrix CDirectionLight::GetViewMatrix() const
{
	return m_matView;
}

//-------------------------------------------------------------
// �V���h�E�p�v���W�F�N�V�����s��̎擾
// �߂�l�FD3DXMATRIX	���C�g�����p
//-------------------------------------------------------------
Matrix CDirectionLight::GetProjectionMatrix() const
{
	return m_matProj;
}

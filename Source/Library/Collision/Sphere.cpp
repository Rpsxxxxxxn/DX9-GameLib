//*************************************************************
//
// BSphere.cpp
//
// �����蔻��N���X(�X�t�B�A)
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "Sphere.h"

//-------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------
CSphere::CSphere()
	: m_radius(0)
	, m_pMesh(nullptr)
{
	D3DXMatrixIdentity(&m_matWorld);
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
CSphere::~CSphere()
{
	if (m_pMesh != nullptr) 
		m_pMesh->Release();
}

//-------------------------------------------------------------
// ������
// ���@���FCSystem* pSystem		�V�X�e���N���X�̃|�C���^
// �@�@�@�@const float			���a
// �@�@�@�@const bool			�f�o�b�O�p�`��
//-------------------------------------------------------------
void CSphere::Create(
	CSystem * pSystem, 
	const float radius, 
	const bool showDebug
)
{
	m_radius = radius;

	if (showDebug)
	{
		LPDIRECT3DDEVICE9 pDevice = pSystem->GetDevice();

		HRESULT hr = D3DXCreateSphere(
			pDevice,
			radius,
			24,
			24,
			&m_pMesh,
			nullptr);

		if (FAILED(hr)) 
			return;
	}
}

//-------------------------------------------------------------
// �`��
// ���@���FCSystem* pSystem		�V�X�e���N���X�̃|�C���^
//-------------------------------------------------------------
void CSphere::Draw(CSystem * pSystem)
{
	if (m_pMesh)
	{
		m_pMesh->DrawSubset(0);
	}
}

//-------------------------------------------------------------
// ���E�s��̐ݒ�
// ���@���Fconst Matrix*		���E�s��
//-------------------------------------------------------------
void CSphere::SetTransform(const Matrix * pMatWorld)
{
	m_matWorld = *pMatWorld;
}

//-------------------------------------------------------------
// ���E�s��̎擾
// �߂�l�Fconst float			���E�s��
//-------------------------------------------------------------
Matrix CSphere::GetTransform() const
{
	return m_matWorld;
}

//-------------------------------------------------------------
// ���a�̎擾
// �߂�l�Fconst float			���a
//-------------------------------------------------------------
float CSphere::GetRadius() const
{
	return m_radius;
}

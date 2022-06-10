//*************************************************************
//
// ShaderBase.cpp
//
// �V�F�[�_�N���X(���)
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "ShaderBase.h"

//-------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------
CShaderBase::CShaderBase()
	: m_pDevice(nullptr)
	, m_pEffect(nullptr)
{
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
CShaderBase::~CShaderBase()
{
	if (m_pEffect)
	{
		m_pEffect->Release();
		m_pEffect = nullptr;
	}
}

//-------------------------------------------------------------
// ������
//-------------------------------------------------------------
void CShaderBase::Initialize(
	LPDIRECT3DDEVICE9 pDevice,
	const std::string path
)
{
	m_pDevice = pDevice;

	//�G�t�F�N�g�ǂݍ���
	D3DXCreateEffectFromFile(
		m_pDevice, 
		(LPCSTR)path.c_str(),
		NULL,
		NULL, 
		D3DXSHADER_DEBUG,
		NULL,
		&m_pEffect,
		NULL);

	if (m_pEffect != nullptr)
		m_pEffect->OnResetDevice();
}

//-------------------------------------------------------------
// �G�t�F�N�g�̎擾
// �߂�l�FLPD3DXEFFECT			�G�t�F�N�g
//-------------------------------------------------------------
LPD3DXEFFECT CShaderBase::GetEffect()
{
	return m_pEffect;
}

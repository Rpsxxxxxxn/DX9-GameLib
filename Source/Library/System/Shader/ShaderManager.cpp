//*************************************************************
//
// ShaderManager.cpp
//
// �V�F�[�_�N���X�Ǘ�
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "ShaderManager.h"

//-------------------------------------------------------------
// ������
// ���@���FCWindowSystem*	�E�B���h�E�V�X�e���N���X�̃|�C���^
//-------------------------------------------------------------
void CShaderManager::Initialize(
	LPDIRECT3DDEVICE9 pDevice,
	CWindowSystem * pWindowSystem
)
{
	m_pDevice = pDevice;

	auto& ws_ = pWindowSystem->GetWindowSettings();

	auto* pDepth = new CShadowDepth(ws_.Width, ws_.Height);
	pDepth->Initialize(pDevice);
	this->AddShader(pDepth);

	auto* pShadow = new CShadowMap;
	pShadow->Initialize(pDevice);
	this->AddShader(pShadow);

	auto* pDefault = new CDefaultShader;
	pDefault->Initialize(pDevice);
	this->AddShader(pDefault);
}

//-------------------------------------------------------------
// �X�V
//-------------------------------------------------------------
void CShaderManager::Update()
{
	for (auto itr = m_vecShader.begin(); itr != m_vecShader.end(); ++itr)
	{
		(*itr)->Update();
		(*itr)->SetValue("g_vLDir", m_vecLightDir);
		(*itr)->SetValue("g_vEyePos", m_vecCameraEye);
		(*itr)->SetValue("g_mView", m_matView);
		(*itr)->SetValue("g_mProj", m_matProj);
		(*itr)->SetValue("g_mLBias", m_matLBias);
		(*itr)->SetValue("g_mLVP", m_matLViewProj, 3);
	}
}

//-------------------------------------------------------------
// �V�F�[�_�̒ǉ�
//-------------------------------------------------------------
void CShaderManager::AddShader(CShaderBase* pShader)
{
	m_vecShader.push_back(pShader);
}

//-------------------------------------------------------------
// �V�F�[�_�̕`��ݒ�
// ���@���FShaderType �V�F�[�_�̎��
// ���@�l�F���b�V���̕`��O�ɌĂяo�������Ă��������B
//-------------------------------------------------------------
void CShaderManager::SetShaderState(Shader Value)
{
	m_nowShaderState = Value;
}

//-------------------------------------------------------------
// ���݂̃V�F�[�_��Ԃ̎擾
// �߂�l�FShaderType �V�F�[�_�̎��
// ���@�l�F�`��O�ɐݒ�ύX�̂��ߌĂяo���B
//-------------------------------------------------------------
Shader CShaderManager::GetShaderState() const
{
	return m_nowShaderState;
}

//-------------------------------------------------------------
// �V�F�[�_�̎擾
// ���@���Fconst int 
//-------------------------------------------------------------
CShaderBase* CShaderManager::GetShader(const UINT index)
{
	if (index >= m_vecShader.size()) return nullptr;

	return m_vecShader[index];
}

//-------------------------------------------------------------
// �V�F�[�_�̃p�����[�^�ݒ�
// ���@���FD3DXMATRIX* �r���[�s��̐ݒ� 
//-------------------------------------------------------------
void CShaderManager::SetMatView(const Matrix& pMatView)
{
	m_matView = pMatView;
}

//-------------------------------------------------------------
// �V�F�[�_�̃p�����[�^�ݒ�
// ���@���FD3DXMATRIX* �v���W�F�N�V�����s��̐ݒ� 
//-------------------------------------------------------------
void CShaderManager::SetMatProj(const Matrix& pMatProj)
{
	m_matProj = pMatProj;
}

//-------------------------------------------------------------
// �V�F�[�_�̃p�����[�^�ݒ�
// ���@���FD3DXMATRIX* ���C�g�r���[�s��̐ݒ� 
//-------------------------------------------------------------
void CShaderManager::SetMatLView(const Matrix& pMatView)
{
	m_matLView = pMatView;
}

//-------------------------------------------------------------
// �V�F�[�_�̃p�����[�^�ݒ�
// ���@���FD3DXMATRIX* �v���W�F�N�V�����s��̐ݒ� 
//-------------------------------------------------------------
void CShaderManager::SetMatLProj(const Matrix& pMatProj)
{
	m_matLProj = pMatProj;
}

//-------------------------------------------------------------
// �V�F�[�_�̃p�����[�^�ݒ�
// ���@���FD3DXMATRIX* ���C�g�r���[�v���W�F�N�V�����s��̐ݒ� 
//-------------------------------------------------------------
void CShaderManager::SetMatLViewProj(Matrix* pMatViewProj)
{
	m_matLViewProj = pMatViewProj;
}

//-------------------------------------------------------------
// �V�F�[�_�̃p�����[�^�ݒ�
// ���@���FD3DXMATRIX* �V���h�E�������p�s��
//-------------------------------------------------------------
void CShaderManager::SetMatLBias(const Matrix& pMatProj)
{
	m_matLBias = pMatProj;
}

//-------------------------------------------------------------
// �V�F�[�_�̃p�����[�^�ݒ�
// ���@���FD3DXMATRIX* ���C�g���_3�����x�N�g���̐ݒ� 
//-------------------------------------------------------------
void CShaderManager::SetLightEye(const Vector3* pVecEye)
{
	Vector3 eye = *pVecEye;
	Vector3 dir = eye;
	D3DXVec3Normalize(&dir, &dir);
	m_vecLightDir = Vector4(dir, 1.0f);
}

//-------------------------------------------------------------
// �V�F�[�_�̃p�����[�^�ݒ�
// ���@���FD3DXMATRIX* �J�������_3�����x�N�g���̐ݒ� 
//-------------------------------------------------------------
void CShaderManager::SetCameraEye(const Vector3* pVecEye)
{
	m_vecCameraEye = Vector4(*pVecEye, 1.0f);
}

//=============================================================
//
// Private
//
//=============================================================

//-------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------
CShaderManager::CShaderManager()
	: m_nowShaderState(Shader::DEPTH)
{
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
CShaderManager::~CShaderManager()
{
	if (!m_vecShader.empty())
	{
		for (auto shader : m_vecShader)
		{
			if (shader != nullptr)
			{
				delete shader;
				shader = nullptr;
			}
		}
		m_vecShader.clear();
	}
}

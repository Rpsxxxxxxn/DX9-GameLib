//********************************************************************************
//
// Field.cpp
//
// �t�B�[���h�N���X
//
// Date: 2019/07/30 �X�V
//
//********************************************************************************

//================================================================================
// �C���N���[�h
//================================================================================
#include "FieldModel.h"

using namespace FileConst;

//================================================================================
// �N���X��`
//================================================================================

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
CField::CField()
	: m_pModel(new CXFileModel)
	, m_pMesh(new CMesh)
	, m_vecLimit(Vector3(47.0f, 20.0f, 47.0f))
{
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
CField::~CField()
{
	SafeDelete(m_pModel);
	SafeDelete(m_pMesh);
}

//--------------------------------------------------------------------------------
// ������
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CField::Initialize(CSystem * pSystem) {}

//--------------------------------------------------------------------------------
// �X�V
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CField::Update(CSystem * pSystem)
{
	CGameObject::Update(pSystem);

	m_pModel->SetTransform(&m_matWorld);
}

//--------------------------------------------------------------------------------
// �`��
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CField::DrawZDepth(CSystem * pSystem)
{
	m_pModel->Draw(pSystem, Shader::DEPTH, m_isVisible, false);
}

//--------------------------------------------------------------------------------
// �`��
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CField::DrawShader(CSystem * pSystem)
{
	m_pModel->Draw(pSystem, Shader::SHADOW, m_isVisible, !m_isSleep);
}

//--------------------------------------------------------------------------------
// �����蔻��̐���
//--------------------------------------------------------------------------------
void CField::Create(CSystem* pSystem, const int field)
{
	SafeDelete(m_pMesh);

	m_pMesh = new CMesh;
	m_pModel->SetModel(pSystem->GetXFile(field));
	m_pMesh->Create(pSystem->GetXFile(field));
}
//*************************************************************
//
// Application.cpp
//
// �S�̂̃��C���N���X
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �N���X��`
//=============================================================
#include "Application.h"
#include "../Utility/Singleton.h"

//-------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------
CApplication::CApplication()
	: m_pClsWindowSystem(new CWindowSystem)
	, m_pClsSystem(new CSystem)
{
	CSceneManager::Create();
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
CApplication::~CApplication()
{
	CSingletonFinalizer::Finalize();
}

//-------------------------------------------------------------
// ������
// ���@���FHINSTANCE	�C���X�^���X�n���h��
// �@�@�@�@const int	�A�v���P�[�V���������\�����@
//-------------------------------------------------------------
void CApplication::Initialize(HINSTANCE hInst, const int nShowCmd)
{
	//�E�B���h�E������
	m_pClsWindowSystem->Initialize(hInst, nShowCmd);

	//�V�X�e���N���X�̏�����
	m_pClsSystem->Initialize(m_pClsWindowSystem);

	//�V�[���I�u�W�F�N�g�Ǘ��̏�����
	CSceneManager::GetInstance()->Initialize(m_pClsSystem);
}

//-------------------------------------------------------------
// ���C�����[�v
//-------------------------------------------------------------
void CApplication::MainLoop()
{
	while (m_pClsWindowSystem->IsLoop())
	{
		if (!CSceneManager::GetInstance()->ChangeScene())
			break;

		Update();
		Render();
	}
}

//-------------------------------------------------------------
// ���
//-------------------------------------------------------------
void CApplication::ShutDown()
{
	//�V�X�e���N���X�̃V���b�g�_�E��
	m_pClsSystem->ShutDown();

	//ImGui�̏I��
	ImGui_Start::ShutDown();

	//�V�[���I�u�W�F�N�g�Ǘ��̔j��
	CSceneManager::Destroy();

	//�E�B���h�E�V�X�e���N���X�̉��
	SafeDelete(m_pClsWindowSystem);

	//�V�X�e���N���X�̉��
	SafeDelete(m_pClsSystem);
}

//-------------------------------------------------------------
// �X�V
//-------------------------------------------------------------
void CApplication::Update()
{
	//���t���[���̃J�E���^�[
	m_pClsSystem->UpdateTimer();

	//ImGui�p�t���[������
	ImGui_Start::Begin();

	//�V�X�e���N���X�̍X�V
	m_pClsSystem->Update();

	//�V�[���Ǘ��̍X�V
	CSceneManager::GetInstance()->Update();
}

//-------------------------------------------------------------
// �`��
//-------------------------------------------------------------
void CApplication::Render()
{
	//�o�b�N�o�b�t�@�̃N���A
	m_pClsSystem->Clear(0xFF000000);

	//�����_�����O�J�n
	m_pClsSystem->BeginRender();

	//�V�[���Ǘ��̕`��
	CSceneManager::GetInstance()->Draw();

	//�����_�����O�̏I��
	m_pClsSystem->EndRender();

	//ImGui�����_�����O
	ImGui_Start::End();

	//�o�b�N�o�b�t�@���t�����g�o�b�t�@�Ɏ����Ă���
	m_pClsSystem->Present();
}

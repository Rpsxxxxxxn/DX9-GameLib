//*************************************************************
//
// SceneObject.cpp
//
// �V�[���I�u�W�F�N�g�N���X(���)
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "SceneTitle.h"
#include "../SceneManager.h"

//-------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------
CSceneTitle::CSceneTitle(CSystem* pSystem)
	: IScene(pSystem)
	, m_pTitle(new CTitleManager)
{
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
CSceneTitle::~CSceneTitle()
{
	this->Release();
}

//-------------------------------------------------------------
// �t�@�C���ǂݍ���
//-------------------------------------------------------------
void CSceneTitle::LoadFile()
{
	CSceneManager::shareData.fadeAni.LoadFile( m_pSystem );
	CSceneManager::shareData.render.LoadFile( m_pSystem );

	m_pTitle->LoadFile( m_pSystem );
}

//-------------------------------------------------------------
// ������
//-------------------------------------------------------------
void CSceneTitle::Initialize()
{
	CSceneManager::shareData.fadeAni.Initialize( m_pSystem );
	CSceneManager::shareData.fadeAni.SetFadeMode( FadeMode::F_OUT );
	CSceneManager::shareData.render.Initialize( m_pSystem );

	m_pTitle->Initialize( m_pSystem );
}

//-------------------------------------------------------------
// �X�V
//-------------------------------------------------------------
void CSceneTitle::Update()
{
	CSceneManager::shareData.render.Update( m_pSystem );

	m_pTitle->Update( m_pSystem );

	if ( m_pTitle->IsGame( &CSceneManager::shareData.fadeAni ) )
	{
		SCENE_CHANGE(SCENE_GAME);
	}

	if ( m_pTitle->IsExit( &CSceneManager::shareData.fadeAni ) )
	{
		SCENE_CHANGE(SCENE_EXIT);
	}
	
	CONSTANT_TABLE(CSceneManager::shareData.render.GetCamera());
}

//-------------------------------------------------------------
// �`��
//-------------------------------------------------------------
void CSceneTitle::ZDraw()
{
	CSceneManager::shareData.render.ZDraw( m_pSystem );
}

//-------------------------------------------------------------
// �`��
//-------------------------------------------------------------
void CSceneTitle::Draw()
{
	CSceneManager::shareData.render.Draw( m_pSystem );

	m_pTitle->Draw( m_pSystem );

	CSceneManager::shareData.fadeAni.Draw( m_pSystem );
}

//-------------------------------------------------------------
// ���
//-------------------------------------------------------------
void CSceneTitle::Release()
{
	m_pSystem->ReleaseAllFile();
	m_pSystem->ReleaseAllSound();

	SafeDelete(m_pTitle);
}
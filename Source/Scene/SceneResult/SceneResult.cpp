//*************************************************************
//
// SceneResult.cpp
//
// �V�[�����U���g
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "SceneResult.h"
#include "../SceneManager.h"

//=============================================================
// �N���X��`
//=============================================================

//-------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------
CSceneResult::CSceneResult(CSystem* pSystem)
	: IScene(pSystem)
	, m_pTimer(new CShowTimer)
{
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
CSceneResult::~CSceneResult()
{
	this->Release();
}

//-------------------------------------------------------------
// �t�@�C���ǂݍ���
//-------------------------------------------------------------
void CSceneResult::LoadFile()
{
	CSceneManager::shareData.render.LoadFile( m_pSystem );
	CSceneManager::shareData.fadeAni.LoadFile( m_pSystem );

	m_pTimer->LoadFile(m_pSystem);
}

//-------------------------------------------------------------
// ������
//-------------------------------------------------------------
void CSceneResult::Initialize()
{
	CSceneManager::shareData.render.Initialize( m_pSystem );
	CSceneManager::shareData.fadeAni.SetFadeMode(FadeMode::F_OUT);

	m_pTimer->Initialize(m_pSystem, CSceneManager::shareData.clearTime);
}

//-------------------------------------------------------------
// �X�V
//-------------------------------------------------------------
void CSceneResult::Update()
{
	CSceneManager::shareData.render.Update( m_pSystem );

	m_pTimer->Update(m_pSystem, &CSceneManager::shareData.fadeAni);

	if ( m_pTimer->IsTitle() )
	{
		SCENE_CHANGE( Scene::SCENE_TITLE );
	}

	CONSTANT_TABLE(CSceneManager::shareData.render.GetCamera());
}

//-------------------------------------------------------------
// �`��
//-------------------------------------------------------------
void CSceneResult::ZDraw()
{
	CSceneManager::shareData.render.ZDraw( m_pSystem );
}

//-------------------------------------------------------------
// �`��
//-------------------------------------------------------------
void CSceneResult::Draw()
{
	CSceneManager::shareData.render.Draw( m_pSystem );

	m_pTimer->Draw(m_pSystem);

	CSceneManager::shareData.fadeAni.Draw( m_pSystem );
}

//-------------------------------------------------------------
// ���
//-------------------------------------------------------------
void CSceneResult::Release()
{
	m_pSystem->ReleaseAllSound();
	m_pSystem->ReleaseAllFile();

	SafeDelete( m_pTimer );
}
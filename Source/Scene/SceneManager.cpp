//*************************************************************
//
// SceneManager.cpp
//
// �V�[���I�u�W�F�N�g�Ǘ�
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "SceneManager.h"
#include "IScene.h"
#include "SceneTitle/SceneTitle.h"
#include "SceneGame/SceneGame.h"
#include "SceneResult/SceneResult.h"

//-------------------------------------------------------------
// ������
//-------------------------------------------------------------
void CSceneManager::Initialize(CSystem * pSystem)
{
	m_pSystem = pSystem;
}

//-------------------------------------------------------------
// �X�V
//-------------------------------------------------------------
void CSceneManager::Update()
{
	m_pNowScene->Update();
}

//-------------------------------------------------------------
// �`��
//-------------------------------------------------------------
void CSceneManager::Draw()
{
	//�f�v�X�N���X�ϊ�
	auto shaderDepth = CShaderManager::GetInstance().GetShader(Shader::DEPTH);
	auto depth = dynamic_cast<CShadowDepth*>(shaderDepth);

	//�f�v�X�����_�����O
	for (int i = 0; i < 3; i++)
	{
		depth->StartDepthRender(i);
		{
			m_pNowScene->ZDraw();
		}
		depth->EndDepthRender();

		m_pShadowMap[i] = depth->GetTexture(i);
	}

	// �V���h�E�N���X�ϊ�
	auto shaderShadow = CShaderManager::GetInstance().GetShader(Shader::SHADOW);
	auto shadow = dynamic_cast<CShadowMap*>(shaderShadow);

	//�V���h�E�e�N�X�`���̐ݒ�
	shadow->SetShadowTexture0(m_pShadowMap[0]);
	shadow->SetShadowTexture1(m_pShadowMap[1]);
	shadow->SetShadowTexture2(m_pShadowMap[2]);
	
	{
		m_pNowScene->Draw();
	}
}

//-------------------------------------------------------------
// �V�[���̐ݒ�
// ���@���Fconst Scene	���̃V�[���^�C�v
//-------------------------------------------------------------
void CSceneManager::SetScene(const Scene type)
{
	m_newScene = type;
}

//-------------------------------------------------------------
// �V�[���ύX
//-------------------------------------------------------------
bool CSceneManager::ChangeScene()
{
	if (m_newScene != m_nowScene)
	{
		//���ɂ���Ȃ�΍폜
		SafeDelete(m_pNowScene);
		
		switch (m_newScene)
		{
		case SCENE_TITLE:
			m_pNowScene = new CSceneTitle(m_pSystem);
			break;
		case SCENE_GAME:
			m_pNowScene = new CSceneGame(m_pSystem);
			break;
		case SCENE_RESULT:
			m_pNowScene = new CSceneResult(m_pSystem);
			break;
		case SCENE_EXIT:
			return false;
		}

		if (m_pNowScene == nullptr) return true;

		//�t�@�C���ǂݍ���
		m_pNowScene->LoadFile();

		//������
		m_pNowScene->Initialize();

		//���݂̃V�[���ɂ���
		m_nowScene = m_newScene;
	}

	return true;
}

//-------------------------------------------------------------
// �����V���h�E�̒l�ݒ�
// ���@���FCCamera*			�J�����N���X�̃|�C���^
//-------------------------------------------------------------
void CSceneManager::SetConstantTable(
	CCamera* pCamera
)
{
	m_pLight->SetOrtho();

	//���ӓ���
	auto viewMat = pCamera->GetViewMatrix();
	auto projMat = pCamera->GetProjectionMatrix();
	auto lightViewMat = m_pLight->GetViewMatrix();
	auto lightProjMat = m_pLight->GetProjectionMatrix();
	auto lightBiasMat = m_pLight->GetLightBias();
	auto lightEyeVec = m_pLight->GetPosition();

	//�J�X�P�[�f�B���O�V���h�E�}�b�v�̃p�����[�^�ݒ�
	auto shader = CShaderManager::GetInstance().GetShader(DEPTH);
	auto zDepth = static_cast<CShadowDepth*>(shader);

	zDepth->SetLightViewMatrix(&lightViewMat);
	zDepth->SetLightProjMatrix(&lightProjMat);
	zDepth->SetLightPosition(&lightEyeVec);
	zDepth->RecalculateShadowMapMatrices(pCamera);

	m_pMatVP[0] = zDepth->GetViewProjMatrix(0);
	m_pMatVP[1] = zDepth->GetViewProjMatrix(1);
	m_pMatVP[2] = zDepth->GetViewProjMatrix(2);

	//for (int i = 0; i < 3; i++)
	//	D3DXMatrixTranspose(&m_pMatVP[i], &m_pMatVP[i]);

	//D3DXMatrixTranspose(&viewMat, &viewMat);
	//D3DXMatrixTranspose(&projMat, &projMat);

	CShaderManager::GetInstance().SetMatView(viewMat);
	CShaderManager::GetInstance().SetMatProj(projMat);
	CShaderManager::GetInstance().SetMatLViewProj(m_pMatVP);
	CShaderManager::GetInstance().SetMatLBias(lightBiasMat);
	CShaderManager::GetInstance().SetLightEye(&lightEyeVec);
	CShaderManager::GetInstance().SetCameraEye(&pCamera->m_vecEye);
	CShaderManager::GetInstance().Update();
}

//-------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------
CSceneManager::CSceneManager()
	: m_newScene(Scene::SCENE_TITLE)
	, m_nowScene(Scene::SCENE_GAME)
	, m_pLight(new CDirectionLight)
	, m_pMatVP(new Matrix[3])
	, m_pNowScene(nullptr)
{

}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
CSceneManager::~CSceneManager()
{
	SafeDelete(m_pLight);
	SafeDelete(m_pNowScene);
}

//-------------------------------------------------------------
// �C���X�^���X����
//-------------------------------------------------------------
ShareObject CSceneManager::shareData;
CSceneManager* CSceneManager::m_pInstance = nullptr;

//-------------------------------------------------------------
// �C���X�^���X�̎擾
//-------------------------------------------------------------
CSceneManager* CSceneManager::GetInstance()
{
	return m_pInstance;
}

//-------------------------------------------------------------
// ����
//-------------------------------------------------------------
void CSceneManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new CSceneManager;
	}
}

//-------------------------------------------------------------
// �j��
//-------------------------------------------------------------
void CSceneManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

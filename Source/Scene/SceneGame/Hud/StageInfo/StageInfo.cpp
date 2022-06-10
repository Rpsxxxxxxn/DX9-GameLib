//********************************************************************************
//
// StageInfo.hpp
//
// �K�C�h�\���p�N���X
//
// Date: 2019/07/30 �X�V
//
//********************************************************************************

//================================================================================
// �C���N���[�h
//================================================================================
#include "StageInfo.hpp"

using namespace FileConst;

const Vector2 SHOW_TEXTURE_POS(339.0f, 66.5f);

//================================================================================
// �N���X��`
//================================================================================

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
CStageInfo::CStageInfo()
{
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
CStageInfo::~CStageInfo()
{
	SafeDelete(m_pSprite);
}

//--------------------------------------------------------------------------------
// �t�@�C���ǂݍ���
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CStageInfo::LoadFile(CSystem * pSystem)
{
	pSystem->LoadTexture(TEXTURE_GAME_CLEAR,	"Resource/Textures/GameClear/GameClear.png");
	pSystem->LoadTexture(TEXTURE_GAME_OVER,		"Resource/Textures/GameClear/GameOver.png");
	pSystem->LoadTexture(TEXTURE_STAGE_CLEAR,	"Resource/Textures/GameOver/StageClear.png"); 
}

//--------------------------------------------------------------------------------
// ������
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CStageInfo::Initialize(CSystem * pSystem)
{
	m_halfWidth = pSystem->GetWindowSettings().Width / 2.0f;
	m_halfHeight = pSystem->GetWindowSettings().Height / 2.0f;

	m_pSprite = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_STAGE_CLEAR));
	m_pSprite->SetPosition(Vector2(m_halfWidth - SHOW_TEXTURE_POS.x, m_halfHeight - SHOW_TEXTURE_POS.y));
	m_pSprite->SetAlpha(0);
}

//--------------------------------------------------------------------------------
// �X�V
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
// �@�@�@�@CGameProgress*	�i�s�󋵃N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CStageInfo::Update(CSystem * pSystem, CGameProgress* pProgress)
{
	StageManager(pProgress);

	ShowGameClear(pSystem, pProgress);

	ShowGameOver(pSystem, pProgress);
}

//--------------------------------------------------------------------------------
// �`��
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CStageInfo::Draw(CSystem * pSystem)
{
	m_pSprite->Draw(pSystem);
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// �X�e�[�W�Ǘ�
// ���@���FCGameProgress*	�i�s�󋵃N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CStageInfo::StageManager(CGameProgress * pProgress)
{
	int newStage = pProgress->GetNowStage();

	if (m_nowStage != newStage)
	{
		if (!pProgress->IsStartFade()) return;

		m_pSprite->SetAlpha(0);

		m_nowStage = newStage;
	}
}

//--------------------------------------------------------------------------------
// �Q�[���N���A�̕\��
// ���@���FCSystem*		�V�X�e���N���X�̃|�C���^
// �@�@�@�@CGameProgress*	�i�s�󋵃N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CStageInfo::ShowGameClear(CSystem* pSystem, CGameProgress * pProgress)
{
	if (!pProgress->IsStageClear()) return;

	if (m_nowStage < STAGE_3)
	{
		ChangeTextureData(pSystem, TEXTURE_STAGE_CLEAR);
	}
	else 
	{
		ChangeTextureData(pSystem, TEXTURE_GAME_CLEAR);
	}
}

//--------------------------------------------------------------------------------
// �Q�[���I�[�o�[�̕\��
// ���@���FCSystem*			�V�X�e���N���X�̃|�C���^
// �@�@�@�@CGameProgress*	�i�s�󋵃N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CStageInfo::ShowGameOver(CSystem * pSystem, CGameProgress * pProgress)
{
	if (!pProgress->IsPlayerDeath()) return;

	ChangeTextureData(pSystem, TEXTURE_GAME_OVER);
}

//--------------------------------------------------------------------------------
// �e�N�X�`���f�[�^�̍X�V
// ���@���FCSystem*		�V�X�e���N���X�̃|�C���^
// �@�@�@�@const int	�t�@�C���ԍ�
//--------------------------------------------------------------------------------
void CStageInfo::ChangeTextureData(CSystem * pSystem, const int fileConst)
{
	m_pSprite->SetTextureData(pSystem->GetTextureData(fileConst));
	m_pSprite->SetNewAlpha(255);
}
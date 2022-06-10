//********************************************************************************
//
// CStatus.hpp
//
// �X�e�[�^�X���̕`��
//
// Date: 2019/07/30 �X�V
//
//********************************************************************************

//================================================================================
// �C���N���[�h
//================================================================================
#include "Status.hpp"

using namespace FileConst;

//================================================================================
// �N���X��`
//================================================================================

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
CStatus::CStatus()
	: m_playerHp(0.0f)
	, m_playerStamina(0.0f)
{
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
CStatus::~CStatus()
{
	for (auto w : m_pSprite)
	{
		SafeDelete(w);
	}
}

//--------------------------------------------------------------------------------
// �t�@�C���ǂݍ���
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CStatus::LoadFile(CSystem * pSystem)
{
	pSystem->LoadTexture(TEXTURE_STATUS_BACK,	"Resource/Textures/Game/StatusBack.png");
	pSystem->LoadTexture(TEXTURE_HP,			"Resource/Textures/Game/Hp.png");
	pSystem->LoadTexture(TEXTURE_STAMINA,		"Resource/Textures/Game/Stamina.png");
	pSystem->LoadTexture(TEXTURE_STATUS_BACK,	"Resource/Textures/Share/Gauge_Back.png");
	pSystem->LoadTexture(TEXTURE_STATUS_FRONT,	"Resource/Textures/Share/Gauge_Front.png");
	pSystem->LoadTexture(TEXTURE_SHOW_DAMAGE, "Resource/Textures/Game/Damage.png");
	pSystem->LoadTexture(TEXTURE_BOSS_HP_BAR, "Resource/Textures/Game/Enemy_Hp_Bar.png");
	pSystem->LoadTexture(TEXTURE_BOSS_HP_FRONT, "Resource/Textures/Game/Enemy_Hp_Front.png");
}

//--------------------------------------------------------------------------------
// ������
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CStatus::Initialize(CSystem * pSystem)
{
	auto windowSettings = pSystem->GetWindowSettings();
	auto halfWidth = windowSettings.Width / 2.0f;
	auto halfHeight = windowSettings.Height / 2.0f;

	m_pSprite[STATUS_BACK]	= new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_STATUS_BACK));
	m_pSprite[HP_BAR]		= new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_HP));
	m_pSprite[STAMINA_BAR]	= new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_STAMINA));
	m_pSprite[HP_FRONT]		= new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_STATUS_FRONT));
	m_pSprite[ST_FRONT]		= new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_STATUS_FRONT));
	m_pSprite[SHOW_DAMAGE]	= new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_SHOW_DAMAGE));
	m_pSprite[BOSS_HP_BAR]	= new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_BOSS_HP_BAR));
	m_pSprite[BOSS_HP_FRONT] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_BOSS_HP_FRONT));

	m_pSprite[STATUS_BACK]->SetPosition(Vector2(5.0f, 5.0f));
	m_pSprite[HP_BAR]->SetPosition(Vector2(55.0f, 15.0f));
	m_pSprite[STAMINA_BAR]->SetPosition(Vector2(55.0f, 52.0f));
	m_pSprite[HP_FRONT]->SetPosition(Vector2(52.0f, 13.0f));
	m_pSprite[ST_FRONT]->SetPosition(Vector2(52.0f, 50.0f));
	m_pSprite[BOSS_HP_BAR]->SetPosition(Vector2(halfWidth - 253.0f, windowSettings.Height - 97.0f));
	m_pSprite[BOSS_HP_FRONT]->SetPosition(Vector2(halfWidth - 257.0f, windowSettings.Height - 100.0f));

	m_pSprite[SHOW_DAMAGE]->SetSize(windowSettings.Width, windowSettings.Height);

	m_pSprite[SHOW_DAMAGE]->SetAlpha(0);
	m_pSprite[BOSS_HP_BAR]->SetAlpha(0);
	m_pSprite[BOSS_HP_FRONT]->SetAlpha(0);
}

//--------------------------------------------------------------------------------
// �X�V
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CStatus::Update(CSystem * pSystem, CPlayer* pPlayer, CAIManager* pAI)
{
	UpdatePlayerStatus(pPlayer);

	UpdateProgress();

	UpdateBossHp(pAI);

	ShowDamage(pPlayer);
}

//--------------------------------------------------------------------------------
// �`��
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CStatus::Draw(CSystem * pSystem)
{
	for (int i = 0; i < MAX; i++)
	{
		m_pSprite[i]->Draw(pSystem);
	}
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// �v���C���[���̍X�V
//--------------------------------------------------------------------------------
void CStatus::UpdatePlayerStatus(CPlayer* pPlayer)
{
	m_playerHp = pPlayer->GetHp();
	m_playerStamina = pPlayer->GetStamina();
}

//--------------------------------------------------------------------------------
// �v���C���[HP�ƃX�^�~�i�̏󋵍X�V
//--------------------------------------------------------------------------------
void CStatus::UpdateProgress()
{
	float width = 0.0f;

	width = Calculate::ProgressBar(329.0f, 100.0f, m_playerHp);
	m_pSprite[HP_BAR]->SetSizeX((UINT)width);

	width = Calculate::ProgressBar(329.0f, 100.0f, m_playerStamina);
	m_pSprite[STAMINA_BAR]->SetSizeX((UINT)width);
}

//--------------------------------------------------------------------------------
// �{�XHP�̍X�V�ƌv�Z
//--------------------------------------------------------------------------------
void CStatus::UpdateBossHp(CAIManager* pAI)
{
	if (!pAI->IsAddedBoss()) return;

	m_bossHp = pAI->GetBoss()->GetHp();

	m_pSprite[BOSS_HP_BAR]->SetAlpha(255);
	m_pSprite[BOSS_HP_FRONT]->SetAlpha(255);

	auto width = Calculate::ProgressBar(506.0f, 100.0f, m_bossHp);
	m_pSprite[BOSS_HP_BAR]->SetSizeX((UINT)width);
}

//--------------------------------------------------------------------------------
// �_���[�W�󂯂��Ƃ��ɐԂ��Ȃ���
//--------------------------------------------------------------------------------
void CStatus::ShowDamage(CPlayer* pPlayer)
{
	if (!pPlayer->IsEnemyHit()) return;

	m_pSprite[SHOW_DAMAGE]->SetAlpha(255);
	m_pSprite[SHOW_DAMAGE]->SetNewAlpha(0);

	pPlayer->EnemyHitReset();
}

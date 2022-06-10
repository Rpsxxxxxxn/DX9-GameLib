//********************************************************************************
//
// StartSelect.cpp
//
// �^�C�g����ʂ̃Z���N�g
//
// Date: 2019/07/30 �X�V
//
//********************************************************************************

//================================================================================
// �C���N���[�h
//================================================================================
#include "StartSelect.hpp"

using namespace FileConst;

//================================================================================
// �N���X��`
//================================================================================

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
CStartSelect::CStartSelect()
	: m_determination(StartSelect::SS_NONE)
	, m_nowSelect(StartSelect::SS_START)
	, m_nowCount(-1)
	, m_angle(0.0f)
	, m_cursorHeight(405.0f)
	, m_enableShowAni(true)
	, m_enableHideAni(false)
	, m_isVisible(true)
	, m_isDownKey(false)
	, m_isUpKey(false)
	, m_isUp(false)
	, m_isDown(false)
	, m_isEnter(false)
	, m_isJoyStickUpSync(false)
	, m_isJoyStickDownSync(false)
{
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
CStartSelect::~CStartSelect()
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
void CStartSelect::LoadFile(CSystem * pSystem)
{
	pSystem->LoadTexture(TEXTURE_START,			"Resource/Textures/Title/Start.png");
	pSystem->LoadTexture(TEXTURE_EXIT,			"Resource/Textures/Title/Exit.png");
	pSystem->LoadTexture(TEXTURE_CURSOR,		"Resource/Textures/Title/Cursor.png");
	pSystem->LoadTexture(TEXTURE_SWORD_LOGO,	"Resource/Textures/Title/StealthLogo.png");
	pSystem->LoadTexture(TEXTURE_MASTER_LOGO,	"Resource/Textures/Title/MasterLogo.png");
	pSystem->LoadTexture(TEXTURE_SWORD,			"Resource/Textures/Title/Sword.png");

	if (pSystem->IsControllerConnect())
	{
		pSystem->LoadTexture(TEXTURE_TITLE_KEY, "Resource/Textures/Guide/Title_Pad.png");
	}
	else
	{
		pSystem->LoadTexture(TEXTURE_TITLE_KEY, "Resource/Textures/Guide/Title_Key.png");
	}
}

//--------------------------------------------------------------------------------
// ������
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CStartSelect::Initialize(CSystem * pSystem)
{
	auto width = (float)pSystem->GetWindowSettings().Width;
	auto height = (float)pSystem->GetWindowSettings().Height;
	m_halfWidth = width / 2.0f;
	m_halfHeight = height / 2.0f;

	m_pSprite[ST_START] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_START));
	m_pSprite[ST_EXIT] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_EXIT));
	m_pSprite[ST_L_CURSOR] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_CURSOR));
	m_pSprite[ST_R_CURSOR] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_CURSOR));
	m_pSprite[ST_SWORDLOGO] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_SWORD_LOGO));
	m_pSprite[ST_MASTERLOGO] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_MASTER_LOGO));
	m_pSprite[ST_SWORD] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_SWORD));
	m_pSprite[ST_GUIDE] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_TITLE_KEY));

	m_pSprite[ST_START]->SetPosition(Vector2(width, m_halfHeight + 75.0f));
	m_pSprite[ST_EXIT]->SetPosition(Vector2(width, m_halfHeight + 205.0f));
	m_pSprite[ST_L_CURSOR]->SetPosition(Vector2(width, m_halfHeight + 105.0f));
	m_pSprite[ST_R_CURSOR]->SetPosition(Vector2(width, m_halfHeight + 105.0f));
	m_pSprite[ST_SWORDLOGO]->SetPosition(Vector2(0.0f, m_halfHeight - 300.0f));
	m_pSprite[ST_MASTERLOGO]->SetPosition(Vector2(width, m_halfHeight - 150.0f));
	m_pSprite[ST_SWORD]->SetPosition(Vector2(width, m_halfHeight - 300.0f));
	m_pSprite[ST_GUIDE]->SetPosition(Vector2(-100.0f, m_halfHeight + (m_halfHeight / 4.0f)));

	m_pSprite[ST_SWORDLOGO]->SetEndTime(2000);
	m_pSprite[ST_MASTERLOGO]->SetEndTime(2000);

	for (auto w : m_pSprite) w->SetAlpha(0);
}

//--------------------------------------------------------------------------------
// �X�V
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CStartSelect::Update(CSystem * pSystem)
{
	KeyInput(pSystem);

	SelectController(pSystem);

	CursorRotate();

	ShowAnimation();

	HideAnimation();
}

//--------------------------------------------------------------------------------
// �`��
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CStartSelect::Draw(CSystem * pSystem)
{
	if (!m_isVisible) return;

	for (auto i = 0; i < ST_MAX; i++)
	{
		m_pSprite[i]->Draw(pSystem);
	}
}

//--------------------------------------------------------------------------------
// �����̑I������
// �߂�l�Fconst StartSelect	�I���ς�
//--------------------------------------------------------------------------------
CStartSelect::StartSelect CStartSelect::GetSelected() const
{
	return m_determination;
}

//--------------------------------------------------------------------------------
// �\�����̃Z�b�g(�\��)
//--------------------------------------------------------------------------------
void CStartSelect::SetShowSelect()
{
	m_enableShowAni = true;
}

//--------------------------------------------------------------------------------
// �\�����̃Z�b�g(�B��)
//--------------------------------------------------------------------------------
void CStartSelect::SetHideSelect()
{
	m_enableHideAni = true;
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// ���x�̂�
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
bool CStartSelect::JoyStickUpSync(CSystem * pSystem)
{
	if (pSystem->GetJoyStickState(KeyConfig::LEFTVER) < -0.1f)
	{
		if (!m_isJoyStickUpSync)
		{
			m_isJoyStickUpSync = true;
			return true;
		}
	}
	else {
		m_isJoyStickUpSync = false;
	}

	return false;
}

//--------------------------------------------------------------------------------
// ����x�̂�
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
bool CStartSelect::JoyStickDownSync(CSystem * pSystem)
{
	if (pSystem->GetJoyStickState(KeyConfig::LEFTVER) > 0.1f)
	{
		if (!m_isJoyStickDownSync)
		{
			m_isJoyStickDownSync = true;
			return true;
		}
	}
	else {
		m_isJoyStickDownSync = false;
	}

	return false;
}

//--------------------------------------------------------------------------------
// �L�[�{�[�h���́��W���C�X�e�B�b�N���͏���
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CStartSelect::KeyInput(CSystem * pSystem)
{
	m_isUp = pSystem->IsKeyPressed(DIK_UP) || JoyStickUpSync(pSystem);
	m_isDown = pSystem->IsKeyPressed(DIK_DOWN) || JoyStickDownSync(pSystem);
	m_isEnter = pSystem->IsKeyPressed(DIK_RETURN) || pSystem->IsPadPressed(KeyConfig::CIRCLE);
}

//--------------------------------------------------------------------------------
// �I�𑀍�
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CStartSelect::SelectController(CSystem * pSystem)
{
	m_determination = SS_NONE;

	if (m_isUp)
	{
		pSystem->PlaySe(SE_SELECT);
		m_nowSelect = SS_START;
	}

	if (m_isDown)
	{
		pSystem->PlaySe(SE_SELECT);
		m_nowSelect = SS_EXIT;
	}

	if (m_isEnter && !m_enableHideAni && !m_enableShowAni)
	{
		pSystem->PlaySe(SE_ENTER);
		m_determination = m_nowSelect;
		SetHideSelect();
	}

	if (m_isUp || m_isDown)
	{
		switch (m_nowSelect)
		{
		case CStartSelect::SS_START: SetCursorPos(105.0f); break;
		case CStartSelect::SS_EXIT: SetCursorPos(235.0f); break;
		}
	}
}

//--------------------------------------------------------------------------------
// �\���A�j���[�V����
//--------------------------------------------------------------------------------
void CStartSelect::ShowAnimation()
{
	if (!m_enableShowAni) return;

	switch (m_nowCount)
	{
	case 0: LogoTexture(true); break;
	case 1: SwordTexture(true); break;
	case 2: SelectTexture(true); break;
	case 3: CursorTexture(true); break;
	}

	if (m_nowCount > 3 )
	{
		m_nowSelect = SS_START;
		m_enableShowAni = false;
		m_nowCount = 0;
	}
	else {
		m_nowCount++;
	}
}

//--------------------------------------------------------------------------------
// �B���A�j���[�V����
//--------------------------------------------------------------------------------
void CStartSelect::HideAnimation()
{
	if (!m_enableHideAni) return;

	switch (m_nowCount)
	{
	case 0: CursorTexture(false); break;
	case 1: SelectTexture(false); break;
	case 2: SwordTexture(false); break;
	case 3: LogoTexture(false); break;
	}

	if (m_nowCount > 3)
	{
		m_nowSelect = SS_START;
		m_enableHideAni = false;
		m_nowCount = 0;
	}
	else {
		m_nowCount++;
	}
}

//--------------------------------------------------------------------------------
// �J�[�\���̉�]
//--------------------------------------------------------------------------------
void CStartSelect::CursorRotate()
{
	m_angle += 0.03f;

	m_pSprite[ST_L_CURSOR]->SetAngle(m_angle);
	m_pSprite[ST_R_CURSOR]->SetAngle(-m_angle);
}

//--------------------------------------------------------------------------------
// �J�[�\���̈ʒu
//--------------------------------------------------------------------------------
void CStartSelect::SetCursorPos(const float height)
{
	m_cursorHeight = height;
	m_pSprite[ST_L_CURSOR]->SetNewPosition(Vector2(m_halfWidth - 300.0f, m_halfHeight + height));
	m_pSprite[ST_R_CURSOR]->SetNewPosition(Vector2(m_halfWidth + 250.0f, m_halfHeight + height));
}

//--------------------------------------------------------------------------------
// ���S�e�N�X�`��
//--------------------------------------------------------------------------------
void CStartSelect::LogoTexture(const bool show)
{
	if (show)
	{
		m_pSprite[ST_SWORDLOGO]->SetNewPosition(Vector2(m_halfWidth - 397.5f, m_halfHeight - 300.0f));
		m_pSprite[ST_SWORDLOGO]->SetNewAlpha(255);
		m_pSprite[ST_MASTERLOGO]->SetNewPosition(Vector2(m_halfWidth - 183.0f, m_halfHeight - 150.0f));
		m_pSprite[ST_MASTERLOGO]->SetNewAlpha(255);
	}
	else {
		m_pSprite[ST_SWORDLOGO]->SetNewPosition(Vector2(0.0f, m_halfHeight - 300.0f));
		m_pSprite[ST_SWORDLOGO]->SetNewAlpha(0);
		m_pSprite[ST_MASTERLOGO]->SetNewPosition(Vector2(0.0f, m_halfHeight - 150.0f));
		m_pSprite[ST_MASTERLOGO]->SetNewAlpha(0);
	}
}

//--------------------------------------------------------------------------------
// �\�[�h�e�N�X�`��
//--------------------------------------------------------------------------------
void CStartSelect::SwordTexture(const bool show)
{
	if (show)
	{
		m_pSprite[ST_SWORD]->SetNewPosition(Vector2(m_halfWidth - 450.5f, m_halfHeight - 300.0f));
		m_pSprite[ST_SWORD]->SetNewAlpha(255);
	}
	else {
		m_pSprite[ST_SWORD]->SetNewPosition(Vector2(0.0f, m_halfHeight - 300.0f));
		m_pSprite[ST_SWORD]->SetNewAlpha(0);
	}
}

//--------------------------------------------------------------------------------
// �I���e�N�X�`��
//--------------------------------------------------------------------------------
void CStartSelect::SelectTexture(const bool show)
{
	if (show)
	{
		m_pSprite[ST_START]->SetNewPosition(Vector2(m_halfWidth - 158.5f, m_halfHeight + 75.0f));
		m_pSprite[ST_START]->SetNewAlpha(255);
		m_pSprite[ST_EXIT]->SetNewPosition(Vector2(m_halfWidth - 123.5f, m_halfHeight + 205.0f));
		m_pSprite[ST_EXIT]->SetNewAlpha(255);
		m_pSprite[ST_GUIDE]->SetNewPosition(Vector2(5.0f, m_halfHeight + (m_halfHeight / 4.0f)));
		m_pSprite[ST_GUIDE]->SetNewAlpha(255);
	}
	else {
		m_pSprite[ST_START]->SetNewPosition(Vector2(0.0f, m_halfHeight + 75.0f));
		m_pSprite[ST_START]->SetNewAlpha(0);
		m_pSprite[ST_EXIT]->SetNewPosition(Vector2(0.0f, m_halfHeight + 205.0f));
		m_pSprite[ST_EXIT]->SetNewAlpha(0);
		m_pSprite[ST_GUIDE]->SetNewPosition(Vector2(-100.0f, m_halfHeight + (m_halfHeight / 4.0f)));
		m_pSprite[ST_GUIDE]->SetNewAlpha(0);
	}
}

//--------------------------------------------------------------------------------
// �J�[�\���e�N�X�`��
//--------------------------------------------------------------------------------
void CStartSelect::CursorTexture(const bool show)
{
	if (show)
	{
		m_pSprite[ST_L_CURSOR]->SetNewPosition(Vector2(m_halfWidth - 300.0f, m_halfHeight + 105.0f));
		m_pSprite[ST_L_CURSOR]->SetNewAlpha(255);
		m_pSprite[ST_R_CURSOR]->SetNewPosition(Vector2(m_halfWidth + 250.0f, m_halfHeight + 105.0f));
		m_pSprite[ST_R_CURSOR]->SetNewAlpha(255);
	}
	else {
		m_pSprite[ST_L_CURSOR]->SetNewPosition(Vector2(m_halfWidth, m_halfHeight + m_cursorHeight));
		m_pSprite[ST_L_CURSOR]->SetNewAlpha(0);
		m_pSprite[ST_R_CURSOR]->SetNewPosition(Vector2(m_halfWidth, m_halfHeight + m_cursorHeight));
		m_pSprite[ST_R_CURSOR]->SetNewAlpha(0);
	}
}

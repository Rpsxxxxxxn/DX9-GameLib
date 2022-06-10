//********************************************************************************
//
// FadeAni.cpp
//
// �t�F�[�h�A�j���[�V�����N���X
//
// Date: 2019/07/30 �X�V
//
//********************************************************************************

//================================================================================
// �C���N���[�h
//================================================================================
#include "FadeAni.hpp"

using namespace FileConst;

const DWORD MAX_ALPHA = 255;
const DWORD HALF_ALPHA = MAX_ALPHA / 2;
const float VOLUME_LERP_SPEED = 0.05f;

//================================================================================
// �N���X��`
//================================================================================

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
CFadeAni::CFadeAni()
	: m_dwAlpha(MAX_ALPHA)
	, m_dwSpeed(3)
	, m_fadeVolume(0.0f)
	, m_soundVolume(0.0f)
	, m_isFadeEnd(false)
	, m_fadeMode(F_NONE)
{
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
CFadeAni::~CFadeAni()
{
}

//--------------------------------------------------------------------------------
// �t�@�C���ǂݍ���
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CFadeAni::LoadFile(CSystem * pSystem)
{
	pSystem->LoadTexture(TEXTURE_FADE, "Resource/Textures/Fade/Fade.png");
}

//--------------------------------------------------------------------------------
// ������
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CFadeAni::Initialize(CSystem * pSystem)
{
	auto window = pSystem->GetWindowSettings();
	auto texData = pSystem->GetTextureData(TEXTURE_FADE);

	SetRect(&m_rSour, 0, 0, texData->uiWidth, texData->uiHeight);
	SetRect(&m_rDest, 0, 0, window.Width, window.Height);
}

//--------------------------------------------------------------------------------
// �`��
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CFadeAni::Draw(CSystem * pSystem)
{
	Update(pSystem);

	pSystem->SpriteBegin();
	pSystem->DrawSprite(&m_rSour, pSystem->GetTexture(TEXTURE_FADE), &m_rDest, m_dwAlpha);
	pSystem->SpriteClose();
}

//--------------------------------------------------------------------------------
// �t�F�[�h�̑��x��ݒ�
// ���@���Fconst DWORD	�X�s�[�h
//--------------------------------------------------------------------------------
void CFadeAni::SetFadeSpeed(const DWORD dwSpeed)
{
	m_dwSpeed = dwSpeed;
}

//--------------------------------------------------------------------------------
// �t�F�[�h�̎d���ݒ�
// ���@���FFade	�t�F�[�h���[�h
//--------------------------------------------------------------------------------
void CFadeAni::SetFadeMode(FadeMode mode)
{
	m_fadeMode = mode;
}

//--------------------------------------------------------------------------------
// �t�F�[�h���I���������Ƃ�m�点��
// �߂�l�Fconst bool �t�F�[�h�I��
//--------------------------------------------------------------------------------
bool CFadeAni::IsFadeEnd() const
{
	return m_isFadeEnd;
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// �t�F�[�h�J�n
// ���@�l�F���񂾂�Â��Ȃ�
//--------------------------------------------------------------------------------
void CFadeAni::FadeInStart(CSystem * pSystem)
{
	if (m_dwAlpha < MAX_ALPHA)
	{
		m_fadeVolume = 0.0f;
		m_dwAlpha += m_dwSpeed;
	}
	else {
		m_fadeMode = F_NONE;
		m_isFadeEnd = true;
	}
}

//--------------------------------------------------------------------------------
// �t�F�[�h�J�n
// ���@�l�F���񂾂񖾂邭�Ȃ�
//--------------------------------------------------------------------------------
void CFadeAni::FadeOutStart(CSystem * pSystem)
{
	if (m_dwAlpha > 0)
	{
		m_fadeVolume = 1.0f;
		m_dwAlpha -= m_dwSpeed;
	}
	else {
		m_fadeMode = F_NONE;
		m_isFadeEnd = true;
	}
}

//--------------------------------------------------------------------------------
// �t�F�[�h�J�n
// ���@�l�F���񂾂�Â��Ȃ�(����)
//--------------------------------------------------------------------------------
void CFadeAni::HalfFadeInStart(CSystem * pSystem)
{
	if (m_dwAlpha < HALF_ALPHA)
	{
		m_fadeVolume = 0.5f;
		m_dwAlpha += m_dwSpeed;
	}
	else {
		m_fadeMode = F_NONE;
		m_isFadeEnd = true;
	}
}

//--------------------------------------------------------------------------------
// �t�F�[�h�J�n
// ���@�l�F���񂾂񖾂邭�Ȃ�(����)
//--------------------------------------------------------------------------------
void CFadeAni::HalfFadeOutStart(CSystem * pSystem)
{
	if (m_dwAlpha > HALF_ALPHA)
	{
		m_fadeVolume = 1.0f;
		m_dwAlpha -= m_dwSpeed;
	}
	else {
		m_fadeMode = F_NONE;
		m_isFadeEnd = true;
	}
}

//--------------------------------------------------------------------------------
// �T�E���h
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CFadeAni::SoundManager(CSystem* pSystem)
{
	m_soundVolume = VectorCalculate::Lerp(
		m_soundVolume,
		m_fadeVolume,
		VOLUME_LERP_SPEED);

	pSystem->SetVolume(m_soundVolume);
}

//--------------------------------------------------------------------------------
// �X�V
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CFadeAni::Update(CSystem * pSystem)
{
	m_isFadeEnd = false;

	switch (m_fadeMode)
	{
	case FadeMode::F_IN: FadeInStart(pSystem); break;
	case FadeMode::F_OUT: FadeOutStart(pSystem); break;
	case FadeMode::F_HALF_IN: HalfFadeInStart(pSystem);	break;
	case FadeMode::F_HALF_OUT: HalfFadeOutStart(pSystem); break;
	}

	SoundManager(pSystem);
}
//********************************************************************************
//
// RankingTimeRender.cpp
//
// �����L���O����
//
// Date: 2019/10/10 �X�V
//
//********************************************************************************

//================================================================================
// �C���N���[�h
//================================================================================
#include "RankingTimeRender.hpp"

using namespace FileConst;

//================================================================================
// �N���X��`
//================================================================================

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
CRankingTimeRender::CRankingTimeRender()
	: m_vecPos(Vector2(0.0f, 0.0f))
	, m_vecSize(Vector2(80.0f, 80.0f))
	, m_time(0)
{
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
CRankingTimeRender::~CRankingTimeRender()
{
	for (auto w : m_pSprite)
	{
		SafeDelete(w);
	}
}

//--------------------------------------------------------------------------------
// ������
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CRankingTimeRender::Initialize(CSystem * pSystem)
{
	m_pSprite[ST_MIN1] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_NUMBER));
	m_pSprite[ST_MIN2] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_NUMBER));
	m_pSprite[ST_DOT] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_TIME_DOT));
	m_pSprite[ST_SEC1] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_NUMBER));
	m_pSprite[ST_SEC2] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_NUMBER));

	for (auto i = 0; i < ST_MAX; i++)
	{
		m_pSprite[i]->SetSize((UINT)m_vecSize.x, (UINT)m_vecSize.y);
	}
}

//--------------------------------------------------------------------------------
// �`��
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CRankingTimeRender::Draw(CSystem * pSystem)
{
	Update(pSystem);

	for (auto i = 0; i < ST_MAX; i++)
	{
		m_pSprite[i]->Draw(pSystem);
	}
}

//--------------------------------------------------------------------------------
// ���Ԃ̐ݒ�
// ���@���Fconst int �v����������
//--------------------------------------------------------------------------------
void CRankingTimeRender::SetTime(const int value)
{
	m_time = value / 1000;
}

//--------------------------------------------------------------------------------
// ���Ԃ̐ݒ�
// ���@���Fconst int �v����������
// ���@�l�FSetTime�ƈႢ�A����Ȃ��^�C�v
//--------------------------------------------------------------------------------
void CRankingTimeRender::SetRankTime(const int value)
{
	m_time = value;
}

//--------------------------------------------------------------------------------
// �V�����ʒu�֕�Ԃ���
// ���@���Fconst Vector2 2�����x�N�g��
//--------------------------------------------------------------------------------
void CRankingTimeRender::SetNewPos(const Vector2 value)
{
	for (auto i = 0; i < ST_MAX; i++)
	{
		m_pSprite[i]->SetNewPosition(Vector2(value.x - i * (m_vecSize.x / 3), value.y));
	}
}

//--------------------------------------------------------------------------------
// �V�����ʒu�֎w�肷��
// ���@���Fconst Vector2 2�����x�N�g��
//--------------------------------------------------------------------------------
void CRankingTimeRender::SetPos(const Vector2 value)
{
	for (auto i = 0; i < ST_MAX; i++)
	{
		m_pSprite[i]->SetPosition(Vector2(value.x - i * (m_vecSize.x / 3), value.y));
	}
}

//--------------------------------------------------------------------------------
// �����x�̎w�肷��
// ���@���Fconst Vector2 2�����x�N�g��
//--------------------------------------------------------------------------------
void CRankingTimeRender::SetNewAlpha(const BYTE alpha)
{
	for (auto i = 0; i < ST_MAX; i++)
	{
		m_pSprite[i]->SetNewAlpha(alpha);
	}
}

//--------------------------------------------------------------------------------
// �����x�̎w�肷��
// ���@���Fconst Vector2 2�����x�N�g��
//--------------------------------------------------------------------------------
void CRankingTimeRender::SetAlpha(const BYTE alpha)
{
	for (auto i = 0; i < ST_MAX; i++)
	{
		m_pSprite[i]->SetAlpha(alpha);
	}
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// �X�V
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CRankingTimeRender::Update(CSystem * pSystem)
{
	int timeMin = m_time / 60;
	int timeSec = m_time % 60;

	m_pSprite[ST_MIN1]->SetNumber(152, 1515, timeMin % 10); timeMin /= 10;
	m_pSprite[ST_MIN2]->SetNumber(152, 1515, timeMin % 10);
	m_pSprite[ST_SEC1]->SetNumber(152, 1515, timeSec % 10); timeSec /= 10;
	m_pSprite[ST_SEC2]->SetNumber(152, 1515, timeSec % 10);
}

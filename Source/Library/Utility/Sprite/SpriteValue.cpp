//*************************************************************
//
// SpriteValue.cpp
//
// ���l�̕`��
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "SpriteValue.h"

//-------------------------------------------------------------
// �R���X�g���N�^
// ���@���Fstd::string	�f�B���N�g��
//-------------------------------------------------------------
CSpriteValue::CSpriteValue(TEXTURE_DATA * pTextureData)
	: m_pTexData(pTextureData)
	, m_nowDigit(0)
	, m_oldDigit(0)
	, m_alpha(255)
	, m_vecPos(Vector2(0.0f, 0.0f))
	, m_vecSize(Vector2(0.0f, 0.0f))
	, m_endTime(0.0)
	, m_elapsedTime(0.0)
	, m_angle(0.0f)
	, m_isDestroy(false)
{
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
CSpriteValue::~CSpriteValue()
{
	for (auto val : m_vecSprite)
	{
		SafeDelete(val);
	}
	m_vecSprite.clear();
}

//-------------------------------------------------------------
// �`��
// ���@���FEngine*	�G���W���N���X�̃|�C���^
//-------------------------------------------------------------
void CSpriteValue::Draw(CSystem * pSystem)
{
	m_elapsedTime += pSystem->GetDeltaTime();

	if (m_elapsedTime > m_endTime && m_endTime != 0)
	{
		m_isDestroy = true;
	}

	//���݂̌����Ɖߋ��̌������ׂđ傫����Α��₷
	if (m_nowDigit > m_oldDigit)
	{
		for (int i = m_oldDigit; i < m_nowDigit; i++)
		{
			CSpriteAnimation* pWork = new CSpriteAnimation(m_pTexData);
			pWork->SetPosition(Vector2(m_vecPos.x - i * (m_vecSize.x / 3), m_vecPos.y));
			pWork->SetSize((UINT)m_vecSize.x, (UINT)m_vecSize.y);
			m_vecSprite.push_back(pWork);
		}
	}
	//�ߋ��̌����̂ق����傫����΍폜����
	else if (m_nowDigit < m_oldDigit)
	{
		for (auto it = m_vecSprite.begin() + m_nowDigit;
			it != m_vecSprite.begin() + m_oldDigit;
			it++)
		{
			SafeDelete(*it);
		}

		m_vecSprite.erase(
			m_vecSprite.begin() + m_nowDigit,
			m_vecSprite.begin() + m_oldDigit);
	}

	//�ʏ�ʂ�X�V
	for (size_t i = 0; i < m_vecSprite.size(); i++)
	{
		m_vecSprite[i]->SetPosition(Vector2(m_vecPos.x - i * (m_vecSize.x / 3), m_vecPos.y));
		m_vecSprite[i]->SetNumber(m_pTexData->uiWidth, m_pTexData->uiHeight, m_num[i]);
		m_vecSprite[i]->Draw(pSystem);
	}

	m_oldDigit = m_nowDigit;
}

//-------------------------------------------------------------
// �`�悷�鐔�l�̐ݒ�
// ���@���Fconst int ���l
//-------------------------------------------------------------
void CSpriteValue::SetValue(const int value)
{
	m_nowDigit = CalcDigit(value);
}

//-------------------------------------------------------------
// �ʒu�̐ݒ�
// ���@���Fconst Vector2	3�����x�N�g���̈ʒu
//-------------------------------------------------------------
void CSpriteValue::SetPosition(const Vector2 vecPos)
{
	m_vecPos = vecPos;
}

//-------------------------------------------------------------
// �����x�̐ݒ�
// ���@���Fconst int	�����x(�O�@�`�@�Q�T�T)
//-------------------------------------------------------------
void CSpriteValue::SetAlpha(const int alpha)
{
	m_alpha = alpha;
}

//-------------------------------------------------------------
// ��]�p�x�̐ݒ�
// ���@���Fconst float	���W�A���p
//-------------------------------------------------------------
void CSpriteValue::SetAngle(const float angle)
{
	m_angle = angle;
}

//-------------------------------------------------------------
// �傫���̐ݒ�
// ���@���Fconst UINT	������UINT
// �@�@�@�@const UINT	�c����UINT
//-------------------------------------------------------------
void CSpriteValue::SetSize(const UINT width, const UINT height)
{
	m_vecSize.x = static_cast<float>(width);
	m_vecSize.y = static_cast<float>(height);
}

//-------------------------------------------------------------
// �ʒu�̐ݒ�
// ���@���Fconst Vector2	3�����x�N�g���̈ʒu
// ���@�l�F�A�j���[�V�����g�p���ɗ��p
//-------------------------------------------------------------
void CSpriteValue::SetNewPosition(const Vector2 vecPos)
{
	m_vecPos = vecPos;

	for (size_t i = 0; i < m_vecSprite.size(); i++)
	{
		m_vecSprite[i]->SetNewPosition(
			Vector2(m_vecPos.x - i * (m_vecSize.x / 3), m_vecPos.y));
	}
}

//-------------------------------------------------------------
// �����x�̐ݒ�
// ���@���Fconst int	�����x(�O�@�`�@�Q�T�T)
// ���@�l�F�A�j���[�V�����g�p���ɗ��p
//-------------------------------------------------------------
void CSpriteValue::SetNewAlpha(const int alpha)
{
	m_alpha = alpha;

	for (size_t i = 0; i < m_vecSprite.size(); i++)
	{
		m_vecSprite[i]->SetNewAlpha(m_alpha);
	}
}

//-------------------------------------------------------------
// ��]�p�x�̐ݒ�
// ���@���Fconst float	���W�A���p
// ���@�l�F�A�j���[�V�����g�p���ɗ��p
//-------------------------------------------------------------
void CSpriteValue::SetNewAngle(const float angle)
{
	m_angle = angle;
}

//-------------------------------------------------------------
// �傫���̐ݒ�
// ���@���Fconst UINT	������UINT
// �@�@�@�@const UINT	�c����UINT
// ���@�l�F�A�j���[�V�����g�p���ɗ��p
//-------------------------------------------------------------
void CSpriteValue::SetNewSize(const UINT width, const UINT height)
{
	m_vecSize.x = static_cast<float>(width);
	m_vecSize.y = static_cast<float>(height);
}

//-------------------------------------------------------------
// �A�j���[�V�����I������
// ���@���Fconst DWORD	�I������(ms)
// ���@�l�Fms�Ȃ̂� [1�b] -> [1000ms]
//-------------------------------------------------------------
void CSpriteValue::SetEndTime(const double time)
{
	m_endTime = time;

	for (size_t i = 0; i < m_vecSprite.size(); i++)
	{
		m_vecSprite[i]->SetEndTime(time);
	}
}

//-------------------------------------------------------------
// �������ԏI���m�F
// �߂�l�Fconst bool	�������Ă��Ȃ����TRUE
//-------------------------------------------------------------
bool CSpriteValue::IsDestroy() const
{
	return m_isDestroy;
}

//*************************************************************
//
// private
//
//*************************************************************

//-------------------------------------------------------------
// ���̌v�Z
// ���@���Fconst int ���l
//-------------------------------------------------------------
BYTE CSpriteValue::CalcDigit(const int val)
{
	//���l�i�[
	int workVal = val;
	BYTE digit = 0;
	while (workVal != 0)
	{
		workVal /= 10;
		digit++;
	}

	//���l�i�[
	workVal = val;

	//�����␳(0�Ƃ������l���������Ȃ��ݒ�Ȃ̂� 0�Ȃ��1��������B)
	digit = digit != 0 ? digit : 1;

	for (int i = 0; i < digit; i++)
	{
		m_num[i] = (workVal % 10);
		workVal /= 10;
	}

	return digit;
}
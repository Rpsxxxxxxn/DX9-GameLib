//*************************************************************
//
// SpriteAnimation.cpp
//
// �`��A�j���[�V�����⏕�N���X
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "SpriteAnimation.h"
#include "../../System/System.h"

//=============================================================
// �N���X��`
//=============================================================

//-------------------------------------------------------------
// �R���X�g���N�^
	// ���@���FTEXTURE_DATA*	�e�N�X�`���f�[�^�̃|�C���^
//-------------------------------------------------------------
CSpriteAnimation::CSpriteAnimation(TEXTURE_DATA* pTextureData)
	: m_endtime(1000)
	, m_elapsedTime(0)
	, m_nowAnimation(false)
	, m_enablePosUpdate(false)
	, m_enableAlphaUpdate(false)
	, m_enableAngleUpdate(false)
	, m_enableSizeUpdate(false)
{
	m_pTexture = pTextureData->pTexture;
	m_uiWidth = pTextureData->uiWidth;
	m_uiHeight = pTextureData->uiHeight;
	for (int i = 0; i < MAX; i++)
	{
		m_data[i].vecPos = D3DXVECTOR2(0.0f, 0.0f);
		m_data[i].angle = 0.0f;
		m_data[i].alpha = 255;
		m_data[i].uiWidth = pTextureData->uiWidth;
		m_data[i].uiHeight = pTextureData->uiHeight;
	}

	SetRect(&m_rSour, 0, 0, (int)m_uiWidth, (int)m_uiHeight);
}

//-------------------------------------------------------------
// �`��
// ���@���FEngine*	�G���W���N���X�̃|�C���^
//-------------------------------------------------------------
void CSpriteAnimation::Draw(CSystem* pSystem)
{
	Update(pSystem);

	SetRect(&m_rDest,
		(int)m_data[NOW].vecPos.x,
		(int)m_data[NOW].vecPos.y,
		(int)m_data[NOW].vecPos.x + m_data[NOW].uiWidth,
		(int)m_data[NOW].vecPos.y + m_data[NOW].uiHeight
	);

	pSystem->SpriteBegin();

	pSystem->DrawSprite(
		&m_rSour,
		m_pTexture,
		&m_rDest,
		m_data[NOW].alpha,
		m_data[NOW].angle);

	pSystem->SpriteClose();
}

//-------------------------------------------------------------
// �ʒu�̐ݒ�
// ���@���Fconst D3DXVECTOR2	3�����x�N�g���̈ʒu
//-------------------------------------------------------------
void CSpriteAnimation::SetPosition(const D3DXVECTOR2 vecPos)
{
	m_data[NOW].vecPos = vecPos;
}

//-------------------------------------------------------------
// �����x�̐ݒ�
// ���@���Fconst D3DXVECTOR2	3�����x�N�g���̈ʒu
//-------------------------------------------------------------
void CSpriteAnimation::SetAlpha(const int alpha)
{
	m_data[NOW].alpha = alpha;
}

//-------------------------------------------------------------
// ��]�p�x�̐ݒ�
// ���@���Fconst D3DXVECTOR2	3�����x�N�g���̈ʒu
//-------------------------------------------------------------
void CSpriteAnimation::SetAngle(const float angle)
{
	m_data[NOW].angle = angle;
}

//-------------------------------------------------------------
// �傫���̐ݒ�
// ���@���Fconst UINT	������UINT
// �@�@�@�@const UINT	�c����UINT
//-------------------------------------------------------------
void CSpriteAnimation::SetSize(const UINT width, const UINT height)
{
	if (width != 0) m_data[NOW].uiWidth = width;
	if (height != 0) m_data[NOW].uiHeight = height;
}

//-------------------------------------------------------------
// �傫���̐ݒ�
// ���@���Fconst UINT	������UINT
//-------------------------------------------------------------
void CSpriteAnimation::SetSizeX(const UINT width)
{
	m_data[NOW].uiWidth = width;
}

//-------------------------------------------------------------
// �傫���̐ݒ�
// ���@���Fconst UINT	�c����UINT
//-------------------------------------------------------------
void CSpriteAnimation::SetSizeY(const UINT height)
{
	m_data[NOW].uiHeight = height;
}

//-------------------------------------------------------------
// �ʒu�̐ݒ�
// ���@���Fconst D3DXVECTOR2	3�����x�N�g���̈ʒu
//-------------------------------------------------------------
void CSpriteAnimation::SetNewPosition(const D3DXVECTOR2 vecPos)
{
	m_data[NEW].vecPos = vecPos;
	m_data[OLD].vecPos = m_data[NOW].vecPos;
	m_elapsedTime = 0;
	m_nowAnimation = true;
	m_enablePosUpdate = true;
}

//-------------------------------------------------------------
// �����x�̐ݒ�
// ���@���Fconst D3DXVECTOR2	3�����x�N�g���̈ʒu
//-------------------------------------------------------------
void CSpriteAnimation::SetNewAlpha(const int alpha)
{
	m_data[NEW].alpha = alpha;
	m_data[OLD].alpha = m_data[NOW].alpha;
	m_elapsedTime = 0;
	m_nowAnimation = true;
	m_enableAlphaUpdate = true;
}

//-------------------------------------------------------------
// ��]�p�x�̐ݒ�
// ���@���Fconst D3DXVECTOR2	3�����x�N�g���̈ʒu
//-------------------------------------------------------------
void CSpriteAnimation::SetNewAngle(const float angle)
{
	m_data[NEW].angle = angle;
	m_data[OLD].angle = m_data[NOW].angle;
	m_elapsedTime = 0;
	m_nowAnimation = true;
	m_enableAngleUpdate = true;
}

//-------------------------------------------------------------
// �傫���̐ݒ�
// ���@���Fconst UINT	������UINT
// �@�@�@�@const UINT	�c����UINT
// ���@�l�F�A�j���[�V�����g�p���ɗ��p
//-------------------------------------------------------------
void CSpriteAnimation::SetNewSize(const UINT width, const UINT height)
{
	if (width != 0)
	{
		m_data[NEW].uiWidth = width;
		m_data[OLD].uiWidth = m_data[NOW].uiWidth;
	}

	if (width != 0)
	{
		m_data[NEW].uiHeight = height;
		m_data[OLD].uiHeight = m_data[NOW].uiHeight;
	}
	m_elapsedTime = 0;
	m_nowAnimation = true;
	m_enableSizeUpdate = true;
}

//-------------------------------------------------------------
// �A�j���[�V�����I������
// ���@���Fconst D3DXVECTOR2	3�����x�N�g���̈ʒu
//-------------------------------------------------------------
void CSpriteAnimation::SetEndTime(const double time)
{
	m_endtime = time;
}

//-------------------------------------------------------------
// ���l�̐ݒ�
// ���@���Fconst int	����
// �@�@�@�@const int	�c��
// �@�@�@�@const int	���l
//-------------------------------------------------------------
void CSpriteAnimation::SetNumber(
	const int width,
	const int height,
	const int num
)
{
	//���̂ق����傫���Ƃ�
	if (width > height)
	{
		int numWidth = width / 10;

		int nWidth = numWidth * num;

		SetRect(&m_rSour, nWidth, 0, nWidth + numWidth, height);
	}
	//�c�̂ق����傫���Ƃ�
	else
	{
		int numHeight = height / 10;

		int nHeight = numHeight * num;

		SetRect(&m_rSour, 0, nHeight, width, nHeight + numHeight);
	}
}

//-------------------------------------------------------------
// �e�N�X�`���f�[�^�̐ݒ�
// ���@���FTEXTURE_DATA*	�e�N�X�`���f�[�^�̃|�C���^
//-------------------------------------------------------------
void CSpriteAnimation::SetTextureData(TEXTURE_DATA * pTextureData)
{
	m_pTexture = pTextureData->pTexture;
	m_uiWidth = pTextureData->uiWidth;
	m_uiHeight = pTextureData->uiHeight;
	for (int i = 0; i < MAX; i++)
	{
		m_data[i].uiWidth = pTextureData->uiWidth;
		m_data[i].uiHeight = pTextureData->uiHeight;
	}

	SetRect(&m_rSour, 0, 0, (int)m_uiWidth, (int)m_uiHeight);
}

//*************************************************************
//
// private
//
//*************************************************************

//-------------------------------------------------------------
// �X�V
// ���@���FEngine*	�G���W���N���X�̃|�C���^
//-------------------------------------------------------------
void CSpriteAnimation::Update(CSystem * pSystem)
{
	if (m_elapsedTime < m_endtime)
	{
		if (!m_nowAnimation) return;

		m_elapsedTime += pSystem->GetDeltaTime();

		if (m_enablePosUpdate)
		{
			m_data[NOW].vecPos.x = 
				(float)Calculate::OutQuart(
					m_elapsedTime, 
					m_endtime, 
					m_data[NEW].vecPos.x,
					m_data[OLD].vecPos.x);

			m_data[NOW].vecPos.y = 
				(float)Calculate::OutQuart(
					m_elapsedTime,
					m_endtime,
					m_data[NEW].vecPos.y,
					m_data[OLD].vecPos.y);
		}

		if (m_enableAngleUpdate)
		{
			m_data[NOW].angle = 
				(float)Calculate::OutQuart(
					m_elapsedTime,
					m_endtime, 
					m_data[NEW].angle, 
					m_data[OLD].angle);
		}

		if (m_enableAlphaUpdate)
		{
			m_data[NOW].alpha = 
				(int)Calculate::OutQuart(
					m_elapsedTime,
					m_endtime, 
					m_data[NEW].alpha,
					m_data[OLD].alpha);
		}

		if (m_enableSizeUpdate)
		{
			m_data[NOW].uiWidth = 
				(UINT)Calculate::OutQuart(
					m_elapsedTime,
					m_endtime, 
					(double)m_data[NEW].uiWidth,
					(double)m_data[OLD].uiWidth);

			m_data[NOW].uiHeight = 
				(UINT)Calculate::OutQuart(
					m_elapsedTime,
					m_endtime, 
					(double)m_data[NEW].uiHeight, 
					(double)m_data[OLD].uiHeight);
		}
	}
	else {
		m_elapsedTime = 0;
		m_nowAnimation = false;
		m_enablePosUpdate = false;
		m_enableAngleUpdate = false;
		m_enableAlphaUpdate = false;
		m_enableSizeUpdate = false;
	}
}
//*************************************************************
//
// Sprite.cpp
//
// �`��N���X
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "Sprite.h"

//=============================================================
// �N���X��`
//=============================================================

//-------------------------------------------------------------
// �V���O���g������
//-------------------------------------------------------------
CSprite * CSprite::Create()
{
	static CSprite instance;
	return &instance;
}

//-------------------------------------------------------------
// ������
// ���@���FLPDIRECT3DDEVICE9	�f�o�C�X�̃|�C���^
//-------------------------------------------------------------
void CSprite::Initialize(LPDIRECT3DDEVICE9 pDevice)
{
	HRESULT hr = D3DXCreateSprite(pDevice, &m_pSprite);
	if (FAILED(hr)) return;
}

//-------------------------------------------------------------
// �`��
// ���@���FRECT*				�\�[�X�摜�̃}�X�N
// �@�@�@�@LPDIRECT3DTEXTURE9	�e�N�X�`���̃|�C���^
// �@�@�@�@RECT*				�`���̈ʒu
// �@�@�@�@DWORD				�����x
//-------------------------------------------------------------
void CSprite::Draw(
	RECT * pSrcRect, 
	LPDIRECT3DTEXTURE9 pTexture,
	RECT * pDstRect, 
	DWORD dwColor,
	float angle
)
{
	Matrix mWorld, mTrans, mScale, mRot;

	Vector2 Scale(
		(float)(pDstRect->right - pDstRect->left) / (float)(pSrcRect->right - pSrcRect->left),
		(float)(pDstRect->bottom - pDstRect->top) / (float)(pSrcRect->bottom - pSrcRect->top)
	);

	Vector2 SrcRectCenter = 
		Vector2((pSrcRect->right - pSrcRect->left) / 2.0f, (pSrcRect->bottom - pSrcRect->top) / 2.0f);

	Vector2 DstRectCenter = 
		Vector2((pDstRect->right - pDstRect->left) / 2.0f, (pDstRect->bottom - pDstRect->top) / 2.0f);

	Vector3 SrcCenter = Vector3(SrcRectCenter.x, SrcRectCenter.y, 0.0f);

	D3DXMatrixScaling(&mScale, Scale.x, Scale.y, 1.0f);

	D3DXMatrixRotationZ(&mRot, angle);

	D3DXMatrixTranslation(&mTrans, (float)pDstRect->left + DstRectCenter.x, (float)pDstRect->top + DstRectCenter.y, 0.0f);

	mWorld = mScale * mRot * mTrans;

	m_pSprite->SetTransform(&mWorld);

	m_pSprite->Draw(pTexture, pSrcRect, &SrcCenter, NULL, D3DCOLOR_ARGB(dwColor & 0x000000FF, 255, 255, 255));

	D3DXMatrixIdentity(&mWorld);

	m_pSprite->SetTransform(&mWorld);
}

//-------------------------------------------------------------
// �X�v���C�g�����_�����O�J�n
// ���@�l�F�`��O�ɌĂяo����K��
//-------------------------------------------------------------
void CSprite::SpriteBegin()
{
	m_pSprite->Begin(0);
}

//-------------------------------------------------------------
// �X�v���C�g�����_�����O�I��
// ���@�l�F�`���ɌĂяo����K��
//-------------------------------------------------------------
void CSprite::SpriteClose()
{
	m_pSprite->End();
}

//-------------------------------------------------------------
// �X�v���C�g�̃|�C���^�擾
//-------------------------------------------------------------
LPD3DXSPRITE CSprite::GetSprite()
{
	return m_pSprite;
}

//-------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------
CSprite::CSprite()
	: m_pSprite(nullptr)
{
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
CSprite::~CSprite()
{
	SafeRelease(m_pSprite);
}

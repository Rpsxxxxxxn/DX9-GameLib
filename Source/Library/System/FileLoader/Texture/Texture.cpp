//*************************************************************
//
// Texture.cpp
//
// Texture�t�@�C���ǂݍ��ݗp�N���X
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "Texture.h"

//=============================================================
// �N���X��`
//=============================================================

//-------------------------------------------------------------
// �R���X�g���N�^
// ���@���FLPDIRECT3DDEVICE9	�f�o�C�X�̃|�C���^
// �@�@�@�@string				�t�@�C���p�X
//-------------------------------------------------------------
CTexture::CTexture(LPDIRECT3DDEVICE9 pDevice, string strFileName)
	: CFileBase(pDevice)
{
	m_texData.pTexture = nullptr;
	m_texData.uiWidth = 0;
	m_texData.uiHeight = 0;

	this->Initialize(strFileName);
}

//-------------------------------------------------------------
// ������
// ���@���Fstring	�t�@�C���f�B���N�g��
//-------------------------------------------------------------
void CTexture::Initialize(string strFileName)
{
	D3DXIMAGE_INFO imageInfo;

	//�C���[�W�t�@�C���̏��擾
	D3DXGetImageInfoFromFile(
		(LPCSTR)strFileName.c_str(), &imageInfo);

	m_texData.uiWidth = imageInfo.Width;
	m_texData.uiHeight = imageInfo.Height;

	//�e�N�X�`���̐���
	if (FAILED(D3DXCreateTextureFromFileExA(
		m_pDevice,
		(LPCSTR)strFileName.c_str(),
		imageInfo.Width,
		imageInfo.Height,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,
		D3DX_FILTER_NONE,
		0,
		NULL,
		NULL,
		&m_texData.pTexture
	)))
	{
		//�ǂݍ��߂Ȃ�����
		MessageBox(nullptr, "�e�N�X�`���̐����Ɏ��s", "�G���[", MB_OK);
	}
}

//-------------------------------------------------------------
// ���
//-------------------------------------------------------------
void CTexture::Release()
{
	SafeRelease(m_texData.pTexture);
}

//-------------------------------------------------------------
// �e�N�X�`���̎擾
//-------------------------------------------------------------
LPDIRECT3DTEXTURE9 CTexture::GetTexture()
{
	return m_texData.pTexture;
}

//-------------------------------------------------------------
// �e�N�X�`���\����
//-------------------------------------------------------------
TEXTURE_DATA * CTexture::GetTextureData()
{
	return &m_texData;
}

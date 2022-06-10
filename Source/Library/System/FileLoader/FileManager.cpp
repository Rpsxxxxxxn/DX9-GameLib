//*************************************************************
//
// FileManager.cpp
//
// �t�@�C���Ǘ��p�N���X
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "FileManager.h"

//-------------------------------------------------------------
// �C���X�^���X�̎擾
//-------------------------------------------------------------
CFileManager * CFileManager::Create()
{
	static CFileManager instance;
	return &instance;
}

//-------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------
CFileManager::CFileManager(){}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
CFileManager::~CFileManager()
{
	this->Release();
}

//-------------------------------------------------------------
// ������
//-------------------------------------------------------------
void CFileManager::Initialize(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
}

//-------------------------------------------------------------
// ���
//-------------------------------------------------------------
void CFileManager::Release()
{
	if (!m_aryFile.empty())
	{
		for (auto itr = m_aryFile.begin(); itr != m_aryFile.end(); itr++)
		{
			SafeDelete(itr->second);
		}

		m_aryFile.clear();
	}
}

//-------------------------------------------------------------
// XFile�t�@�C���ǂݍ���
// ���@���Fconst int 	�t�@�C���̌Œ�ԍ�
// �@�@�@�@const string	�t�@�C����
//-------------------------------------------------------------
void CFileManager::LoadXFile(const int index, const string strFileName)
{
	//�t�@�C���̌���
	auto itr = m_aryFile.find(index);
	if ( itr != m_aryFile.end() ) return;

	//�t�@�C���̐���
	CFileBase* pFile = new CXFile(m_pDevice, strFileName);
	if (pFile == nullptr) return;

	m_aryFile.insert(make_pair(index, pFile));
}

//-------------------------------------------------------------
// Texture�t�@�C���ǂݍ���
// ���@���Fconst int 	�t�@�C���̌Œ�ԍ�
// �@�@�@�@const string	�t�@�C����
//-------------------------------------------------------------
void CFileManager::LoadTexture(const int index, const string strFileName)
{
	//�t�@�C���̌���
	auto itr = m_aryFile.find(index);
	if (itr != m_aryFile.end()) return;

	CFileBase* pFile = new CTexture(m_pDevice, strFileName);
	if (pFile == nullptr) return;

	m_aryFile.insert(make_pair(index, pFile));
}

//-------------------------------------------------------------
// ���[�h�����t�@�C���̉��(�ȃ�����)
//-------------------------------------------------------------
void CFileManager::EraseData(const int index)
{
	//�t�@�C���̌���
	auto itr = m_aryFile.find(index);
	if (itr == m_aryFile.end()) return;

	SafeDelete(itr->second);
}

//-------------------------------------------------------------
// �e�N�X�`���̃|�C���^�擾
// �߂�l�FLPDIRECT3DTEXTURE9	�e�N�X�`��
//-------------------------------------------------------------
LPDIRECT3DTEXTURE9 CFileManager::GetTexture(const int index)
{
	//�t�@�C���̌���
	auto itr = m_aryFile.find(index);
	if ( itr == m_aryFile.end() ) return nullptr;

	return dynamic_cast<CTexture*>(itr->second)->GetTexture();
}

//-------------------------------------------------------------
// �e�N�X�`���\���̂̎擾
// �߂�l�FTEXTURE_DATA*		�e�N�X�`��
//-------------------------------------------------------------
TEXTURE_DATA * CFileManager::GetTextureData(const int index)
{
	//�t�@�C���̌���
	auto itr = m_aryFile.find(index);
	if (itr == m_aryFile.end()) return nullptr;

	return dynamic_cast<CTexture*>(itr->second)->GetTextureData();
}

//-------------------------------------------------------------
// XFile�N���X�̃|�C���^�擾
// �߂�l�FCXFile*	XFile���f���N���X
//-------------------------------------------------------------
CXFile * CFileManager::GetXFile(const int index)
{
	//�t�@�C���̌���
	auto itr = m_aryFile.find(index);
	if (itr == m_aryFile.end()) return nullptr;

	return dynamic_cast<CXFile*>(itr->second);
}
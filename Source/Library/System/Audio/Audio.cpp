//*************************************************************
//
// Audio.cpp
//
// �I�[�f�B�I�Đ��p�N���X
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "Audio.h"

//-------------------------------------------------------------
// �V���O���g������
//-------------------------------------------------------------
CAudio * CAudio::Create()
{
	static CAudio instance;
	return &instance;
}

//-------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------
CAudio::CAudio()
	: m_pXAudio2(nullptr)
	, m_pXAudioMaster(nullptr)
{
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
CAudio::~CAudio()
{
	//�S�Ẳ����f�[�^���폜
	Release();

	// MasteringVoice����������Ă���Δj��
	if (m_pXAudioMaster != nullptr)
	{
		m_pXAudioMaster->DestroyVoice();
		m_pXAudioMaster = nullptr;
	}

	// XAudio2����������Ă���Δj��
	SafeRelease(m_pXAudio2);

	// COM�̔j��
	CoUninitialize();
}

//-------------------------------------------------------------
// Bgm�̒ǉ�
//-------------------------------------------------------------
void CAudio::AddBgm(const int index, string strFileName)
{
	auto itr = m_mapBgm.find(index);
	if (itr != m_mapBgm.end()) return;

	auto bgm = new CBgm(m_pXAudio2, strFileName);

	m_mapBgm.insert(make_pair(index, bgm));
}

//-------------------------------------------------------------
// Se�̒ǉ�
//-------------------------------------------------------------
void CAudio::AddSe(const int index, string strFileName)
{
	auto itr = m_mapSe.find(index);
	if (itr != m_mapSe.end()) return;

	auto se = new CSe(m_pXAudio2, strFileName);

	m_mapSe.insert(make_pair(index, se));
}

//-------------------------------------------------------------
// ������
//-------------------------------------------------------------
void CAudio::Initialize()
{
	HRESULT hr;

	// COM���C�u����������
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr)) MessageBox(nullptr, "COM���C�u����������", "�G���[", MB_OK);

	// XAudio2�̃C���X�^���X�𐶐�
	hr = XAudio2Create(&m_pXAudio2, 0);
	if (FAILED(hr)) MessageBox(nullptr, "XAudio2�̃C���X�^���X�𐶐�", "�G���[", MB_OK);

	// MasteringVoice�̐���
	hr = m_pXAudio2->CreateMasteringVoice(&m_pXAudioMaster);
	if (FAILED(hr)) MessageBox(nullptr, "MasteringVoice�̐���", "�G���[", MB_OK);

	//���ʐݒ�
	m_pXAudioMaster->SetVolume(0.0);
}

//-------------------------------------------------------------
// �X�V
//-------------------------------------------------------------
void CAudio::Update()
{
	for (auto obj : m_mapBgm)
	{
		obj.second->Update();
	}

	for (auto obj : m_mapSe)
	{
		obj.second->Update();
	}
}

//-------------------------------------------------------------
// Bgm�Đ�
//-------------------------------------------------------------
void CAudio::PlayBgm(const int index)
{
	m_mapBgm[index]->Play();
}

//-------------------------------------------------------------
// Bgm��~
//-------------------------------------------------------------
void CAudio::StopBgm(const int index)
{
	m_mapBgm[index]->Stop();
}

//-------------------------------------------------------------
// Se�Đ�
// ���@���Fstring	�ǉ�����Se��
//-------------------------------------------------------------
void CAudio::PlaySe(const int index)
{
	m_mapSe[index]->Play();
}

//-------------------------------------------------------------
// Bgm��0�ɂ��ǂ�
// ���@���Fstring	�ǉ�����Bgm��
//-------------------------------------------------------------
void CAudio::RewindBgm(const int index)
{
	m_mapBgm[index]->Rewind();
}

//-------------------------------------------------------------
// �ǉ�����Bgm�̍폜
// ���@���Fconst string	�ǉ�����Bgm��
//-------------------------------------------------------------
void CAudio::EraseBgm(const int index)
{
	//�t�@�C���̌���
	auto itr = m_mapBgm.find(index);
	if (itr == m_mapBgm.end()) return;

	SafeDelete(itr->second);
}

//-------------------------------------------------------------
// �ǉ�����Se�̍폜
// ���@���Fconst string	�ǉ�����Se��
//-------------------------------------------------------------
void CAudio::EraseSe(const int index)
{
	//�t�@�C���̌���
	auto itr = m_mapSe.find(index);
	if (itr == m_mapSe.end()) return;

	SafeDelete(itr->second);
}

//-------------------------------------------------------------
// �S�Ẳ����f�[�^���폜
//-------------------------------------------------------------
void CAudio::Release()
{
	for (auto obj : m_mapBgm)
	{
		SafeDelete(obj.second);
	}
	m_mapBgm.clear();

	for (auto obj : m_mapSe)
	{
		SafeDelete(obj.second);
	}
	m_mapSe.clear();
}

//-------------------------------------------------------------
// �{�����[���̐ݒ�
//-------------------------------------------------------------
void CAudio::SetVolume(float volume)
{
	m_pXAudioMaster->SetVolume(volume);
}

//-------------------------------------------------------------
// �I�[�f�B�I�̎擾
// �߂�l�FIXAudio2	�I�[�f�B�I�N���X�̃|�C���^
//-------------------------------------------------------------
IXAudio2 * CAudio::GetXAudio()
{
	return m_pXAudio2;
};

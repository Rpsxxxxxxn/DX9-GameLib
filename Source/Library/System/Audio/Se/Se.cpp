//*************************************************************
//
// Se.cpp
//
// Se�t�@�C���ǂݍ��ݗp�N���X
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "Se.h"

//=============================================================
// �萔��`
//=============================================================
const int bufLen = 2;

//-------------------------------------------------------------
// �R���X�g���N�^
// ���@���FIXAudio2*	�I�[�f�B�I�C���^�[�t�F�[�X
// �@�@�@�@string		�t�@�C���p�X
//-------------------------------------------------------------
CSe::CSe(IXAudio2* pXAudio, string name)
	: m_pXAudio2(pXAudio)
	, m_mmio(nullptr)
	, bufCnt(0)
	, m_volume(1.0f)
{
	this->Initialize(name);
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
CSe::~CSe()
{
	this->Release();
}

//-------------------------------------------------------------
// ������
// ���@���Fstring	�t�@�C���f�B���N�g��
//-------------------------------------------------------------
void CSe::Initialize(string name)
{
	this->ReadFile(name);
	this->CreateBuffer(m_pData);
}

//-------------------------------------------------------------
// �X�V
//-------------------------------------------------------------
void CSe::Update()
{
	for (auto it = begin(m_vecSour); it != end(m_vecSour);)
	{
		XAUDIO2_VOICE_STATE state;
		(*it)->GetState(&state);

		//�o�b�t�@�L���[���Ȃ��Ȃ�Ώ�������
		if (state.BuffersQueued == 0)
		{
			(*it)->DestroyVoice();
			it = m_vecSour.erase(it);
		}
		else {
			it++;
		}
	}
}

//-------------------------------------------------------------
// �J��
//-------------------------------------------------------------
void CSe::Release()
{
	for (int i = 0; i < bufLen; i++)
		SafeDeleteArray(m_pBuf);

	for (auto w : m_vecSour)
		w->DestroyVoice();

	m_vecSour.clear();
}

//-------------------------------------------------------------
// �Đ�
//-------------------------------------------------------------
void CSe::Play()
{
	this->AddSe();
}

//-------------------------------------------------------------
// �{�����[���̐ݒ�
// ���@���Ffloat	���̑傫��
//-------------------------------------------------------------
void CSe::SetVolume(float volume)
{
	m_volume = volume;

	for (auto w : m_vecSour)
		w->SetVolume(volume);
}

//-------------------------------------------------------------
// �{�����[���̎擾
// �߂�l�Ffloat	���݂̑傫��
//-------------------------------------------------------------
float CSe::GetVolume()
{
	return m_volume;
}

//-------------------------------------------------------------
// Se�̒ǉ�1
//-------------------------------------------------------------
void CSe::AddSe()
{
	IXAudio2SourceVoice* pWork;

	m_pXAudio2->CreateSourceVoice(&pWork, &m_fmt);

	pWork->SetVolume(m_volume);
	m_vecSour.push_back(pWork);
	pWork->FlushSourceBuffers();
	pWork->SubmitSourceBuffer(&m_pBuffer);
	pWork->Start();
}

//-------------------------------------------------------------
// �t�@�C���ǂݍ���
//-------------------------------------------------------------
void CSe::ReadFile(string name)
{
	MMIOINFO info = { 0 };
	MMRESULT mret;
	MMCKINFO riff_chunk;
	MMCKINFO chunk;

	m_mmio = mmioOpen((LPSTR)name.c_str(), &info, MMIO_READ);
	if (!m_mmio) MessageBox(NULL, "MMIO�I�[�v�����s", "�G���[", MB_OK); 

	riff_chunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mret = mmioDescend(m_mmio, &riff_chunk, NULL, MMIO_FINDRIFF);
	if (mret != MMSYSERR_NOERROR) MessageBox(NULL, "wave", "�G���[", MB_OK); 

	chunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mret = mmioDescend(m_mmio, &chunk, &riff_chunk, MMIO_FINDCHUNK);
	if (mret != MMSYSERR_NOERROR) MessageBox(NULL, "fmt", "�G���[", MB_OK); 

	memset(&m_fmt, 0, sizeof(WAVEFORMATEX));
	{
		DWORD size = mmioRead(m_mmio, (HPSTR)&m_fmt, chunk.cksize);
		if (size != chunk.cksize) MessageBox(NULL, "fmt", "�G���[", MB_OK); 
	}

	chunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mret = mmioDescend(m_mmio, &chunk, &riff_chunk, MMIO_FINDCHUNK);
	if (mret != MMSYSERR_NOERROR) MessageBox(NULL, "data", "�G���[", MB_OK);

	m_pData = chunk.cksize;
}

//-------------------------------------------------------------
// �o�b�t�@�̐���
// ���@���Fconst int �o�b�t�@�J�E���g
// �@�@�@�Fconst int �o�b�t�@�̒���
//-------------------------------------------------------------
void CSe::CreateBuffer(const int bufferLen)
{
	m_pBuffer = XAUDIO2_BUFFER();
	m_pBuf = new BYTE[bufferLen];
	int size = mmioRead(m_mmio, (HPSTR)m_pBuf, bufferLen);
	m_pBuffer.AudioBytes = size;
	m_pBuffer.pAudioData = m_pBuf;
}
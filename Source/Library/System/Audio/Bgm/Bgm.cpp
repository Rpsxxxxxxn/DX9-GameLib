//*************************************************************
//
// Bgm.cpp
//
// Bgm�t�@�C���ǂݍ��ݗp�N���X
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "Bgm.h"

//=============================================================
// �萔��`
//=============================================================
const int bufLen = 2;

//-------------------------------------------------------------
// �R���X�g���N�^
// ���@���FIXAudio2*	�I�[�f�B�I�C���^�[�t�F�[�X
// �@�@�@�@string		�t�@�C���p�X
//-------------------------------------------------------------
CBgm::CBgm(IXAudio2* pXAudio, string name)
	: m_pXAudio2(pXAudio)
	, m_mmio(nullptr)
	, bufCnt(0)
	, m_loaded(0)
	, m_loopPoint(0)
{
	Initialize(name);
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
CBgm::~CBgm()
{
	Release();
}

//-------------------------------------------------------------
// ������
// ���@���Fstring	�t�@�C���f�B���N�g��
//-------------------------------------------------------------
void CBgm::Initialize(string name)
{
	ReadFile(name);
	m_pBuffer = { 0 };

	for (int i = 0; i < bufLen; i++)
		m_pBuf[i] = new BYTE[m_fmt.nAvgBytesPerSec];

	HRESULT hr = m_pXAudio2->CreateSourceVoice(&m_pSource, &m_fmt);
	if (FAILED(hr))	MessageBox(NULL, "CreateSourceVoice", "�G���[", MB_OK);
}

//-------------------------------------------------------------
// �X�V
//-------------------------------------------------------------
void CBgm::Update()
{
	XAUDIO2_VOICE_STATE state;
	m_pSource->GetState(&state);

	if (state.BuffersQueued < 2)
	{
		if (m_loaded > m_chunkSize)
		{
			Seek(m_loopPoint, SEEK_SET);
			m_loaded = m_loopPoint;
		}

		if (m_fmt.nAvgBytesPerSec <= 0) return;

		CreateBuffer(bufCnt, m_fmt.nAvgBytesPerSec);

		SetBuffer();

		if (bufLen <= ++bufCnt) bufCnt = 0;
	}
}

//-------------------------------------------------------------
// �J��
//-------------------------------------------------------------
void CBgm::Release()
{
	Stop();

	for (int i = 0; i < bufLen; i++)
		SafeDeleteArray(m_pBuf[i]);

	m_pSource->DestroyVoice();
}

//-------------------------------------------------------------
// �V�[�N
//-------------------------------------------------------------
bool CBgm::Seek(int offset, int origin)
{
	if (mmioSeek(m_mmio, offset, origin))
	{
		MessageBox(nullptr, "mmioSeek", "�G���[", MB_OK);
		return false;
	}

	m_chunkSize -= offset;

	return true;
}

//-------------------------------------------------------------
// �Đ�
//-------------------------------------------------------------
void CBgm::Play()
{
	m_pSource->Start();
}

//-------------------------------------------------------------
// ��~
//-------------------------------------------------------------
void CBgm::Stop()
{
	m_pSource->Stop();
}

//-------------------------------------------------------------
// �����߂�
//-------------------------------------------------------------
void CBgm::Rewind()
{
	Seek(m_loopPoint, SEEK_SET);
	m_loaded = m_loopPoint;
}

//-------------------------------------------------------------
// �{�����[���̐ݒ�
// ���@���Ffloat	���̑傫��
//-------------------------------------------------------------
void CBgm::SetVolume(float volume)
{
	m_pSource->SetVolume(volume);
}

//-------------------------------------------------------------
// �{�����[���̎擾
// �߂�l�Ffloat	���݂̑傫��
//-------------------------------------------------------------
float CBgm::GetVolume()
{
	float volume;
	m_pSource->GetVolume(&volume);
	return volume;
}

//-------------------------------------------------------------
// �t�@�C���ǂݍ���
//-------------------------------------------------------------
void CBgm::ReadFile(string name)
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

	m_chunkSize = chunk.cksize;
}

//-------------------------------------------------------------
// �o�b�t�@�̐ݒ�
//-------------------------------------------------------------
void CBgm::SetBuffer()
{
	m_pSource->SubmitSourceBuffer(&m_pBuffer);
}

//-------------------------------------------------------------
// �o�b�t�@�̐���
// ���@���Fconst int �o�b�t�@�J�E���g
// �@�@�@�Fconst int �o�b�t�@�̒���
//-------------------------------------------------------------
void CBgm::CreateBuffer(const int bufferCnt, const int bufferLen)
{
	m_pBuffer = XAUDIO2_BUFFER();
	int size = mmioRead(m_mmio, (HPSTR)m_pBuf[bufferCnt], bufferLen);
	m_pBuffer.AudioBytes = size;
	m_pBuffer.pAudioData = m_pBuf[bufferCnt];
	m_loaded += size;
}

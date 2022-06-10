//*************************************************************
//
// Audio.hpp
//
// �I�[�f�B�I�Đ��p�N���X
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

#pragma once

//=============================================================
// �C���N���[�h
//=============================================================
#include "../../Common.h"
#include "Bgm/Bgm.h"
#include "Se/Se.h"

//=============================================================
// �N���X��`
//=============================================================
class CAudio
{
public:

	//-------------------------------------------------------------
	// �V���O���g������
	//-------------------------------------------------------------
	static CAudio* Create();

	//-------------------------------------------------------------
	// �R���X�g���N�^
	//-------------------------------------------------------------
	CAudio();

	//-------------------------------------------------------------
	// �f�X�g���N�^
	//-------------------------------------------------------------
	~CAudio();

	//-------------------------------------------------------------
	// Bgm�̒ǉ�
	// ���@���Fstring	����
	// �@�@�@�FCBgm*	Bgm�N���X�̃|�C���^
	//-------------------------------------------------------------
	void AddBgm(const int index, string strFileName);

	//-------------------------------------------------------------
	// Se�̒ǉ�
	// ���@���Fstring	����
	// �@�@�@�FCSe*		Se�N���X�̃|�C���^
	//-------------------------------------------------------------
	void AddSe(const int index, string strFileName);

	//-------------------------------------------------------------
	// ������
	//-------------------------------------------------------------
	void Initialize();

	//-------------------------------------------------------------
	// �X�V
	//-------------------------------------------------------------
	void Update();

	//-------------------------------------------------------------
	// Bgm�Đ�
	// ���@���Fconst int	�ǉ�����Bgm��
	//-------------------------------------------------------------
	void PlayBgm(const int index);

	//-------------------------------------------------------------
	// Bgm��~
	// ���@���Fconst int	�ǉ�����Bgm��
	//-------------------------------------------------------------
	void StopBgm(const int index);

	//-------------------------------------------------------------
	// Se�Đ�
	// ���@���Fconst int	�ǉ�����Se��
	//-------------------------------------------------------------
	void PlaySe(const int index);

	//-------------------------------------------------------------
	// �����߂�
	// ���@���Fconst int	�ǉ�����Bgm��
	//-------------------------------------------------------------
	void RewindBgm(const int index);

	//-------------------------------------------------------------
	// �ǉ�����Bgm�̍폜
	// ���@���Fconst int	�ǉ�����Bgm��
	//-------------------------------------------------------------
	void EraseBgm(const int index);

	//-------------------------------------------------------------
	// �ǉ�����Se�̍폜
	// ���@���Fconst int	�ǉ�����Se��
	//-------------------------------------------------------------
	void EraseSe(const int index);

	//-------------------------------------------------------------
	// �S�Ẳ����f�[�^���폜
	//-------------------------------------------------------------
	void Release();

	//-------------------------------------------------------------
	// �{�����[���̐ݒ�
	// ���@���Ffloat	�{�����[��
	//-------------------------------------------------------------
	void SetVolume(float volume);

	//-------------------------------------------------------------
	// �I�[�f�B�I�̎擾
	// �߂�l�FIXAudio2	�I�[�f�B�I�N���X�̃|�C���^
	//-------------------------------------------------------------
	IXAudio2* GetXAudio();

private:

	IXAudio2* m_pXAudio2;
	IXAudio2MasteringVoice* m_pXAudioMaster;

	map<int, CBgm*> m_mapBgm;
	map<int, CSe*> m_mapSe;

	//�R�s�[�h�~
	CAudio& operator=(const CAudio&);
	CAudio(const CAudio&);
};
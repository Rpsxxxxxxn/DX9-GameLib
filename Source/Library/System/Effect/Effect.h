//*************************************************************
//
// Effect.h
//
// �G�t�F�N�g�N���X
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

#pragma once

//=============================================================
// �C���N���[�h
//=============================================================
#include "../../Common.h"

#include "../../../../Include/Effekseer/Effekseer.h"
#include "../../../../Include/Effekseer/EffekseerRendererDX9.h"
#include "../../../../Include/Effekseer/EffekseerSoundXAudio2.h"

#if _DEBUG
#pragma comment(lib, "Debug/Effekseer.lib" )
#pragma comment(lib, "Debug/EffekseerRendererDX9.lib" )
#pragma comment(lib, "Debug/EffekseerSoundXAudio2.lib" )
#else
#pragma comment(lib, "Release/Effekseer.lib" )
#pragma comment(lib, "Release/EffekseerRendererDX9.lib" )
#pragma comment(lib, "Release/EffekseerSoundXAudio2.lib" )
#endif

//=============================================================
// �O���錾
//=============================================================
class CCamera;

//=============================================================
// �N���X��`
//=============================================================
class CEffectSystem
{
public:

	//-------------------------------------------------------------
	// �V���O���g������
	//-------------------------------------------------------------
	static CEffectSystem* Create();

	//-------------------------------------------------------------
	// �R���X�g���N�^
	//-------------------------------------------------------------
	CEffectSystem();

	//-------------------------------------------------------------
	// �f�X�g���N�^
	//-------------------------------------------------------------
	~CEffectSystem();

	//-------------------------------------------------------------
	// ������
	// ���@���FLPDIRECT3DDEVICE9*�@	�f�o�C�X�̃|�C���^
	// �@�@�@�FUINT					�\���ő�X�v���C�g��
	//-------------------------------------------------------------
	void Initialize(LPDIRECT3DDEVICE9 pDevice, UINT drawCntMax);

	//-------------------------------------------------------------
	// �f�[�^�̐ݒ�
	// ���@���Fconst float ����p�i���W�A���p�j
	// �@�@�@�@const float �A�X�y�N�g��iwidth / height�j
	// �@�@�@�@const float �O���N���b�v��
	// �@�@�@�@const float ����N���b�v��
	//-------------------------------------------------------------
	void InitProjection(
		const float fov,
		const float aspect, 
		const float nearClip, 
		const float farClip);

	//-------------------------------------------------------------
	// �G�t�F�N�g�̒ǉ�
	// ���@���FEFK_CHAR*	�t�@�C����
	//-------------------------------------------------------------
	void AddEffect(const EFK_CHAR* strFileName);

	//-------------------------------------------------------------
	// �G�t�F�N�g�̒ǉ�
	// ���@���Fconst int	�z��̔Ԗ�
	// �@�@�@�@EFK_CHAR*	�t�@�C����
	//-------------------------------------------------------------
	void AddEffect(const int index, const EFK_CHAR* strFileName);

	//-------------------------------------------------------------
	// �G�t�F�N�g�̍Đ�
	// ���@���Fconst int	�z��̔Ԗ�
	// �@�@�@�@D3DXVECTOR3	3�����x�N�g��
	//-------------------------------------------------------------
	void Play(const int index, D3DXVECTOR3& VecPos);

	//-------------------------------------------------------------
	// �G�t�F�N�g�̍Đ�
	// ���@���Fconst int	�z��̔Ԗ�
	// �@�@�@�@D3DXVECTOR3*	3�����x�N�g���|�C���^
	//-------------------------------------------------------------
	void Play(const int index, D3DXVECTOR3* pVecPos);

	//-------------------------------------------------------------
	// �J�������W�̍X�V
	// ���@���FCamera*	�J�����N���X�̃|�C���^
	//-------------------------------------------------------------
	void Update(CCamera* pCamera);

	//-------------------------------------------------------------
	// �G�t�F�N�g�`��
	//-------------------------------------------------------------
	void Draw();

private:

	//-------------------------------------------------------------
	// �x�N�g���N���X�ϊ�(EffekVec3 -> D3DXVec3)
	// ���@���FD3DXVECTOR3	�R�����x�N�g���N���X
	//-------------------------------------------------------------
	Effekseer::Vector3D ConvertD3DXVec3(D3DXVECTOR3 Vec3);

	//-------------------------------------------------------------
	// �x�N�g���N���X�ϊ�(D3DXVec3 -> EffekVec3)
	// ���@���FEffekseer::Vector3D	3�����x�N�g���N���X
	//-------------------------------------------------------------
	D3DXVECTOR3 ConvertEffekVec3(Effekseer::Vector3D Vec3);

	//�n���h��
	Effekseer::Handle m_hHandle;

	//�G�t�F�N�g�z��
	vector<Effekseer::Effect*> m_pEffect;

	//�`��Ǘ�
	EffekseerRenderer::Renderer* m_pRenderer;

	//�G�t�F�N�g�Ǘ�
	Effekseer::Manager* m_pManager;

	//�R�s�[�h�~
	CEffectSystem& operator=(const CEffectSystem&);
	CEffectSystem(const CEffectSystem&);
};
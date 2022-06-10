//********************************************************************************
//
// CDemoRender.hpp
//
// �f���p���f�������_�����O
//
// Date: 2019/07/30 �X�V
//
//********************************************************************************

#pragma once

//================================================================================
// �C���N���[�h
//================================================================================
#include "../../GameManager/GameManager.h"

//================================================================================
// �N���X��`
//================================================================================
class CDemoRender
{
public:

	//--------------------------------------------------------------------------------
	// �R���X�g���N�^
	//--------------------------------------------------------------------------------
	CDemoRender();

	//--------------------------------------------------------------------------------
	// �f�X�g���N�^
	//--------------------------------------------------------------------------------
	~CDemoRender();

	//--------------------------------------------------------------------------------
	// �t�@�C���ǂݍ���
	// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
	//--------------------------------------------------------------------------------
	void LoadFile(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// ������
	// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
	//--------------------------------------------------------------------------------
	void Initialize(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// �X�V
	// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
	//--------------------------------------------------------------------------------
	void Update(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// �`��
	// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
	//--------------------------------------------------------------------------------
	void ZDraw(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// �`��
	// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
	//--------------------------------------------------------------------------------
	void Draw(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// �J�����N���X�̃|�C���^�擾
	// �߂�l�Fconst CCamera*	�J�����N���X�̃|�C���^
	//--------------------------------------------------------------------------------
	CCamera* GetCamera();

	//--------------------------------------------------------------------------------
	// �^�C�g���̃X�e�[�W��ύX���܂�
	//--------------------------------------------------------------------------------
	void ChangeStage(CSystem* pSystem);
private:

	void UpdateMeshAlpha();

	//--------------------------------------------------------------------------------
	// ���E�s��̐ݒ�
	//--------------------------------------------------------------------------------
	void SetWorldTransform();

	//--------------------------------------------------------------------------------
	// �J�����̋����ݒ�
	//--------------------------------------------------------------------------------
	void SetCameraDist();

	CCamera* m_pCamera;

	CXFileModel* m_pPlayer;
	CXFileModel* m_pField;
	CXFileModel* m_pSky;
	CIntervalTimer m_timer;

	Vector3 m_vecLockAt;
	Vector3 m_vecAngle;

	float m_cameraXDist;
	float m_cameraZDist;

	float m_baseAngle;
	float m_demoAngle;
	float m_sinAngle;

	float m_meshAlpha;
	bool m_isChangeMesh;
	bool m_isChangeAlpha;
};
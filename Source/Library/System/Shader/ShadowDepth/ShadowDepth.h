//*************************************************************
//
// �f�v�X�����_�����O
//
// ShadowDepth.h
//
// Cascaded Shadow Maps
//
//*************************************************************

#pragma once

//=============================================================
// �C���N���[�h
//=============================================================
#include "../ShaderBase.h"
#include "../../../Graphics/Camera/Camera.h"

//================================================================
// �N���X��`
//================================================================
class CShadowDepth
	: public CShaderBase
{
public:
	//������
	static const int CASCADE_SPLIT_COUNT = 3;

	//-------------------------------------------------------------
	// �R���X�g���N�^
	// ���@���Fconst UINT	����
	// �@�@�@�@const UINT	�c��
	//-------------------------------------------------------------
	CShadowDepth(const UINT width, const UINT height);

	//-------------------------------------------------------------
	// �f�X�g���N�^
	//-------------------------------------------------------------
	~CShadowDepth();

	//-------------------------------------------------------------
	// ������
	// ���@���FLPDIRECT3DDEVICE9 �f�o�C�X�̃|�C���^
	//-------------------------------------------------------------
	void Initialize(LPDIRECT3DDEVICE9 pDevice);

	//-------------------------------------------------------------
	// �X�V
	//-------------------------------------------------------------
	void Update();

	//-------------------------------------------------------------
	// �f�v�X�����_�����O�J�n
	// ���@���Fint	�ԍ�
	//-------------------------------------------------------------
	void StartDepthRender(const int index);

	//-------------------------------------------------------------
	// �f�v�X�����_�����O�I��
	//-------------------------------------------------------------
	void EndDepthRender();

	//-------------------------------------------------------------
	// �V���h�E�e�N�X�`���̎擾
	// ���@���Fint	�ԍ�
	//-------------------------------------------------------------
	LPDIRECT3DTEXTURE9 GetTexture(const int index) const;

	//-------------------------------------------------------------
	// ������̌v�Z
	// ���@���FCamera �J�����N���X
	// �@�@�@�@int	  �����ԍ�
	// �@�@�@�@bool	  �X�V�����邩
	//-------------------------------------------------------------
	void CalculateCascadePlanes();

	//-------------------------------------------------------------
	// �����V���h�E�}�b�v�̐[�x�v�Z
	//-------------------------------------------------------------
	void CalculateShadowMapSplitDepths();

	//-------------------------------------------------------------
	// �V���h�E�}�b�v�s��̍Čv�Z
	// ���@���FCamera		�J�����N���X
	//-------------------------------------------------------------
	void RecalculateShadowMapMatrices(CCamera* camera);

	//-------------------------------------------------------------
	// ���C�g�r���[�s��
	// ���@���FD3DXMATRIX*	�r���[�s��̃|�C���^
	//-------------------------------------------------------------
	void SetLightViewMatrix(D3DXMATRIX* pMatView);

	//-------------------------------------------------------------
	// ���C�g�v���W�F�N�V�����s��
	// ���@���FD3DXMATRIX*	�v���W�F�N�V�����s��̃|�C���^
	//-------------------------------------------------------------
	void SetLightProjMatrix(D3DXMATRIX* pMatProj);

	//-------------------------------------------------------------
	// ���C�g�v���W�F�N�V�����s��
	// ���@���FD3DXVECTOR3*		���C�g�ʒu�x�N�g���̃|�C���^
	//-------------------------------------------------------------
	void SetLightPosition(D3DXVECTOR3* pVecLightPos);

	//-------------------------------------------------------------
	// �r���[�v���W�F�N�V�����̍s����擾
	// ���@���Fconst int		�����ԍ�
	//-------------------------------------------------------------
	D3DXMATRIX GetViewProjMatrix(const int index);

private:
	//-------------------------------------------------------------
	// �����V���h�E�̍s��v�Z
	// ���@���FCamera �J�����N���X
	// �@�@�@�@int	  �����ԍ�
	// �@�@�@�@bool	  �X�V�����邩
	//-------------------------------------------------------------
	void CalculateShadowMatrices(CCamera* camera, int i, bool forceUpdate);

	LPDIRECT3DTEXTURE9 m_pShadow[CASCADE_SPLIT_COUNT];
	LPDIRECT3DSURFACE9 m_pShadowSurf[CASCADE_SPLIT_COUNT];
	LPDIRECT3DSURFACE9 m_pShadowZ[CASCADE_SPLIT_COUNT];

	LPDIRECT3DSURFACE9 m_pOldBackBuffer;
	LPDIRECT3DSURFACE9 m_pOldZBuffer;
	D3DVIEWPORT9 m_oldViewport;

	D3DXMATRIX m_matWorld;
	D3DXMATRIX m_matLView;
	D3DXMATRIX m_matLProj;
	D3DXMATRIX m_matViewProj[CASCADE_SPLIT_COUNT];
	D3DXVECTOR3 m_previousMinValues[CASCADE_SPLIT_COUNT];
	D3DXVECTOR3 m_vecLightPosition;

	FLOAT m_shadowSplitDepths[CASCADE_SPLIT_COUNT + 1];
	FLOAT m_blendDistance;
	FLOAT m_radii[CASCADE_SPLIT_COUNT];
	D3DXPLANE** m_ppCascadePlanes;

	UINT m_width;
	UINT m_height;
};
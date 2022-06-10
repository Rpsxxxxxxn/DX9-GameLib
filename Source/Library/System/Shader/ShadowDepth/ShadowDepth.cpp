//*************************************************************
//
// �f�v�X�����_�����O
//
// ShadowDepth.cpp
//
// Cascaded Shadow Maps
//
//*************************************************************

//=============================================================
// �}�N����`
//=============================================================
#define SHADOW_MAP_FORMAT D3DFMT_R32F
#define SHADOW_MAP_SIZE   4000
#define SHADOW_MAP_NEAR_CLIP 0.1f
#define SHADOW_MAP_FAR_CLIP 100.0f

#include "ShadowDepth.h"

//=============================================================
// �N���X��`
//=============================================================

//-------------------------------------------------------------
// �R���X�g���N�^
// ���@���Fconst UINT	����
// �@�@�@�@const UINT	�c��
//-------------------------------------------------------------
CShadowDepth::CShadowDepth(const UINT width, const UINT height)
	: m_width(width)
	, m_height(height)
{
	m_blendDistance = 0.0f;
	m_ppCascadePlanes = new D3DXPLANE*[CASCADE_SPLIT_COUNT];

	for (int i = 0; i < CASCADE_SPLIT_COUNT; i++)
	{
		this->m_pShadow[i] = nullptr;
		this->m_pShadow[i] = nullptr;
		this->m_pShadowZ[i] = nullptr;
		this->m_ppCascadePlanes[i] = new D3DXPLANE[6];

		m_previousMinValues[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
CShadowDepth::~CShadowDepth()
{
	for (int i = 0; i < CASCADE_SPLIT_COUNT; i++)
	{
		delete[] m_ppCascadePlanes[i];
		m_pShadow[i]->Release();
		m_pShadowZ[i]->Release();
		m_pShadowSurf[i]->Release();
	}
	delete[] m_ppCascadePlanes;
}

//-------------------------------------------------------------
// ������
// ���@���FLPDIRECT3DDEVICE9 �f�o�C�X�̃|�C���^
//-------------------------------------------------------------
void CShadowDepth::Initialize(LPDIRECT3DDEVICE9 pDevice)
{
	CShaderBase::Initialize(pDevice, "Resource/Shaders/shadow_depth.fx");

	for (int i = 0; i < CASCADE_SPLIT_COUNT; i++) 
	{
		m_pDevice->CreateTexture(
			SHADOW_MAP_SIZE,
			SHADOW_MAP_SIZE,
			1,
			D3DUSAGE_RENDERTARGET,
			SHADOW_MAP_FORMAT, 
			D3DPOOL_DEFAULT, 
			&m_pShadow[i],
			NULL);
		
		m_pShadow[i]->GetSurfaceLevel(0, &m_pShadowSurf[i]);

		m_pDevice->CreateDepthStencilSurface(
			SHADOW_MAP_SIZE,
			SHADOW_MAP_SIZE, 
			D3DFMT_D16, 
			D3DMULTISAMPLE_NONE,
			0,
			TRUE,
			&m_pShadowZ[i],
			NULL);

		D3DXMatrixIdentity(&m_matViewProj[i]);
	}

	m_radii[0] = SHADOW_MAP_FAR_CLIP * 0.1f;
	m_radii[1] = SHADOW_MAP_FAR_CLIP * 0.5f;
	m_radii[2] = SHADOW_MAP_FAR_CLIP;

	CalculateShadowMapSplitDepths();
}

//-------------------------------------------------------------
// �X�V
// ���@���FEngine*	�G���W���N���X�̃|�C���^
//-------------------------------------------------------------
void CShadowDepth::Update()
{
}

//-------------------------------------------------------------
// �f�v�X�����_�����O�J�n
//-------------------------------------------------------------
void CShadowDepth::StartDepthRender(const int index)
{
	m_pDevice->GetRenderTarget(0, &m_pOldBackBuffer);
	m_pDevice->GetDepthStencilSurface(&m_pOldZBuffer);
	m_pDevice->GetViewport(&m_oldViewport);

	m_pDevice->SetRenderTarget(0, m_pShadowSurf[index]);
	m_pDevice->SetDepthStencilSurface(m_pShadowZ[index]);

	D3DVIEWPORT9 view;

	view.Width = SHADOW_MAP_SIZE;
	view.Height = SHADOW_MAP_SIZE;
	view.MinZ = 0.0f;
	view.MaxZ = 1.0f;
	view.X = 0;
	view.Y = 0;

	m_pDevice->SetViewport(&view);

	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);

	int i = index;
	this->SetValue("g_iSliceNum", i);
}

//-------------------------------------------------------------
// �f�v�X�����_�����O�I��
//-------------------------------------------------------------
void CShadowDepth::EndDepthRender()
{
	m_pDevice->SetRenderTarget(0, m_pOldBackBuffer);
	m_pDevice->SetDepthStencilSurface(m_pOldZBuffer);
	m_pDevice->SetViewport(&m_oldViewport);
	m_pOldBackBuffer->Release();
	m_pOldZBuffer->Release();

	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFF000000, 1.0f, 0);
}

//-------------------------------------------------------------
// �V���h�E�e�N�X�`���̎擾
//-------------------------------------------------------------
LPDIRECT3DTEXTURE9 CShadowDepth::GetTexture(const int index) const
{
	return m_pShadow[index];
}

//-------------------------------------------------------------
// ������̌v�Z
//-------------------------------------------------------------
void CShadowDepth::CalculateCascadePlanes()
{
	for (int i = 0; i < CASCADE_SPLIT_COUNT; i++)
	{
		//��O�̃v���[�g�v�Z
		m_ppCascadePlanes[i][0].a = m_matViewProj[i]._14 + m_matViewProj[i]._13;
		m_ppCascadePlanes[i][0].b = m_matViewProj[i]._24 + m_matViewProj[i]._23;
		m_ppCascadePlanes[i][0].c = m_matViewProj[i]._34 + m_matViewProj[i]._33;
		m_ppCascadePlanes[i][0].d = m_matViewProj[i]._44 + m_matViewProj[i]._43;
		D3DXPlaneNormalize(&m_ppCascadePlanes[i][0], &m_ppCascadePlanes[i][0]);

		// ���s���̃v���[�g�̌v�Z
		m_ppCascadePlanes[i][1].a = m_matViewProj[i]._14 - m_matViewProj[i]._13;
		m_ppCascadePlanes[i][1].b = m_matViewProj[i]._24 - m_matViewProj[i]._23;
		m_ppCascadePlanes[i][1].c = m_matViewProj[i]._34 - m_matViewProj[i]._33;
		m_ppCascadePlanes[i][1].d = m_matViewProj[i]._44 - m_matViewProj[i]._43;
		D3DXPlaneNormalize(&m_ppCascadePlanes[i][1], &m_ppCascadePlanes[i][1]);

		// ���̃v���[�g�v�Z
		m_ppCascadePlanes[i][2].a = m_matViewProj[i]._14 + m_matViewProj[i]._11;
		m_ppCascadePlanes[i][2].b = m_matViewProj[i]._24 + m_matViewProj[i]._21;
		m_ppCascadePlanes[i][2].c = m_matViewProj[i]._34 + m_matViewProj[i]._31;
		m_ppCascadePlanes[i][2].d = m_matViewProj[i]._44 + m_matViewProj[i]._41;
		D3DXPlaneNormalize(&m_ppCascadePlanes[i][2], &m_ppCascadePlanes[i][2]);

		// �E�̃v���[�g�v�Z
		m_ppCascadePlanes[i][3].a = m_matViewProj[i]._14 - m_matViewProj[i]._11;
		m_ppCascadePlanes[i][3].b = m_matViewProj[i]._24 - m_matViewProj[i]._21;
		m_ppCascadePlanes[i][3].c = m_matViewProj[i]._34 - m_matViewProj[i]._31;
		m_ppCascadePlanes[i][3].d = m_matViewProj[i]._44 - m_matViewProj[i]._41;
		D3DXPlaneNormalize(&m_ppCascadePlanes[i][3], &m_ppCascadePlanes[i][3]);

		// ��̃v���[�g�v�Z
		m_ppCascadePlanes[i][4].a = m_matViewProj[i]._14 - m_matViewProj[i]._12;
		m_ppCascadePlanes[i][4].b = m_matViewProj[i]._24 - m_matViewProj[i]._22;
		m_ppCascadePlanes[i][4].c = m_matViewProj[i]._34 - m_matViewProj[i]._32;
		m_ppCascadePlanes[i][4].d = m_matViewProj[i]._44 - m_matViewProj[i]._42;
		D3DXPlaneNormalize(&m_ppCascadePlanes[i][4], &m_ppCascadePlanes[i][4]);

		// ���̃v���[�g�v�Z
		m_ppCascadePlanes[i][5].a = m_matViewProj[i]._14 + m_matViewProj[i]._12;
		m_ppCascadePlanes[i][5].b = m_matViewProj[i]._24 + m_matViewProj[i]._22;
		m_ppCascadePlanes[i][5].c = m_matViewProj[i]._34 + m_matViewProj[i]._32;
		m_ppCascadePlanes[i][5].d = m_matViewProj[i]._44 + m_matViewProj[i]._42;
		D3DXPlaneNormalize(&m_ppCascadePlanes[i][5], &m_ppCascadePlanes[i][5]);
	}
}

//-------------------------------------------------------------
// �����V���h�E�}�b�v�̐[�x�v�Z
//-------------------------------------------------------------
void CShadowDepth::CalculateShadowMapSplitDepths()
{
	m_shadowSplitDepths[0] = SHADOW_MAP_NEAR_CLIP;
	m_shadowSplitDepths[1] = SHADOW_MAP_FAR_CLIP * 0.1f;
	m_shadowSplitDepths[2] = SHADOW_MAP_FAR_CLIP * 0.5f;
	m_shadowSplitDepths[3] = SHADOW_MAP_FAR_CLIP;
}

//-------------------------------------------------------------
// �V���h�E�}�b�v�s��̍Čv�Z
// ���@���FCamera			�J�����N���X
// �@�@�@�@D3DXVECTOR3*		���C�g�ʒu�x�N�g���̃|�C���^
//-------------------------------------------------------------
void CShadowDepth::RecalculateShadowMapMatrices(CCamera * camera)
{
	for (int i = 0; i < CASCADE_SPLIT_COUNT; i++)
	{
		CalculateShadowMatrices(camera, i, false);
	}
}

//-------------------------------------------------------------
// ���C�g�r���[�s��
// ���@���FD3DXMATRIX*	�r���[�s��̃|�C���^
//-------------------------------------------------------------
void CShadowDepth::SetLightViewMatrix(D3DXMATRIX * pMatView)
{
	m_matLView = *pMatView;
}

//-------------------------------------------------------------
// ���C�g�v���W�F�N�V�����s��
// ���@���FD3DXMATRIX*	�v���W�F�N�V�����s��̃|�C���^
//-------------------------------------------------------------
void CShadowDepth::SetLightProjMatrix(D3DXMATRIX * pMatProj)
{
	m_matLProj = *pMatProj;
}

//-------------------------------------------------------------
// ���C�g�v���W�F�N�V�����s��
// ���@���FD3DXVECTOR3*		���C�g�ʒu�x�N�g���̃|�C���^
//-------------------------------------------------------------
void CShadowDepth::SetLightPosition(D3DXVECTOR3 * pVecLightPos)
{
	m_vecLightPosition = *pVecLightPos;
}

//-------------------------------------------------------------
// �r���[�v���W�F�N�V�����̍s����擾
// ���@���Fconst int		�����ԍ�
//-------------------------------------------------------------
D3DXMATRIX CShadowDepth::GetViewProjMatrix(const int index)
{
	return m_matViewProj[index];
}

//-------------------------------------------------------------
// �����V���h�E�̍s��v�Z
// ���@���FCamera �J�����N���X
// �@�@�@�@int	  �����ԍ�
// �@�@�@�@bool	  �X�V�����邩
//-------------------------------------------------------------
void CShadowDepth::CalculateShadowMatrices(CCamera * camera, int i, bool forceUpdate)
{
	D3DXVECTOR4 cameraEye(camera->m_vecEye, 1.0f);
	D3DXVECTOR4 cameraFront(camera->m_vecForward, 1.0f);
	D3DXVECTOR4 cameraRight(camera->m_vecRight, 1.0f);
	D3DXVECTOR4 cameraUp(camera->m_vecUp, 1.0f);
	float aspect = (float)m_width / (float)m_height;
	float tmp = camera->GetFov();

	// ��O�̃v���[�g
	float halfNearHeight = tmp * m_shadowSplitDepths[i];
	float halfNearWidth = halfNearHeight * aspect;

	// ��O��4�̓_�v�Z
	D3DXVECTOR4	nearCenter = cameraEye + cameraFront * this->m_shadowSplitDepths[i];
	D3DXVECTOR4	nearTopLeft = nearCenter + (cameraUp * halfNearHeight) - (cameraRight * halfNearWidth);
	D3DXVECTOR4	nearTopRight = nearCenter + (cameraUp * halfNearHeight) + (cameraRight * halfNearWidth);
	D3DXVECTOR4	nearBottomLeft = nearCenter - (cameraUp * halfNearHeight) - (cameraRight * halfNearWidth);
	D3DXVECTOR4	nearBottomRight = nearCenter - (cameraUp * halfNearHeight) + (cameraRight * halfNearWidth);

	// ���s���̃v���[�g�v�Z
	float halfFarHeight = tmp * this->m_shadowSplitDepths[i + 1];
	float halfFarWidth = halfFarHeight * aspect;

	// ���s����4�̓_�v�Z
	D3DXVECTOR4 farCenter = cameraEye + cameraFront * this->m_shadowSplitDepths[i + 1];
	D3DXVECTOR4 farTopLeft = farCenter + (cameraUp * halfFarHeight) - (cameraRight * halfFarWidth);
	D3DXVECTOR4	farTopRight = farCenter + (cameraUp * halfFarHeight) + (cameraRight * halfFarWidth);
	D3DXVECTOR4	farBottomLeft = farCenter - (cameraUp * halfFarHeight) - (cameraRight * halfFarWidth);
	D3DXVECTOR4	farBottomRight = farCenter - (cameraUp * halfFarHeight) + (cameraRight * halfFarWidth);

	// �������8�_������
	D3DXVECTOR4 frustumPoints[8] =
	{
		nearTopLeft, nearTopRight, nearBottomLeft, nearBottomRight,
		farTopLeft, farTopRight, farBottomLeft, farBottomRight
	};

	// �|�C���g�����[���h��Ԃ��烉�C�g�̋ψ��Ԃɕϊ����܂��B
	for (int index = 0; index < 8; index++)
	{
		frustumPoints[index].w = 1.0f;
		D3DXVec4Transform(&frustumPoints[index], &frustumPoints[index], &m_matLView);
	}

	// �����e��min�inear plane�j�����max�ifar plane�j�l���v�Z���܂��B
	float infinity = std::numeric_limits<float>::infinity();
	D3DXVECTOR3 minValue(infinity, infinity, infinity);
	D3DXVECTOR3 maxValue(-infinity, -infinity, -infinity);
	for (int index = 0; index < 8; index++)
	{
		D3DXVECTOR3 vec3(frustumPoints[index].x, frustumPoints[index].y, frustumPoints[index].z);

		// �ŏ��_�ƍő�_�������܂��B
		D3DXVec3Minimize(&minValue, &minValue, &vec3);
		D3DXVec3Maximize(&maxValue, &maxValue, &vec3);
	}

	// �O�̍ŏ��|�C���g�Ԃ̋������v�Z���܂��B
	D3DXVECTOR3 tmpVec = minValue - m_previousMinValues[i];
	float len = D3DXVec3Length(&tmpVec);
	if (len > m_radii[i])
	{
		m_previousMinValues[i] = minValue;
	}
	else
	{
		// �|�C���g���܂����a���ɂ���A�ʂ̃J�X�P�[�h�ɂ���ċ����I�ɍX�V����Ȃ��ꍇ�A
		// �J�X�P�[�h���Čv�Z����K�v�͂���܂���
		if (!forceUpdate)
		{
			//return;
		}
	}

	D3DXVECTOR3 tmpNearPlanePoint = minValue;
	D3DXVECTOR3 tmpFarPlanePoint = maxValue;
	// ���ݓI�ȃI�N���[�_�[���܂߂邽�߂ɁA�j�A�v���[�������C�g�ɋ߂Â��܂�
	if (m_vecLightPosition.z < 0.0f)
	{
		m_vecLightPosition *= -1.0f;
	}
	// ���ׂẴJ�X�P�[�h�ɓ����j�A�v���[����ݒ�
	tmpNearPlanePoint -= m_vecLightPosition * (float)(50 * (i + 1));
	tmpNearPlanePoint += m_vecLightPosition * m_radii[i]; //���z�ɋ߂����ʂ̋߂��Ɉړ����܂�
	tmpFarPlanePoint -= m_vecLightPosition * m_radii[i]; //���z���牓�����ʂ��ړ����܂�
	float nearPlane = tmpNearPlanePoint.z;
	float farPlane = maxValue.z;

	// ���̃J�X�P�[�h/����X���C�X�̐����e���쐬���܂��B
	// �u�����h������ǉ����āA�V���h�E�}�b�v�ԂŃu�����h���邽�߂ɓ��e���݂��ɃI�[�o�[���b�v����悤�ɂ��܂�
	D3DXMatrixIdentity(&m_matLProj);

	// �u�����h�����ɂ�蓊�e���d�Ȃ�܂�
	// ���̃I�[�o�[���b�v�́A�J�X�P�[�h�������G�b�W�����炩/�ڂ������߂Ɏg�p����܂�
	D3DXMatrixOrthoOffCenterLH(&m_matLProj,
		minValue.x - m_blendDistance - m_radii[i], // ��
		maxValue.x + m_blendDistance + m_radii[i], // �E
		minValue.y - m_blendDistance - m_radii[i], // ��
		maxValue.y + m_blendDistance + m_radii[i], // ��
		nearPlane, farPlane);

	m_matViewProj[i] = m_matLView * m_matLProj;

	// ���̂��ׂẴJ�X�P�[�h�i���j�������I�ɍX�V���܂��i�����J�X�P�[�h�͉������p�ɂɍX�V���܂��j
	if (i > 0)
	{
		CalculateShadowMatrices(camera, i - 1, true);
	}
}

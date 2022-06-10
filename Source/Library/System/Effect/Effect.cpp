//*************************************************************
//
// Effect.cpp
//
// �G�t�F�N�g�N���X
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "Effect.h"
#include "../../Graphics/Camera/Camera.h"

//=============================================================
// �V���O���g������
//=============================================================
CEffectSystem * CEffectSystem::Create()
{
	static CEffectSystem instance;
	return &instance;
}

//-------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------
CEffectSystem::CEffectSystem()
{
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
CEffectSystem::~CEffectSystem()
{
	// �G�t�F�N�g�̒�~
	m_pManager->StopAllEffects();

	// �G�t�F�N�g�Ǘ��p�C���X�^���X��j��
	m_pManager->Destroy();

	// �`��p�C���X�^���X��j
	m_pRenderer->Destroy();

	// �G�t�F�N�g�̊J��
	for (UINT i = 0; i < m_pEffect.size(); i++)
	{
		if (m_pEffect[i] != nullptr)
		{
			m_pEffect[i]->Release();
			m_pEffect[i] = nullptr;
		}
	}

	m_pManager = nullptr;
	m_pRenderer = nullptr;
}

//-------------------------------------------------------------
// ������
// ���@���FLPDIRECT3DDEVICE9*�@	�f�o�C�X�̃|�C���^
// �@�@�@�FUINT					�\���ő�X�v���C�g��
//-------------------------------------------------------------
void CEffectSystem::Initialize(LPDIRECT3DDEVICE9 pDevice, UINT drawCntMax)
{
	// �`��Ǘ��C���X�^���X�̐���
	m_pRenderer = EffekseerRendererDX9::Renderer::Create(pDevice, drawCntMax);

	// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
	m_pManager = Effekseer::Manager::Create(drawCntMax);

	// �`����@�̎w��
	m_pManager->SetSpriteRenderer(m_pRenderer->CreateSpriteRenderer());
	m_pManager->SetRibbonRenderer(m_pRenderer->CreateRibbonRenderer());
	m_pManager->SetRingRenderer(m_pRenderer->CreateRingRenderer());
	m_pManager->SetTrackRenderer(m_pRenderer->CreateTrackRenderer());
	m_pManager->SetModelRenderer(m_pRenderer->CreateModelRenderer());

	// �e�N�X�`���摜�ǂݍ��ݕ��@�̎w��
	m_pManager->SetTextureLoader(m_pRenderer->CreateTextureLoader());
	m_pManager->SetModelLoader(m_pRenderer->CreateModelLoader());

	// ���W�n�̎w��(RH�ŉE��n�ALH�ō���n)
	m_pManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
}

//-------------------------------------------------------------
// �f�[�^�̐ݒ�
// ���@���Fconst float ����p�i���W�A���p�j
// �@�@�@�@const float �A�X�y�N�g��iwidth / height�j
// �@�@�@�@const float �O���N���b�v��
// �@�@�@�@const float ����N���b�v��
//-------------------------------------------------------------
void CEffectSystem::InitProjection(
	const float fov, 
	const float aspect,
	const float nearClip, 
	const float farClip
)
{
	// ���e�s��
	Effekseer::Matrix44 matProj;

	// ���e�s��̐ݒ�
	matProj.PerspectiveFovLH(fov, aspect, nearClip, farClip);

	// ���e�s��̐ݒ�
	m_pRenderer->SetProjectionMatrix(matProj);

	// �J������ݒ肷��
	m_pRenderer->SetCameraMatrix(
		Effekseer::Matrix44().LookAtLH(
			Effekseer::Vector3D(0.0f, 0.0f, 0.0f), // ���_
			Effekseer::Vector3D(0.0f, 0.0f, 0.0f), // ���ړ_
			Effekseer::Vector3D(0.0f, 1.0f, 0.0f)  // �����
		)
	);
}

//-------------------------------------------------------------
// �G�t�F�N�g�̒ǉ�
// ���@���FEFK_CHAR*	�t�@�C����
//-------------------------------------------------------------
void CEffectSystem::AddEffect(const EFK_CHAR * strFileName)
{
	m_pEffect.push_back(Effekseer::Effect::Create(m_pManager, strFileName));
}

//-------------------------------------------------------------
// �G�t�F�N�g�̒ǉ�
// ���@���Fconst int	�z��̔Ԗ�
// �@�@�@�@EFK_CHAR*	�t�@�C����
//-------------------------------------------------------------
void CEffectSystem::AddEffect(const int index, const EFK_CHAR* strFileName)
{
	m_pEffect[index] = Effekseer::Effect::Create(m_pManager, strFileName);
}

//-------------------------------------------------------------
// �G�t�F�N�g�̍Đ�
// ���@���FD3DXVECTOR3 3�����x�N�g��
//-------------------------------------------------------------
void CEffectSystem::Play(const int index, D3DXVECTOR3& VecPos)
{
	m_hHandle = m_pManager->Play(
		m_pEffect[index],
		VecPos.x,
		VecPos.y,
		VecPos.z);
}

//-------------------------------------------------------------
// �G�t�F�N�g�̍Đ�
// ���@���FD3DXVECTOR3 3�����x�N�g���̃|�C���^
//-------------------------------------------------------------
void CEffectSystem::Play(const int index, D3DXVECTOR3* pVecPos)
{
	m_hHandle = m_pManager->Play(
		m_pEffect[index],
		pVecPos->x,
		pVecPos->y,
		pVecPos->z);
}

//-------------------------------------------------------------
// �J�������W�̍X�V
// ���@���FCamera*	�J�����N���X�̃|�C���^
//-------------------------------------------------------------
void CEffectSystem::Update(CCamera * pCamera)
{
	// ���ӑ��
	Effekseer::Vector3D vecEye = ConvertD3DXVec3(pCamera->m_vecEye);
	Effekseer::Vector3D vecLookAt = ConvertD3DXVec3(pCamera->m_vecLookAt);
	Effekseer::Vector3D vecUp = ConvertD3DXVec3(pCamera->m_vecUp);

	// �J�����s��̍X�V
	m_pRenderer->SetCameraMatrix(Effekseer::Matrix44().LookAtLH(vecEye, vecLookAt, vecUp));

	// �Đ����̃G�t�F�N�g�̈ړ���(::Effekseer::Manager�o�R�ŗl�X�ȃp�����[�^�[���ݒ�ł��܂��B)
	m_pManager->AddLocation(m_hHandle, ConvertD3DXVec3(D3DXVECTOR3(0.0f, 0.0f, 0.0f)));

	// �S�ẴG�t�F�N�g�̍X�V
	m_pManager->Update();
}

//-------------------------------------------------------------
// �G�t�F�N�g�`��
//-------------------------------------------------------------
void CEffectSystem::Draw()
{
	// �����_�����O�J�n
	m_pRenderer->BeginRendering();

	// �����_�����O��
	m_pManager->Draw();

	// �����_�����O�I��
	m_pRenderer->EndRendering();
}

//-------------------------------------------------------------
// �x�N�g���N���X�ϊ�(EffekVec3 -> D3DXVec3)
// ���@���FD3DXVECTOR3	�R�����x�N�g���N���X
//-------------------------------------------------------------
Effekseer::Vector3D CEffectSystem::ConvertD3DXVec3(D3DXVECTOR3 Vec3)
{
	Effekseer::Vector3D w;

	w.X = Vec3.x;
	w.Y = Vec3.y;
	w.Z = Vec3.z;

	return Effekseer::Vector3D(w);
}

//-------------------------------------------------------------
// �x�N�g���N���X�ϊ�(D3DXVec3 -> EffekVec3)
// ���@���FEffekseer::Vector3D	3�����x�N�g���N���X
//-------------------------------------------------------------
D3DXVECTOR3 CEffectSystem::ConvertEffekVec3(Effekseer::Vector3D Vec3)
{
	D3DXVECTOR3 w;

	w.x = Vec3.X;
	w.y = Vec3.Y;
	w.z = Vec3.Z;

	return D3DXVECTOR3(w);
}

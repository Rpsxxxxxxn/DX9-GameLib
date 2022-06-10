//********************************************************************************
//
// PlayerCamera.hpp
//
// �v���C���[�J����
//
// Date: 2019/07/30 �X�V
//
//********************************************************************************

//================================================================================
// �C���N���[�h
//================================================================================
#include "PlayerCamera.hpp"

using namespace FileConst;

//================================================================================
// �萔��`
//================================================================================
const Vector3 CAMERA_ADJUST_HEIGHT(0.0f, 1.8f, 0.0f);
const Vector3 CAMERA_INIT_ROTATE(Mathf::ToRadian(-15.0f), 0.0f, 0.0f);
const float AXIS_HORI_LIMIT = 1.0f;
const float AXIS_MOVE_SPEED = 0.01f;
const float CAMERA_MESH_ADJUST = 0.3f;
const float CAMERA_SPEED_KEY = 2.0f;
const float CAMERA_SPEED_MOUSE = 0.25f;
const float CAMERA_SPEED_JOYSTICK = 2.5f;
const float CAMERA_TRACKING_DIST = 4.0f;
const float CAMERA_DELAY_SPEED = 0.15f;
const float CAMERA_FOV_SPEED = 0.04f;
const float CAMERA_UP_LIMIT = D3DXToRadian(-45);
const float CAMERA_DOWN_LIMIT = D3DXToRadian(15);
const float CAMERA_SHAKE_SPEED = 0.1f;

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
CPlayerCamera::CPlayerCamera()
	: m_pCamera(nullptr)
	, m_pRay(nullptr)
	, m_vecPosDelay(CAMERA_ADJUST_HEIGHT)
	, m_vecRotDelay(CAMERA_INIT_ROTATE)
	, m_shake(0.0f)
	, m_fov(Mathf::ToRadian(60.0f))
	, m_fovDelay(Mathf::ToRadian(60.0f))
{
	m_pCamera = new CCamera();
	m_pRay = new CRay();
	m_rotation = CAMERA_INIT_ROTATE;
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
CPlayerCamera::~CPlayerCamera()
{
	SafeDelete(m_pRay);
	SafeDelete(m_pCamera);
}

//--------------------------------------------------------------------------------
// �t�@�C���ǂݍ���
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CPlayerCamera::LoadFile(CSystem * pSystem)
{
	pSystem->AddEffect((const EFK_CHAR*)L"Resource/Effekseer/Sword.efk");
	pSystem->AddEffect((const EFK_CHAR*)L"Resource/Effekseer/MagicCircleMyRing.efk");
	pSystem->AddEffect((const EFK_CHAR*)L"Resource/Effekseer/hitGuard.efk");
	pSystem->AddEffect((const EFK_CHAR*)L"Resource/Effekseer/DeathEffect.efk");
}

//--------------------------------------------------------------------------------
// ������
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CPlayerCamera::Initialize(CSystem * pSystem)
{
	m_pCamera->Initialize(pSystem->GetDevice(), Mathf::ToRadian(60.0f), 16.0f / 9.0f, 0.1f, 1000.0f);
	pSystem->InitProjection(Mathf::ToRadian(60.0f), 16.0f / 9.0f, 0.1f, 1000.0f);
}

//--------------------------------------------------------------------------------
// �X�V
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
// �@�@�@�@CPlayer*	�v���C���[�N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CPlayerCamera::Update(CSystem * pSystem, CPlayer* pPlayer, const float lockAngle)
{
	pSystem->SetCameraMatrix(m_pCamera);
	UpdateLookAtPlayer(pPlayer);
	UpdateKeyInput(pSystem);
	UpdateController(pPlayer);
	UpdateDashShake(pPlayer);
	UpdateLockOnSystem(pPlayer, lockAngle);
	SetCameraParams();
	UpdatePosition();
}

//--------------------------------------------------------------------------------
// �J�����p�����[�^�̍Čv�Z
//--------------------------------------------------------------------------------
void CPlayerCamera::RecalcCameraParams()
{
	m_pCamera->RecalculateUpDirection();
	m_pCamera->RecalculateFrustumPlane();
	m_pCamera->UpdateViewMatrix();
	m_pCamera->UpdateProjectionMatrix();
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// �ʒu�̍X�V
//--------------------------------------------------------------------------------
void CPlayerCamera::UpdatePosition()
{
	//�J�������b�V���߂荞�ݖh�~
	if (m_pCamera->m_vecEye.y < m_pRay->m_position.y + CAMERA_MESH_ADJUST)
	{
		m_pCamera->m_vecEye.y = m_pRay->m_position.y + CAMERA_MESH_ADJUST;

		//�J�����Čv�Z����
		RecalcCameraParams();
	}

	m_pRay->m_position = m_pCamera->m_vecEye;
}

//--------------------------------------------------------------------------------
// �v���C���[�ɒ��ڂ���
// ���@���FCPlayer*	�v���C���[�N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CPlayerCamera::UpdateLookAtPlayer(CPlayer * pPlayer)
{
	m_position = pPlayer->GetPosition() + CAMERA_ADJUST_HEIGHT;

	m_vecPosDelay = VectorCalculate::Lerp(m_vecPosDelay, m_position, CAMERA_DELAY_SPEED);
}

//--------------------------------------------------------------------------------
// �L�[����
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CPlayerCamera::UpdateKeyInput(CSystem * pSystem)
{
	if (pSystem->IsKeyDown(DIK_DOWN))	m_rotation.x -= D3DXToRadian(CAMERA_SPEED_KEY);
	if (pSystem->IsKeyDown(DIK_UP))		m_rotation.x += D3DXToRadian(CAMERA_SPEED_KEY);
	if (pSystem->IsKeyDown(DIK_LEFT))	m_rotation.y -= D3DXToRadian(CAMERA_SPEED_KEY);
	if (pSystem->IsKeyDown(DIK_RIGHT))	m_rotation.y += D3DXToRadian(CAMERA_SPEED_KEY);

	m_rotation.x -= D3DXToRadian(pSystem->GetMouseMove().y) * CAMERA_SPEED_MOUSE;
	m_rotation.y += D3DXToRadian(pSystem->GetMouseMove().x) * CAMERA_SPEED_MOUSE;

	if(fabsf(pSystem->GetJoyStickState(KeyConfig::RIGHTVER)) > 0.1f)
		m_rotation.x -= D3DXToRadian(pSystem->GetJoyStickState(KeyConfig::RIGHTVER)) * CAMERA_SPEED_JOYSTICK;

	if (fabsf(pSystem->GetJoyStickState(KeyConfig::RIGHTHOR)) > 0.1f)
		m_rotation.y += D3DXToRadian(pSystem->GetJoyStickState(KeyConfig::RIGHTHOR)) * CAMERA_SPEED_JOYSTICK;
}

//--------------------------------------------------------------------------------
// �J��������
// ���@���FCPlayer*	�v���C���[�N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CPlayerCamera::UpdateController(CPlayer * pPlayer)
{
	m_fovDelay = VectorCalculate::Lerp(m_fovDelay, m_fov, CAMERA_FOV_SPEED);

	m_rotation.x = VectorCalculate::Clamp(m_rotation.x, CAMERA_UP_LIMIT, CAMERA_DOWN_LIMIT);

	m_rotation.y = Calculate::PI360(m_rotation.y);
	float diff = m_rotation.y - m_vecRotDelay.y;
	diff = Calculate::PI180(diff);
	m_vecRotDelay.y += diff * CAMERA_DELAY_SPEED;
	m_vecRotDelay.y = Calculate::PI180(m_vecRotDelay.y);

	m_vecRotDelay.x += (m_rotation.x - m_vecRotDelay.x) * CAMERA_DELAY_SPEED;
}

//--------------------------------------------------------------------------------
// �_�b�V�����̃J�����̗h��
// ���@���FCPlayer*	�v���C���[�N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CPlayerCamera::UpdateDashShake(CPlayer* pPlayer)
{
	if (pPlayer->IsDash())
	{
		m_fov = Mathf::ToRadian(80.0f);
		m_shake += CAMERA_SHAKE_SPEED;
		m_rotation.x += Mathf::ToRadian(cosf(m_shake) * CAMERA_DELAY_SPEED);
	}
	else {
		m_fov = Mathf::ToRadian(60.0f);
		m_shake = 0.0f;
	}
}

//--------------------------------------------------------------------------------
// ���b�N�I��
//--------------------------------------------------------------------------------
void CPlayerCamera::UpdateLockOnSystem(CPlayer * pPlayer, const float angle)
{
	if (pPlayer->IsLockOn())
	{
		m_rotation.y = m_vecRotDelay.y = angle;
	}
}

//--------------------------------------------------------------------------------
// �J�����̃p�����[�^�ݒ�
//--------------------------------------------------------------------------------
void CPlayerCamera::SetCameraParams()
{
	m_pCamera->SetFov(m_fovDelay);
	m_pCamera->SetThirdPerson(m_vecPosDelay, m_vecRotDelay, 0.0f, CAMERA_TRACKING_DIST);
}

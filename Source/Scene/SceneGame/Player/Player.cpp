//********************************************************************************
//
// Player.hpp
//
// �v���C���[�N���X
//
// Date: 2019/07/30 �X�V
//
//********************************************************************************

//================================================================================
// �C���N���[�h
//================================================================================
#include "Player.hpp"

using namespace FileConst;

//================================================================================
// �萔��`
//================================================================================
const float STAMINA_REDUCE = 0.35f;
const float STAMINA_RECOVERY = 0.25;
const float STAMINA_MAX = 100.0f;
const Vector3 ADJUST_EFFECT_POS = Vector3(0.0f, 1.0f, 0.0f);

//================================================================================
// �N���X��`
//================================================================================

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
CPlayer::CPlayer()
	: m_pModel(new CXFileModel)
	, m_pAnimation(new CPlayerAnimation(m_pModel))
	, m_pBodyBox(new CBoundingBox)
	, m_pSword(new CSword)
	, m_pRay(new CRay)
	, m_pFootLeft(new CRay)
	, m_pFootRight(new CRay)
	, m_vecInputDirection(VectorCalculate::Zero)
	, m_vecDirection(VectorCalculate::Zero)
	, m_hp(100.0f)
	, m_atk(4.0f)
	, m_stamina(100.0f)
	, m_moveSpeed(0.05f)
	, m_moveSpeedDelay(0.05f)
	, m_padSpeed(0.0f)
	, m_isDashKey(false)
	, m_isDash(false)
	, m_isDeath(false)
	, m_isAttack(false)
	, m_isLockOn(false)
{
	LoadParams("Player");
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
CPlayer::~CPlayer()
{
	SafeDelete(m_pFootLeft);
	SafeDelete(m_pFootRight);
	SafeDelete(m_pAnimation);
	SafeDelete(m_pRay);
	SafeDelete(m_pModel);
	SafeDelete(m_pBodyBox);
	SafeDelete(m_pSword);
}

//--------------------------------------------------------------------------------
// �t�@�C���ǂݍ���
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CPlayer::LoadFile(CSystem * pSystem)
{
	pSystem->LoadXFile(MODEL_PLAYER, "Resource/Models/Player/player.x");
	pSystem->LoadXFile(MODEL_SWORD,	 "Resource/Models/Player/Sword.x");
}

//--------------------------------------------------------------------------------
// ������
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CPlayer::Initialize(CSystem * pSystem)
{
	//���f���̐ݒ�
	m_pModel->SetModel(pSystem->GetXFile(MODEL_PLAYER));
	m_pModel->SetAdvanceTime(1.0 / 50.0f);
	m_pModel->SetBoneFrame("right_momo", "right_hiza", "right_sune");
	m_pModel->SetBoneFrame("left_momo", "left_hiza", "left_sune");
	
	//�{�f�B�p�̃o�E���f�B���O�{�b�N�X����
	auto max = Vector3(0.25f, 1.8f, 0.25f);
	auto min = Vector3(-0.25f, 0.0f, -0.25f);
	m_pBodyBox->Create(pSystem, &max, &min, false);
	
	//���̏�����
	m_pSword->Initialize(pSystem);
}

//--------------------------------------------------------------------------------
// �X�V
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CPlayer::Update(CSystem * pSystem, const Vector3 vecAngle)
{
	if(m_isSleep == false)
	{
		CenterCursorPos();
		UpdateDeath();
		UpdateKeyInput(pSystem);
		UpdatePosition();
		UpdateFootIK();
		UpdateController(pSystem, vecAngle);
		UpdateStamina();
		SetPlayerMatrix();
		SetSwordMatrix();
		CGameObject::Update(pSystem);
	}

	m_pAnimation->Update();
}

//--------------------------------------------------------------------------------
// �`��
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CPlayer::ZDraw(CSystem * pSystem)
{
	m_pModel->Draw( pSystem, Shader::DEPTH, m_isVisible, false );
	m_pSword->ZDraw( pSystem );
}

//--------------------------------------------------------------------------------
// �`��
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CPlayer::Draw(CSystem * pSystem)
{
	m_pModel->Draw(pSystem, Shader::SHADOW, m_isVisible, !m_isSleep);
	m_pBodyBox->Draw(pSystem);
	m_pSword->Draw(pSystem);
}

//--------------------------------------------------------------------------------
// �G����U�����󂯂��Ƃ��̏���
// ���@���FCSystem*		�V�X�e���N���X�̃|�C���^
// �@�@�@�@const float	�U����
//--------------------------------------------------------------------------------
void CPlayer::SetHitEnemyAttack(CSystem* pSystem, const float atk)
{
	if (!m_isDeath)
	{
		auto adjust = m_position + ADJUST_EFFECT_POS;
		pSystem->PlayEffect(2, &adjust);
		pSystem->PlaySe(SE_DAMAGE2);
		m_isEnemyHit = true;
		m_hp -= atk;
	}
}

//--------------------------------------------------------------------------------
// �G�l�~�[����U���󂯂���������Z�b�g
//--------------------------------------------------------------------------------
void CPlayer::EnemyHitReset()
{
	m_isEnemyHit = false;
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// �J�[�\���ʒu�����Œ�
//--------------------------------------------------------------------------------
void CPlayer::CenterCursorPos()
{
#ifndef _DEBUG
	int halfWidth = GetSystemMetrics(SM_CXSCREEN) / 2;
	int halfHeight = GetSystemMetrics(SM_CYSCREEN) / 2;
	SetCursorPos(halfWidth, halfHeight);
#endif
}

//--------------------------------------------------------------------------------
// ���S�󋵂̍X�V
//--------------------------------------------------------------------------------
void CPlayer::UpdateDeath()
{
	if (m_hp <= 0.0f)
	{
		if (!m_isDeath)
		{
			m_hp = 0.0f;
			m_isDeath = true;
			m_pAnimation->SetDeath();
		}
	}

	if (m_pAnimation->IsDeath())
	{
		m_isAlive = false;
	}
}

//--------------------------------------------------------------------------------
// �ʒu�̍X�V
//--------------------------------------------------------------------------------
void CPlayer::UpdatePosition()
{
	m_position.y = m_pRay->m_position.y;

	auto angle = D3DXToRadian(65.0f);
	auto up = Vector3(0.0f, 1.0f, 0.0f);
	auto dot = D3DXVec3Dot(&up, &m_pRay->m_normal);

	if (m_padSpeed > 0.1f)
	{
		if (dot <= angle)
		{
			auto penetration = dot;

			if (penetration > 0.0f)
			{
				m_vecMovement.x = m_pRay->m_normal.x * penetration / 5.0f;
				m_vecMovement.z = m_pRay->m_normal.z * penetration / 5.0f;
			}
		}
	}

	m_position += m_vecMovement;

	m_vecMovement = VectorCalculate::Zero;

	m_pRay->m_position = m_position;
}

//--------------------------------------------------------------------------------
// ����IK�ʒu�ݒ�
//--------------------------------------------------------------------------------
void CPlayer::UpdateFootIK()
{
	auto footRight = m_pFootRight->m_position;
	auto footLeft = m_pFootLeft->m_position;
	m_pModel->SetTargetVector(RIGHT_FOOT, footRight);
	m_pModel->SetTargetVector(LEFT_FOOT, footLeft);
	m_pFootRight->m_position = m_pModel->GetBonePos(RIGHT_FOOT);
	m_pFootLeft->m_position = m_pModel->GetBonePos(LEFT_FOOT);
}

//--------------------------------------------------------------------------------
// �v���C���[�̍s��ݒ�
//--------------------------------------------------------------------------------
void CPlayer::SetPlayerMatrix()
{
	m_pModel->SetTransform(&m_matWorld);
	m_pBodyBox->SetTransform(&m_matWorld);
}

//--------------------------------------------------------------------------------
// ���̍s��ݒ�
//--------------------------------------------------------------------------------
void CPlayer::SetSwordMatrix()
{
	Matrix matWorld;
	m_pModel->GetMatrixFromFrameName("left_hand", &matWorld);
	m_pSword->SetTransform(&matWorld);
}

//--------------------------------------------------------------------------------
// ���͏���
// ���@���FCSystem*			�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CPlayer::UpdateKeyInput(CSystem * pSystem)
{
	m_vecInputDirection = VectorCalculate::Zero;

	if (pSystem->IsKeyPressed(DIK_R) || pSystem->IsPadPressed(KeyConfig::SQUARE))
	{
		m_isLockOn = !m_isLockOn;
	}

	if ( m_pAnimation->IsLockAni() ) return; //�A�j���[�V�������b�N���͓����܂���B
	
	m_vecInputDirection.x = pSystem->GetJoyStickState(KeyConfig::LEFTHOR);
	m_vecInputDirection.z = pSystem->GetJoyStickState(KeyConfig::LEFTVER);

	if (pSystem->IsKeyDown(DIK_W)) m_vecInputDirection.z = -1.0f;
	if (pSystem->IsKeyDown(DIK_S)) m_vecInputDirection.z = 1.0f;
	if (pSystem->IsKeyDown(DIK_A)) m_vecInputDirection.x = -1.0f;
	if (pSystem->IsKeyDown(DIK_D)) m_vecInputDirection.x = 1.0f;

	m_isDashKey = pSystem->IsKeyDown(DIK_LSHIFT) || pSystem->IsPadDown(KeyConfig::L1);
	m_isAttack = pSystem->IsKeyDown(DIK_E) || pSystem->IsPadDown(KeyConfig::R1) || pSystem->IsMouseDown(KeyConfig::DIK_LBTN);
}

//--------------------------------------------------------------------------------
// �v���C���[�̑���
// ���@���FCSystem*			�V�X�e���N���X�̃|�C���^
// �@�@�@�@const Vector3	��]�A���O��
//--------------------------------------------------------------------------------
void CPlayer::UpdateController(CSystem * pSystem, const Vector3 vecAngle)
{
	m_vecDirection = VectorCalculate::Lerp(m_vecDirection, m_vecInputDirection, m_delaySpeed );

	m_padSpeed = VectorCalculate::Magnitude(m_vecDirection);
	m_padSpeed = VectorCalculate::Clamp(m_padSpeed, 0.0f, 1.0f);

	if ( m_padSpeed > 0.1f )
	{
		ModelRotate(m_vecDirection, vecAngle.y );

		float angle = -vecAngle.y + atan2f(m_vecDirection.z, -m_vecDirection.x );
		m_position.x += cosf(angle) * (m_padSpeed * m_moveSpeedDelay);
		m_position.z += sinf(angle) * (m_padSpeed * m_moveSpeedDelay);
		m_pAnimation->SetChangeAni( PlayerAnimation::Action::WALK );
	}
	else {
		m_isDash = false;

		m_pAnimation->SetChangeAni( PlayerAnimation::Action::IDLE );
	}

	if ( m_isAttack )
	{
		m_pAnimation->SetChangeAni( PlayerAnimation::Action::ATTACK );
	}
}

//--------------------------------------------------------------------------------
// ���f����]
// ���@���Fconst D3DXVECTOR3&	�R���������x�N�g��
// �@�@�@�@const float			�J�����̃A���O��
//--------------------------------------------------------------------------------
void CPlayer::ModelRotate(const Vector3& vecDir, const float camAngle)
{
	float rotAngle = 0.0f;

	if (m_isLockOn)
	{
		if (vecDir.z > 0.5f)
		{
			rotAngle = camAngle - atan2f(-fabsf(vecDir.z), vecDir.x);
		}
		else {
			rotAngle = camAngle - atan2f(-fabsf(vecDir.z), -vecDir.x);
		}
	}
	else {
		rotAngle = camAngle - atan2f(vecDir.z, -vecDir.x);
	}

	rotAngle -= D3DX_PI * 0.50f;
	Calculate::LerpRot( rotAngle, m_delaySpeed, &m_rotation.y );
}

//--------------------------------------------------------------------------------
// �X�^�~�i�̍X�V
//--------------------------------------------------------------------------------
void CPlayer::UpdateStamina()
{
	if (m_isDashKey && m_padSpeed > 0.1f)
	{
		if (m_stamina > 0.0f)
		{
			m_isDash = true;
			m_stamina -= STAMINA_REDUCE;
			m_moveSpeed = m_dashSpeed;
			m_pAnimation->SetChangeAni(PlayerAnimation::Action::DASH);
		}
		else {
			m_isDash = false;
			m_moveSpeed = m_walkSpeed;
		}
	}
	else {
		if (m_stamina < STAMINA_MAX)
		{
			m_isDash = false;
			m_moveSpeed = m_walkSpeed;
			m_stamina += STAMINA_RECOVERY;
		}
	}

	m_moveSpeedDelay = VectorCalculate::Lerp(m_moveSpeedDelay, m_moveSpeed, m_delaySpeed);
}

//--------------------------------------------------------------------------------
// �p�����[�^�̎擾
// ���@���Fconst string �t�@�C����
//--------------------------------------------------------------------------------
void CPlayer::LoadParams(const string strFileName)
{
	std::string dir = "Resource/Data/Player/" + strFileName + ".txt";
	std::ifstream ifs;
	ifs.open(dir);

	if (!ifs)
	{
		this->InitParams(strFileName);
		this->LoadParams(strFileName);
		return;
	}

	std::string str = "";
	std::vector<std::string> strAry;
	while (std::getline(ifs, str)) //������
	{
		if (std::getline(ifs, str)) //���l
		{
			strAry.push_back(str);
		}
	}

	m_hp		= (float)atof(strAry[0].c_str());
	m_atk		= (float)atof(strAry[1].c_str());
	m_walkSpeed = (float)atof(strAry[2].c_str());
	m_dashSpeed = (float)atof(strAry[3].c_str());
	m_delaySpeed = (float)atof(strAry[4].c_str());

	ifs.close();
}

//--------------------------------------------------------------------------------
// �p�����[�^�̕ۑ�
// ���@���Fconst string �t�@�C����
//--------------------------------------------------------------------------------
void CPlayer::InitParams(const string strFileName)
{
	std::string dir = "Resource/Data/Player/" + strFileName + ".txt";
	std::ofstream ofs;
	ofs.open(dir);

	ofs << "HP: Default(100)" << std::endl;
	ofs << "100" << std::endl;
	ofs << "ATK: Default(6.0)" << std::endl;
	ofs << "6.0" << std::endl;
	ofs << "WalkSpeed: Default(0.05)" << std::endl;
	ofs << "0.05" << std::endl;
	ofs << "DashSpeed: Default(0.1)" << std::endl;
	ofs << "0.1" << std::endl;
	ofs << "DelaySpeed: Default(0.1)" << std::endl;
	ofs << "0.1" << std::endl;

	ofs.close();
}

//********************************************************************************
//
// BossBase.hpp
//
// �{�X�I�u�W�F�N�g�̊��N���X
//
// Date: 2019/10/06 �X�V
//
//********************************************************************************

//================================================================================
// �C���N���[�h
//================================================================================
#include "BossBase.hpp"

using namespace FileConst;

//================================================================================
// �萔��`
//================================================================================
const Vector3 EFFECT_ADJUST_POS(0.0f, 1.0f, 0.0f);

//================================================================================
// �N���X��`
//================================================================================

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
CBossBase::CBossBase()
	: m_pModel(new CXFileModel)
	, m_pBodyBox(new CBoundingBox)
	, m_pAttackBox(new CBoundingBox)
	, m_pRay(new CRay)
	, m_attackStartTime(0.0)
	, m_deathStartTime(0.0)
	, m_attackTimer(0.0)
	, m_hp(0.0f)
	, m_atk(0.0f)
	, m_walkSpeed(0.0f)
	, m_longDist(0.0f)
	, m_intermediateDist(0.0f)
	, m_shortDist(0.0f)
	, m_rotateSpd(0.0f)
	, m_attackInterval(0.0f)
	, m_distance(0.0)
	, m_isDeath(false)
	, m_isHit(false)
	, m_isHitPlayer(false)
	, m_isDiscovery(false)
{
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
CBossBase::~CBossBase()
{
	SafeDelete(m_pModel);
	SafeDelete(m_pBodyBox);
	SafeDelete(m_pAttackBox);
	SafeDelete(m_pRay);
}

//--------------------------------------------------------------------------------
// �X�V
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
// �@�@�@�@CPlayer*	�v���C���[�N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CBossBase::Update(CSystem * pSystem, CPlayer* pPlayer)
{
	if (!m_isSleep)
	{
		CBossBase::UpdateDeath();
		CGameObject::Update(pSystem);

		m_pModel->SetTransform(&m_matWorld);
		m_pBodyBox->SetTransform(&m_matWorld);
	}
}

//--------------------------------------------------------------------------------
// �`��
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CBossBase::ZDraw(CSystem * pSystem)
{
	m_pModel->Draw(pSystem, DEPTH, m_isVisible, false);
}

//--------------------------------------------------------------------------------
// �`��
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CBossBase::Draw(CSystem * pSystem)
{
	m_pModel->Draw(pSystem, SHADOW, m_isVisible, !m_isSleep);
	m_pBodyBox->Draw(pSystem);
	m_pAttackBox->Draw(pSystem);
}

//--------------------------------------------------------------------------------
// �q�b�g������HP�����炷
// ���@���FCSystem*		�V�X�e���N���X�̃|�C���^
// �@�@�@�@CShowDamage*	�_���[�W�\���N���X�̃|�C���^
// �@�@�@�@const float	�U����
//--------------------------------------------------------------------------------
void CBossBase::SetHitPlayerAttack(CSystem * pSystem, CShowDamage * pDamage, const float atk)
{
	if (!m_isHit)
	{
		m_hp -= atk;
		m_isHit = true;

		Vector3 position = m_position + EFFECT_ADJUST_POS;
		pSystem->PlayEffect(0, &position);

		pDamage->AddDamage(pSystem, position, (int)atk);

		pSystem->PlaySe(SE_DAMAGE1);
	}
}

//--------------------------------------------------------------------------------
// �v���C���[�U���A�j���[�V�������I��������
//--------------------------------------------------------------------------------
void CBossBase::SetHitReset()
{
	if (m_isHit)
	{
		m_isHit = false;
	}
}

//--------------------------------------------------------------------------------
// �v���C���[�ւ̍U�������������Ƃ�
// ���@�l�F����͈�x����̍U���������邱�Ƃ�����
//--------------------------------------------------------------------------------
void CBossBase::SetPlayerHit()
{
	m_isHitPlayer = true;
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// ���S�󋵂̍X�V
//--------------------------------------------------------------------------------
void CBossBase::UpdateDeath()
{
	if (m_hp <= 0)
	{
		if (!m_isDeath)
		{
			m_isDeath = true;
			m_deathStartTime = m_pModel->GetTime();
		}
	}
}

//--------------------------------------------------------------------------------
// �O�����̍X�V
// ���@���Fconst Vector3	�O�����x�N�g��
//--------------------------------------------------------------------------------
void CBossBase::UpdateForward(const Vector3 vecForward)
{
	Matrix matRotate;
	D3DXMatrixRotationY(&matRotate, m_rotation.y);
	D3DXVec3TransformCoord(&m_vecForward, &vecForward, &matRotate);
}

//--------------------------------------------------------------------------------
// ���f���̐ݒ�
// ���@���FCSystem*		�V�X�e���N���X�̃|�C���^
// �@�@�@�@const int	�o�^�������f���N���X�̔ԍ�
//--------------------------------------------------------------------------------
void CBossBase::SetModel(CSystem * pSystem, const int fileConst)
{
	m_pModel->SetModel(pSystem->GetXFile(fileConst));
	m_pModel->SetAdvanceTime(1.0 / 60.0);
}

//--------------------------------------------------------------------------------
// �U���̃o�E���f�B���O�{�b�N�X�ݒ�
// ���@���Fconst string ���f���̃{�[����
//--------------------------------------------------------------------------------
void CBossBase::SetAttackBox(const string strBoneName)
{
	D3DXMATRIX matWorld;
	m_pModel->GetMatrixFromFrameName(strBoneName, &matWorld);
	m_pAttackBox->SetTransform(&matWorld);
}

//--------------------------------------------------------------------------------
// �p�����[�^�̎擾
// ���@���Fconst string �t�@�C����
//--------------------------------------------------------------------------------
void CBossBase::LoadParams(const string strFileName)
{
	string dir = "Resource/Data/Boss/" + strFileName + ".txt";
	ifstream ifs;
	ifs.open(dir);
	if (!ifs)
	{
		this->InitParams(strFileName);
		this->LoadParams(strFileName);
		return;
	}

	string str = "";
	vector<string> strAry;
	while (getline(ifs, str)) //������
	{
		if (getline(ifs, str)) //���l
		{
			strAry.push_back(str);
		}
	}

	m_hp				= (float)atof(strAry[0].c_str());
	m_atk				= (float)atof(strAry[1].c_str());
	m_walkSpeed			= (float)atof(strAry[2].c_str());
	m_longDist			= (float)atof(strAry[3].c_str());
	m_intermediateDist	= (float)atof(strAry[4].c_str());
	m_shortDist			= (float)atof(strAry[5].c_str());
	m_rotateSpd			= (float)atof(strAry[6].c_str());
	m_attackInterval	=		 atof(strAry[7].c_str());
	m_radius			= (float)atof(strAry[8].c_str());

	ifs.close();
}

//--------------------------------------------------------------------------------
// �p�����[�^�̕ۑ�
// ���@���Fconst string �t�@�C����
//--------------------------------------------------------------------------------
void CBossBase::InitParams(const string strFileName)
{
	string dir = "Resource/Data/Boss/" + strFileName + ".txt";
	ofstream ofs;
	ofs.open(dir);

	ofs << "HP: Default(100)" << endl;
	ofs << "100" << endl;

	ofs << "ATK: Default(6.0)" << endl;
	ofs << "6.0" << endl;

	ofs << "WalkSpeed: Default(0.04)" << endl;
	ofs << "0.04" << endl;

	ofs << "LongDistance: Default(4.5)" << endl;
	ofs << "4.5" << endl;

	ofs << "IntermediateDistance: Default(3.5)" << endl;
	ofs << "3.5" << endl;

	ofs << "ShortDistance: Default(2.5)" << endl;
	ofs << "2.5" << endl;

	ofs << "RotateSpeed: Default(2.0)" << endl;
	ofs << "2.0" << endl;

	ofs << "AttackIntervalTime: Default(2000)" << endl;
	ofs << "2000" << endl;

	ofs << "Radius: Default(0.9)" << endl;
	ofs << "0.9";

	ofs.close();
}

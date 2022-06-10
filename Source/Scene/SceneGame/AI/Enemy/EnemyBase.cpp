//********************************************************************************
//
// EnemyBase.hpp
//
// �I�u�W�F�N�g�N���X�̃e���v���[�g
//
// Date: 2019/07/30 �X�V
//
//********************************************************************************

//================================================================================
// �C���N���[�h
//================================================================================
#include "EnemyBase.hpp"

using namespace FileConst;

//================================================================================
// �N���X��`
//================================================================================

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
CEnemyBase::CEnemyBase()
	: m_pModel(new CXFileModel)
	, m_pBodyBox(new CBoundingBox)
	, m_pAttackBox(new CBoundingBox)
	, m_pRay(new CRay)
	, m_vecForward(Vector3(0.0f, 0.0f, 0.0f))
	, m_attackStartTime(0.0)
	, m_deathStartTime(0.0)
	, m_attackTimer(0.0)
	, m_distLimit(0.0f)
	, m_distance(0.0f)
	, m_hp(0.0f)
	, m_atk(0.0f)
	, m_meshAlpha(1.0f)
	, m_isDiscovery(false)
	, m_isDeath(false)
	, m_isHit(false)
	, m_isHitPlayer(false)
{
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
CEnemyBase::~CEnemyBase()
{
	SafeDelete(m_pRay);
	SafeDelete(m_pModel);
	SafeDelete(m_pBodyBox);
	SafeDelete(m_pAttackBox);
}

//--------------------------------------------------------------------------------
// �X�V
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
// �@�@�@�@CPlayer*	�v���C���[�N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CEnemyBase::Update(CSystem * pSystem, CPlayer* pPlayer)
{
	this->UpdateDeath();
	this->UpdatePosition();
	CGameObject::Update(pSystem);
	this->UpdateMatrix();
}

//--------------------------------------------------------------------------------
// �`��
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CEnemyBase::ZDraw(CSystem * pSystem)
{
	m_pModel->Draw(pSystem, DEPTH, m_isVisible, false);
}

//--------------------------------------------------------------------------------
// �`��
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CEnemyBase::Draw(CSystem * pSystem)
{
	m_pModel->SetMeshAlpha(m_meshAlpha);
	m_pModel->Draw(pSystem, SHADOW, m_isVisible, !m_isSleep);
	m_pBodyBox->Draw(pSystem);
	m_pAttackBox->Draw(pSystem);
}

//--------------------------------------------------------------------------------
// �o�����̈ʒu(�����_����)
//--------------------------------------------------------------------------------
void CEnemyBase::SpawnRandomPos(const float radius)
{
	//��ƂȂ�ʒu���w�肷��
	D3DXVECTOR3 basePos = m_position;

	float randomLen = Calculate::Random(0.0f, radius);
	float randomAngle = Calculate::Random(0.0f, Mathf::ToRadian(360));

	basePos.x += randomLen * cosf(randomAngle);
	basePos.z += randomLen * sinf(randomAngle);

	m_rotation.y = randomAngle;

	m_position = D3DXVECTOR3(basePos.x, 0.0f, basePos.z);

	m_vecBasePos = m_position;
}

//--------------------------------------------------------------------------------
// �q�b�g������HP�����炷
// ���@���Fconst float	�U����
//--------------------------------------------------------------------------------
void CEnemyBase::SetHitPlayerAttack(CSystem * pSystem, CShowDamage* pDamage, const float atk)
{
	if (!m_isHit)
	{
		m_hp -= atk;
		m_isHit = true;

		Vector3 vecPos = m_position + Vector3(0.0f, 1.0f, 0.0f);
		pSystem->PlayEffect(0, &vecPos);

		pDamage->AddDamage(pSystem, vecPos ,(int)atk);

		pSystem->PlaySe(SE_DAMAGE1);
	}
}

//--------------------------------------------------------------------------------
// �v���C���[�U���A�j���[�V�������I��������
//--------------------------------------------------------------------------------
void CEnemyBase::SetHitReset()
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
void CEnemyBase::SetPlayerHit()
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
void CEnemyBase::UpdateDeath()
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
// ���E�s��̍X�V
//--------------------------------------------------------------------------------
void CEnemyBase::UpdateMatrix()
{
	m_pModel->SetTransform(&m_matWorld);
	m_pBodyBox->SetTransform(&m_matWorld);
	m_pAttackBox->SetTransform(&m_matWorld);
}

//--------------------------------------------------------------------------------
// �ʒu�̍X�V
//--------------------------------------------------------------------------------
void CEnemyBase::UpdatePosition()
{
	m_position.y = m_pRay->m_position.y;

	auto move = VectorCalculate::Zero;
	auto angle = D3DXToRadian(65.0f);
	auto up = Vector3(0.0f, 1.0f, 0.0f);
	auto dot = D3DXVec3Dot(&up, &m_pRay->m_normal);

	if (dot <= angle)
	{
		auto penetration = dot;

		if (penetration > 0.0f)
		{
			move.x = m_pRay->m_normal.x * penetration / 8.0f;
			move.z = m_pRay->m_normal.z * penetration / 8.0f;
		}
	}

	m_position += move;

	//�ʒu�̍X�V
	m_pRay->m_position = m_position;
}

//--------------------------------------------------------------------------------
// �̂̃o�E���f�B���O�{�b�N�X����
//--------------------------------------------------------------------------------
void CEnemyBase::CreateBodyBox(CSystem* pSystem, const Vector3 vecMax, const Vector3 vecMin)
{
	m_pBodyBox->Create(pSystem, &vecMax, &vecMin, false);
}

//--------------------------------------------------------------------------------
// �U���̃o�E���f�B���O�{�b�N�X����
//--------------------------------------------------------------------------------
void CEnemyBase::CreateAttackBox(CSystem * pSystem, const Vector3 vecMax, const Vector3 vecMin)
{
	m_pAttackBox->Create(pSystem, &vecMax, &vecMin, false);
}

//--------------------------------------------------------------------------------
// �O�����̍X�V
//--------------------------------------------------------------------------------
void CEnemyBase::UpdateForward(const Vector3 vecForward)
{
	Matrix matRotate;
	D3DXMatrixRotationY(&matRotate, m_rotation.y);
	D3DXVec3TransformCoord(&m_vecForward, &vecForward, &matRotate);
}

//--------------------------------------------------------------------------------
// �p�����[�^�̎擾
// ���@���Fconst string �t�@�C����
//--------------------------------------------------------------------------------
void CEnemyBase::LoadParams(const string strFileName)
{
	string dir = "Resource/Data/Enemy/" + strFileName + ".txt";
	ifstream ifs;
	ifs.open(dir);
	if (!ifs)
	{
		InitParams(strFileName);
		LoadParams(strFileName);
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
	m_dashSpeed			= (float)atof(strAry[3].c_str());
	m_longDist			= (float)atof(strAry[4].c_str());
	m_intermediateDist	= (float)atof(strAry[5].c_str());
	m_shortDist			= (float)atof(strAry[6].c_str());
	m_rotateSpd			= (float)atof(strAry[7].c_str());
	m_attackInterval	=		 atof(strAry[8].c_str());
	m_radius			= (float)atof(strAry[9].c_str());

	ifs.close();
}

//--------------------------------------------------------------------------------
// �p�����[�^�̕ۑ�
// ���@���Fconst string �t�@�C����
//--------------------------------------------------------------------------------
void CEnemyBase::InitParams(const string strFileName)
{
	string dir = "Resource/Data/Enemy/" + strFileName + ".txt";
	ofstream ofs;
	ofs.open(dir);

	ofs << "HP: Default(10)" << endl;
	ofs << "10" << endl;

	ofs << "ATK: Default(6.0)" << endl;
	ofs << "6.0" << endl;

	ofs << "WalkSpeed: Default(0.04)" << endl;
	ofs << "0.04" << endl;

	ofs << "DashSpeed: Default(0.06)" << endl;
	ofs << "0.06" << endl;

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

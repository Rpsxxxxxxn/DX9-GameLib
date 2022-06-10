//********************************************************************************
//
// Guide.hpp
//
// �K�C�h�\���p�N���X
//
// Date: 2019/07/30 �X�V
//
//********************************************************************************

//================================================================================
// �C���N���[�h
//================================================================================
#include "Guide.hpp"

using namespace FileConst;

//================================================================================
// �N���X��`
//================================================================================

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
CGuide::CGuide()
	: m_showDistance(5.0f)
	, m_isShowAttackGuide(false)
{
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
CGuide::~CGuide()
{
	for (auto w : m_pSprite)
	{
		SafeDelete(w);
	}
}

//--------------------------------------------------------------------------------
// �t�@�C���ǂݍ���
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGuide::LoadFile(CSystem * pSystem)
{
	if (pSystem->IsControllerConnect())
	{
		pSystem->LoadTexture(TEXTURE_ATTACK_KEY, "Resource/Textures/Guide/Attack_Pad.png");
		pSystem->LoadTexture(TEXTURE_GUIDE_KEY,	 "Resource/Textures/Guide/Guide_Pad.png");
	}
	else 
	{
		pSystem->LoadTexture(TEXTURE_ATTACK_KEY, "Resource/Textures/Guide/Attack_Key.png");
		pSystem->LoadTexture(TEXTURE_GUIDE_KEY,	 "Resource/Textures/Guide/Guide_Key.png");
	}
}

//--------------------------------------------------------------------------------
// ������
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGuide::Initialize(CSystem * pSystem)
{
	m_halfWidth = pSystem->GetWindowSettings().Width / 2.0f;
	m_halfHeight = pSystem->GetWindowSettings().Height / 2.0f;

	m_pSprite[ST_GUIDE]		= new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_GUIDE_KEY));
	m_pSprite[ST_ATTACK]	= new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_ATTACK_KEY));

	ShowGuide();
}

//--------------------------------------------------------------------------------
// �X�V
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
// �@�@�@�@CPlayer*	�v���C���[�N���X�̃|�C���^
// �@�@�@�@CAIManager*	�G�Ǘ��N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGuide::Update(CSystem * pSystem, CPlayer* pPlayer, CAIManager* pAI)
{
	CalcAIAndPlayerDistance(pPlayer, pAI);

	ShowAttackGuide();
}

//--------------------------------------------------------------------------------
// �`��
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGuide::Draw(CSystem * pSystem)
{
	for (auto i = 0; i < ST_MAX; i++)
	{
		m_pSprite[i]->Draw(pSystem);
	}
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// �G�Ǝ����̋����v�Z
// ���@���FCPlayer*	�v���C���[�N���X�̃|�C���^
// �@�@�@�@CAIManager*	�G�Ǘ��N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGuide::CalcAIAndPlayerDistance(CPlayer* pPlayer, CAIManager* pAI)
{
	m_isShowAttackGuide = false;

	for (auto w : pAI->GetEnemyList())
	{
		auto enemy = w->GetPosition();
		auto player = pPlayer->GetPosition();
		auto dist = enemy - player;
		auto magnitude = VectorCalculate::Magnitude(dist);
		if (magnitude < m_showDistance)
		{
			m_isShowAttackGuide = true;
		}
	}

	if (pAI->IsAddedBoss())
	{
		auto boss = pAI->GetPosition();
		auto player = pPlayer->GetPosition();
		auto dist = boss - player;
		auto magnitude = VectorCalculate::Magnitude(dist);
		if (magnitude < m_showDistance)
		{
			m_isShowAttackGuide = true;
		}
	}
}

//--------------------------------------------------------------------------------
// �K�C�h�̕\��(�U��)
//--------------------------------------------------------------------------------
void CGuide::ShowAttackGuide()
{
	if (m_isShowAttackGuide)
	{
		m_pSprite[ST_ATTACK]->SetNewAlpha(255);
	}
	else 
	{
		m_pSprite[ST_ATTACK]->SetNewAlpha(0);
	}
}

//--------------------------------------------------------------------------------
// �K�C�h�̕\��
//--------------------------------------------------------------------------------
void CGuide::ShowGuide()
{
	m_pSprite[ST_ATTACK]->SetPosition(Vector2(m_halfWidth - 108.5f, m_halfHeight + (m_halfHeight / 2.0f)));
	m_pSprite[ST_GUIDE]->SetPosition(Vector2(5.0f, m_halfHeight + (m_halfHeight / 4.0f)));
	m_pSprite[ST_GUIDE]->SetAlpha(255);
}

//--------------------------------------------------------------------------------
// �p�����[�^�̎擾
// ���@���Fconst string �t�@�C����
//--------------------------------------------------------------------------------
void CGuide::LoadParams(const string strFileName)
{
	string dir = "Resource/Data/Guide/" + strFileName + ".txt";
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

	while (getline(ifs, str))
	{
		if (getline(ifs, str))
		{
			strAry.push_back(str);
		}
	}

	m_showDistance = (float)atof(strAry[0].c_str());

	ifs.close();
}

//--------------------------------------------------------------------------------
// �p�����[�^�̕ۑ�
// ���@���Fconst string �t�@�C����
//--------------------------------------------------------------------------------
void CGuide::InitParams(const string strFileName)
{
	string dir = "Resource/Data/Guide/" + strFileName + ".txt";
	ofstream ofs;
	ofs.open(dir);

	ofs << "ShowDistance:	default(3.0)" << endl;
	ofs << "3.0" << endl;

	ofs.close();
}

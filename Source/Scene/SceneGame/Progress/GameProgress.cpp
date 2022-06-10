//********************************************************************************
//
// GameProgress.hpp
//
// �Q�[�����̐i�s�x
//
// Date: 2019/07/30 �X�V
//
//********************************************************************************

//================================================================================
// �C���N���[�h
//================================================================================
#include "GameProgress.hpp"

using namespace FileConst;

//================================================================================
// �N���X��`
//================================================================================

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
CGameProgress::CGameProgress()
	: m_nowPlayBgm(BGM_GAME_FIELD1)
	, m_nowStage(1)
	, m_maxStage(3)
	, m_isNowFade(false)
	, m_isGameClear(false)
	, m_isAddedEnemy(false)
	, m_isAddedBoss(false)
	, m_isStageClear(false)
	, m_isGameOver(false)
	, m_isPlayerDeath(false)
	, m_addStartEnemyNum(0)
{
	LoadParams("GameProgress");
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
CGameProgress::~CGameProgress()
{
}

//--------------------------------------------------------------------------------
// �t�@�C���ǂݍ���
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGameProgress::LoadFile(CSystem * pSystem)
{
	pSystem->AddBgm(BGM_GAME_FIELD1,			"Resource/Sounds/Bgm/bgm_first.wav");
	pSystem->AddBgm(BGM_GAME_FIELD2,			"Resource/Sounds/Bgm/bgm_second.wav");
	pSystem->AddBgm(BGM_GAME_FIELD3,			"Resource/Sounds/Bgm/bgm_final.wav");
	pSystem->AddSe(SE_DAMAGE1,					"Resource/Sounds/Se/damage02.wav");
	pSystem->AddSe(SE_DAMAGE2,					"Resource/Sounds/Se/damage01.wav");
	pSystem->LoadTexture(TEXTURE_STAGE_CLEAR,	"Resource/Textures/GameClear/StageClear.png");
	pSystem->LoadTexture(TEXTURE_GAME_CLEAR,	"Resource/Textures/GameClear/GameClear.png");
	pSystem->LoadTexture(TEXTURE_GAME_OVER,		"Resource/Textures/GameOver/GameOver.png");
}

//--------------------------------------------------------------------------------
// ������
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGameProgress::Initialize(CSystem * pSystem)
{
	pSystem->PlayBgm(BGM_GAME_FIELD1);
}

//--------------------------------------------------------------------------------
// �X�V
// ���@���FCSystem*			�V�X�e���N���X�̃|�C���^
// �@�@�@�@CFieldManager*	�t�B�[���h�N���X�̃|�C���^
// �@�@�@�@CPlayer*			�v���C���[�N���X�̃|�C���^
// �@�@�@�@CAIManager*		�G�Ǘ��N���X�̃|�C���^
// �@�@�@�@CFadeAni*		�t�F�[�h�Ǘ��N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGameProgress::Update(
	CSystem * pSystem, 
	CFieldManager* pField, 
	CPlayer* pPlayer, 
	CAIManager* pAI,
	CFadeAni* pFade
)
{
	UpdateFadeAnimation(pSystem, pField, pFade);

	StageClearDecision();
	
	if (m_nowStage < m_maxStage)
	{
		AddStartEnemy(pSystem, pAI, pFade);

		EnemyExpeditionClearCriteria(pAI);
	}
	else 
	{
		AddSpawnBoss(pSystem, pAI, pFade);

		AddStartEnemy(pSystem, pAI, pFade);

		BossExpeditionClearCriteria(pAI);
	}

	PlayerDeathStatus(pPlayer, pFade);
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// �t�F�[�h�Ǘ�
// ���@���FCSystem*			�V�X�e���N���X�̃|�C���^
// �@�@�@�@CFieldManager*	�t�B�[���h�Ǘ��N���X�̃|�C���^
// �@�@�@�@CFadeAni*		�t�F�[�h�Ǘ��N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGameProgress::UpdateFadeAnimation(
	CSystem * pSystem,
	CFieldManager* pField,
	CFadeAni* pFade
)
{
	m_isStartFade = false;

	if (m_enableFadeStart)
	{
		pFade->SetFadeMode(FadeMode::F_IN);
		m_isNowFade = true;
		m_enableFadeStart = false;
	}

	if (!m_isNowFade) return;

	if (m_nowStage > m_maxStage)
	{
		if (pFade->IsFadeEnd())
		{
			m_isGameClear = true;
			m_isNowFade = false;
		}
	}
	else
	{
		if (pFade->IsFadeEnd())
		{
			SoundManager(pSystem); //���y��ς���
			pFade->SetFadeMode(FadeMode::F_OUT);
			pField->UpdateStage(pSystem, m_nowStage);
			m_isNowFade = false;
			m_isStartFade = true;
		}
	}
}

//--------------------------------------------------------------------------------
// �X�e�[�W�N���A����
//--------------------------------------------------------------------------------
void CGameProgress::StageClearDecision()
{
	if ( !m_isStageClear ) return;

	if ( m_nowStage == m_maxStage )
	{	//�X�e�[�W���ő�Ȃ�N���A����
		m_enableFadeStart = true;
		m_isStageClear = false;
		m_isGameOver = false;
		m_nowStage++;
	}
	else 
	{	//������
		m_isAddedEnemy = false;
		m_isStageClear = false;
		m_isGameOver = false;
		m_enableFadeStart = true;
		m_nowStage++;
	}
}

//--------------------------------------------------------------------------------
// �G�̒ǉ�
// ���@���FCSystem*		�V�X�e���N���X�̃|�C���^
// �@�@�@�@CAIManager*	�G�Ǘ��N���X�̃|�C���^
// �@�@�@�@CFadeAni*	�t�F�[�h�Ǘ��N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGameProgress::AddStartEnemy(
	CSystem* pSystem, 
	CAIManager* pAI, 
	CFadeAni* pFade
)
{
	if ( m_isAddedEnemy ) return;

	if (pFade->IsFadeEnd())
	{
		pAI->AddEnemyMultiple(pSystem, m_addStartEnemyNum);
		m_isAddedEnemy = true;
	}
}

//--------------------------------------------------------------------------------
// �{�X�̏o��
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
// �@�@�@�@CAIManager*	�G�Ǘ��N���X�̃|�C���^
// �@�@�@�@CFadeAni*	�t�F�[�h�Ǘ��N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGameProgress::AddSpawnBoss(
	CSystem* pSystem, 
	CAIManager* pAI, 
	CFadeAni* pFade
)
{
	if ( m_isAddedBoss ) return;

	if (pFade->IsFadeEnd())
	{
		pAI->SpawnBoss(pSystem);
		m_isAddedBoss = true;
	}
}

//--------------------------------------------------------------------------------
// �G�����N���A����
// ���@���FCAIManager*	�G�Ǘ��N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGameProgress::EnemyExpeditionClearCriteria(CAIManager* pAI)
{
	auto enemyList = pAI->GetEnemyList();

	if ( !m_isAddedEnemy ) return;

	//�G�����Ȃ��Ȃ���
	if (enemyList.size() == 0)
	{
		m_isStageClear = true;
	}
}

//--------------------------------------------------------------------------------
// �{�X�����N���A����
// ���@���FCAIManager*	�G�Ǘ��N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGameProgress::BossExpeditionClearCriteria(CAIManager* pAI)
{
	auto boss = pAI->GetBoss();

	if ( !m_isAddedBoss ) return;
	
	//�{�X�����S
	if ( !boss->IsAlive() )
	{
		m_isStageClear = true;
	}
}

//--------------------------------------------------------------------------------
// �v���C���[�̎��S��
// ���@���FCPlayer*			�v���C���[�N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGameProgress::PlayerDeathStatus(CPlayer * pPlayer, CFadeAni* pFade)
{
	if ( !pPlayer->IsAlive() && !m_isPlayerDeath )
	{
		m_isPlayerDeath = true;

		pFade->SetFadeMode(FadeMode::F_IN);
	}

	if ( m_isPlayerDeath && pFade->IsFadeEnd() )
	{
		m_isGameOver = true;
	}
}

//--------------------------------------------------------------------------------
// BGM�Ǘ�
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGameProgress::SoundManager(CSystem* pSystem)
{
	switch (m_nowStage)
	{
	case ST_FIELD_1: StageSoundSelecter( pSystem, BGM_GAME_FIELD1 ); break;
	case ST_FIELD_2: StageSoundSelecter( pSystem, BGM_GAME_FIELD2 ); break;
	case ST_FIELD_3: StageSoundSelecter( pSystem, BGM_GAME_FIELD3 ); break;
	}
}

//--------------------------------------------------------------------------------
// �X�e�[�W�ɂ��Bgm�I��
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
// �@�@�@�@const int �ǂݍ���Bgm�ԍ�
//--------------------------------------------------------------------------------
void CGameProgress::StageSoundSelecter(CSystem* pSystem, const int index)
{
	//���݂�Bgm���~�߂�
	pSystem->StopBgm(m_nowPlayBgm);

	//���̃Z�b�g����Bgm�𗬂�
	pSystem->PlayBgm(index);

	//���݂�Bgm��ύX
	m_nowPlayBgm = index;
}

//--------------------------------------------------------------------------------
// �p�����[�^�̎擾
// ���@���Fconst string �t�@�C����
//--------------------------------------------------------------------------------
void CGameProgress::LoadParams(const string strFileName)
{
	string dir = "Resource/Data/Stage/" + strFileName + ".txt";
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

	m_maxStage			= atoi(strAry[0].c_str());
	m_addStartEnemyNum	= atoi(strAry[1].c_str());

	ifs.close();
}

//--------------------------------------------------------------------------------
// �p�����[�^�̕ۑ�
// ���@���Fconst string �t�@�C����
//--------------------------------------------------------------------------------
void CGameProgress::InitParams(const string strFileName)
{
	string dir = "Resource/Data/Stage/" + strFileName + ".txt";
	ofstream ofs;
	ofs.open(dir);

	ofs << "MaxStage: Default(3)" << endl;
	ofs << "3" << endl;
	ofs << "AddEnemy: Default(6)" << endl;
	ofs << "6";

	ofs.close();
}

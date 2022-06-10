//********************************************************************************
//
// GameProgress.hpp
//
// ゲーム内の進行度
//
// Date: 2019/07/30 更新
//
//********************************************************************************

//================================================================================
// インクルード
//================================================================================
#include "GameProgress.hpp"

using namespace FileConst;

//================================================================================
// クラス定義
//================================================================================

//--------------------------------------------------------------------------------
// コンストラクタ
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
// デストラクタ
//--------------------------------------------------------------------------------
CGameProgress::~CGameProgress()
{
}

//--------------------------------------------------------------------------------
// ファイル読み込み
// 引　数：CSystem*	システムクラスのポインタ
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
// 初期化
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CGameProgress::Initialize(CSystem * pSystem)
{
	pSystem->PlayBgm(BGM_GAME_FIELD1);
}

//--------------------------------------------------------------------------------
// 更新
// 引　数：CSystem*			システムクラスのポインタ
// 　　　　CFieldManager*	フィールドクラスのポインタ
// 　　　　CPlayer*			プレイヤークラスのポインタ
// 　　　　CAIManager*		敵管理クラスのポインタ
// 　　　　CFadeAni*		フェード管理クラスのポインタ
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
// フェード管理
// 引　数：CSystem*			システムクラスのポインタ
// 　　　　CFieldManager*	フィールド管理クラスのポインタ
// 　　　　CFadeAni*		フェード管理クラスのポインタ
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
			SoundManager(pSystem); //音楽を変える
			pFade->SetFadeMode(FadeMode::F_OUT);
			pField->UpdateStage(pSystem, m_nowStage);
			m_isNowFade = false;
			m_isStartFade = true;
		}
	}
}

//--------------------------------------------------------------------------------
// ステージクリア判定
//--------------------------------------------------------------------------------
void CGameProgress::StageClearDecision()
{
	if ( !m_isStageClear ) return;

	if ( m_nowStage == m_maxStage )
	{	//ステージが最大ならクリア判定
		m_enableFadeStart = true;
		m_isStageClear = false;
		m_isGameOver = false;
		m_nowStage++;
	}
	else 
	{	//初期化
		m_isAddedEnemy = false;
		m_isStageClear = false;
		m_isGameOver = false;
		m_enableFadeStart = true;
		m_nowStage++;
	}
}

//--------------------------------------------------------------------------------
// 敵の追加
// 引　数：CSystem*		システムクラスのポインタ
// 　　　　CAIManager*	敵管理クラスのポインタ
// 　　　　CFadeAni*	フェード管理クラスのポインタ
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
// ボスの出現
// 引　数：CSystem*	システムクラスのポインタ
// 　　　　CAIManager*	敵管理クラスのポインタ
// 　　　　CFadeAni*	フェード管理クラスのポインタ
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
// 敵討伐クリア条件
// 引　数：CAIManager*	敵管理クラスのポインタ
//--------------------------------------------------------------------------------
void CGameProgress::EnemyExpeditionClearCriteria(CAIManager* pAI)
{
	auto enemyList = pAI->GetEnemyList();

	if ( !m_isAddedEnemy ) return;

	//敵がいなくなった
	if (enemyList.size() == 0)
	{
		m_isStageClear = true;
	}
}

//--------------------------------------------------------------------------------
// ボス討伐クリア条件
// 引　数：CAIManager*	敵管理クラスのポインタ
//--------------------------------------------------------------------------------
void CGameProgress::BossExpeditionClearCriteria(CAIManager* pAI)
{
	auto boss = pAI->GetBoss();

	if ( !m_isAddedBoss ) return;
	
	//ボスが死亡
	if ( !boss->IsAlive() )
	{
		m_isStageClear = true;
	}
}

//--------------------------------------------------------------------------------
// プレイヤーの死亡状況
// 引　数：CPlayer*			プレイヤークラスのポインタ
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
// BGM管理
// 引　数：CSystem*	システムクラスのポインタ
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
// ステージによるBgm選択
// 引　数：CSystem*	システムクラスのポインタ
// 　　　　const int 読み込んだBgm番号
//--------------------------------------------------------------------------------
void CGameProgress::StageSoundSelecter(CSystem* pSystem, const int index)
{
	//現在のBgmを止める
	pSystem->StopBgm(m_nowPlayBgm);

	//次のセットしたBgmを流す
	pSystem->PlayBgm(index);

	//現在のBgmを変更
	m_nowPlayBgm = index;
}

//--------------------------------------------------------------------------------
// パラメータの取得
// 引　数：const string ファイル名
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
	while (getline(ifs, str)) //説明文
	{
		if (getline(ifs, str)) //数値
		{
			strAry.push_back(str);
		}
	}

	m_maxStage			= atoi(strAry[0].c_str());
	m_addStartEnemyNum	= atoi(strAry[1].c_str());

	ifs.close();
}

//--------------------------------------------------------------------------------
// パラメータの保存
// 引　数：const string ファイル名
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

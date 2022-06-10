//********************************************************************************
//
// ShowTimer.hpp
//
// タイマーの表示
//
// Date: 2019/07/30 更新
//
//********************************************************************************

//================================================================================
// インクルード
//================================================================================
#include "ShowTimer.hpp"

using namespace FileConst;

//================================================================================
// クラス定義
//================================================================================

//--------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------
CShowTimer::CShowTimer()
	: m_pRanking(new CRanking)
	, m_isStart(false)
	, m_isTitle(false)
{
}

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
CShowTimer::~CShowTimer()
{
	for (auto w : m_pSprite)
		SafeDelete(w);

	for (auto w : m_pRender)
		SafeDelete(w);

	SafeDelete(m_pRanking);
}

//--------------------------------------------------------------------------------
// ファイル読み込み
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CShowTimer::LoadFile(CSystem * pSystem)
{
	pSystem->LoadTexture(TEXTURE_NUMBER,		"Resource/Textures/Share/Number.png");
	pSystem->LoadTexture(TEXTURE_TIME_DOT,		"Resource/Textures/Share/Dot.png");
	pSystem->LoadTexture(TEXTURE_RESULT_BACK,	"Resource/Textures/Result/ResultBack.png");
	pSystem->LoadTexture(TEXTURE_RESULT_LOGO,	"Resource/Textures/Result/Result.png");
	pSystem->LoadTexture(TEXTURE_HALF_LINE,		"Resource/Textures/Result/Hr.png");
	pSystem->LoadTexture(TEXTURE_RESULT_TIME,	"Resource/Textures/Result/Time.png");
	pSystem->LoadTexture(TEXTURE_RESULT_RANK,	"Resource/Textures/Result/Ranking.png");
	pSystem->LoadTexture(TEXTURE_RESULT_NUM,	"Resource/Textures/Result/RankNumber.png");

	if (pSystem->IsControllerConnect())
	{
		pSystem->LoadTexture(TEXTURE_RESULT_KEY, "Resource/Textures/Guide/Result_Pad.png");
	}
	else
	{
		pSystem->LoadTexture(TEXTURE_RESULT_KEY, "Resource/Textures/Guide/Result_Key.png");
	}

	pSystem->AddBgm(BGM_RESULT,	"Resource/Sounds/Bgm/bgm_result.wav");
	pSystem->AddSe(SE_SELECT,	"Resource/Sounds/Se/se_next.wav");

	m_pRanking->LoadFile(pSystem);
}

//--------------------------------------------------------------------------------
// 初期化
// 引　数：CSystem*		システムクラスのポインタ
// 　　　　const double	クリアタイム
//--------------------------------------------------------------------------------
void CShowTimer::Initialize(CSystem * pSystem, const double clearTime)
{
	pSystem->PlayBgm(BGM_RESULT);
	m_pRanking->Initialize(pSystem);
	InitializeResultTexture(pSystem);
	InitializeTimeRender(pSystem);
	InitializeMyTime(pSystem, clearTime);
	InitializeRankTime(pSystem);
}

//--------------------------------------------------------------------------------
// 更新
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CShowTimer::Update(CSystem * pSystem, CFadeAni* pFade)
{
	if (pSystem->IsKeyPressed(DIK_RETURN) || pSystem->IsPadPressed(KeyConfig::CIRCLE))
	{
		pFade->SetFadeMode(FadeMode::F_IN);
		pSystem->PlaySe(SE_SELECT);
		m_isStart = true;
	}

	if (pFade->IsFadeEnd() && m_isStart)
	{
		m_isTitle = true;
	}
}

//--------------------------------------------------------------------------------
// 描画
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CShowTimer::Draw(CSystem * pSystem)
{
	for (int i = 0; i < ST_MAX; i++)
	{
		m_pSprite[i]->Draw(pSystem);
	}

	for (int i = 0; i < TR_MAX; i++)
	{
		m_pRender[i]->Draw(pSystem);
	}
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// リザルトの初期化
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CShowTimer::InitializeResultTexture(CSystem* pSystem)
{
	m_halfWidth = pSystem->GetWindowSettings().Width / 2.0f;
	m_halfHeight = pSystem->GetWindowSettings().Height / 2.0f;

	m_pSprite[ST_RESULT_BACK]	= new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_RESULT_BACK));
	m_pSprite[ST_RESILT_LOGO]	= new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_RESULT_LOGO));
	m_pSprite[ST_HALF_LINE]		= new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_HALF_LINE));
	m_pSprite[ST_RESULT_TIME]	= new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_RESULT_TIME));
	m_pSprite[ST_RESULT_RANK]	= new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_RESULT_RANK));
	m_pSprite[ST_RESULT_NUM]	= new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_RESULT_NUM));
	m_pSprite[ST_RESULT_GUIDE]	= new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_RESULT_KEY));

	m_pSprite[ST_RESULT_BACK]->SetPosition(Vector2(m_halfWidth - 377.0f, m_halfHeight - 295.5f));
	m_pSprite[ST_RESILT_LOGO]->SetPosition(Vector2(m_halfWidth - 141.0f, m_halfHeight - 250.0f));
	m_pSprite[ST_HALF_LINE]->SetPosition(Vector2(m_halfWidth - 298.0f, m_halfHeight - 180.0f));
	m_pSprite[ST_RESULT_TIME]->SetPosition(Vector2(m_halfWidth - 298.0f, m_halfHeight - 160.0f));
	m_pSprite[ST_RESULT_RANK]->SetPosition(Vector2(m_halfWidth + 30.0f, m_halfHeight - 160.0f));
	m_pSprite[ST_RESULT_NUM]->SetPosition(Vector2(m_halfWidth - 30.0f, m_halfHeight - 95.0f));
	m_pSprite[ST_RESULT_GUIDE]->SetPosition(Vector2(5.0f, 650.0f));
}

//--------------------------------------------------------------------------------
// ランキング表示用クラスの初期化
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CShowTimer::InitializeTimeRender(CSystem* pSystem)
{
	for (int i = 0; i < TR_MAX; i++)
	{
		m_pRender[i] = new CRankingTimeRender();
	}
}

//--------------------------------------------------------------------------------
// 自己タイム表示の初期化
// 引　数：CSystem*		システムクラスのポインタ
// 　　　　const double	クリアタイム
//--------------------------------------------------------------------------------
void CShowTimer::InitializeMyTime(CSystem* pSystem, const double clearTime)
{
	m_pRender[TR_MY_TIME]->Initialize(pSystem);
	m_pRender[TR_MY_TIME]->SetTime((int)clearTime);
	m_pRender[TR_MY_TIME]->SetPos(Vector2(m_halfWidth - 195.0f, m_halfHeight - 100.0f));
	m_pRanking->SetTime((int)(clearTime / 1000));
}

//--------------------------------------------------------------------------------
// ランキングタイム表示の初期化
// 引　数：CSystem*		システムクラスのポインタ
//--------------------------------------------------------------------------------
void CShowTimer::InitializeRankTime(CSystem* pSystem)
{
	for (int i = TR_RANK1; i < TR_MAX; ++i)
	{	//ランキングタイム
		m_pRender[i]->Initialize(pSystem);
		m_pRender[i]->SetRankTime(m_pRanking->GetRankTimer(i - 1));
		m_pRender[i]->SetPos(Vector2(m_halfWidth + 170.0f, m_halfHeight - 100.0f + ( (i - 1 ) * 70 )));
	}
}

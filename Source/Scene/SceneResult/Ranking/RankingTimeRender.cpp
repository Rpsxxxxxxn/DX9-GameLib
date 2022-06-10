//********************************************************************************
//
// RankingTimeRender.cpp
//
// ランキング処理
//
// Date: 2019/10/10 更新
//
//********************************************************************************

//================================================================================
// インクルード
//================================================================================
#include "RankingTimeRender.hpp"

using namespace FileConst;

//================================================================================
// クラス定義
//================================================================================

//--------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------
CRankingTimeRender::CRankingTimeRender()
	: m_vecPos(Vector2(0.0f, 0.0f))
	, m_vecSize(Vector2(80.0f, 80.0f))
	, m_time(0)
{
}

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
CRankingTimeRender::~CRankingTimeRender()
{
	for (auto w : m_pSprite)
	{
		SafeDelete(w);
	}
}

//--------------------------------------------------------------------------------
// 初期化
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CRankingTimeRender::Initialize(CSystem * pSystem)
{
	m_pSprite[ST_MIN1] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_NUMBER));
	m_pSprite[ST_MIN2] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_NUMBER));
	m_pSprite[ST_DOT] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_TIME_DOT));
	m_pSprite[ST_SEC1] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_NUMBER));
	m_pSprite[ST_SEC2] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_NUMBER));

	for (auto i = 0; i < ST_MAX; i++)
	{
		m_pSprite[i]->SetSize((UINT)m_vecSize.x, (UINT)m_vecSize.y);
	}
}

//--------------------------------------------------------------------------------
// 描画
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CRankingTimeRender::Draw(CSystem * pSystem)
{
	Update(pSystem);

	for (auto i = 0; i < ST_MAX; i++)
	{
		m_pSprite[i]->Draw(pSystem);
	}
}

//--------------------------------------------------------------------------------
// 時間の設定
// 引　数：const int 計測した時間
//--------------------------------------------------------------------------------
void CRankingTimeRender::SetTime(const int value)
{
	m_time = value / 1000;
}

//--------------------------------------------------------------------------------
// 時間の設定
// 引　数：const int 計測した時間
// 備　考：SetTimeと違い、割らないタイプ
//--------------------------------------------------------------------------------
void CRankingTimeRender::SetRankTime(const int value)
{
	m_time = value;
}

//--------------------------------------------------------------------------------
// 新しい位置へ補間する
// 引　数：const Vector2 2次元ベクトル
//--------------------------------------------------------------------------------
void CRankingTimeRender::SetNewPos(const Vector2 value)
{
	for (auto i = 0; i < ST_MAX; i++)
	{
		m_pSprite[i]->SetNewPosition(Vector2(value.x - i * (m_vecSize.x / 3), value.y));
	}
}

//--------------------------------------------------------------------------------
// 新しい位置へ指定する
// 引　数：const Vector2 2次元ベクトル
//--------------------------------------------------------------------------------
void CRankingTimeRender::SetPos(const Vector2 value)
{
	for (auto i = 0; i < ST_MAX; i++)
	{
		m_pSprite[i]->SetPosition(Vector2(value.x - i * (m_vecSize.x / 3), value.y));
	}
}

//--------------------------------------------------------------------------------
// 透明度の指定する
// 引　数：const Vector2 2次元ベクトル
//--------------------------------------------------------------------------------
void CRankingTimeRender::SetNewAlpha(const BYTE alpha)
{
	for (auto i = 0; i < ST_MAX; i++)
	{
		m_pSprite[i]->SetNewAlpha(alpha);
	}
}

//--------------------------------------------------------------------------------
// 透明度の指定する
// 引　数：const Vector2 2次元ベクトル
//--------------------------------------------------------------------------------
void CRankingTimeRender::SetAlpha(const BYTE alpha)
{
	for (auto i = 0; i < ST_MAX; i++)
	{
		m_pSprite[i]->SetAlpha(alpha);
	}
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// 更新
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CRankingTimeRender::Update(CSystem * pSystem)
{
	int timeMin = m_time / 60;
	int timeSec = m_time % 60;

	m_pSprite[ST_MIN1]->SetNumber(152, 1515, timeMin % 10); timeMin /= 10;
	m_pSprite[ST_MIN2]->SetNumber(152, 1515, timeMin % 10);
	m_pSprite[ST_SEC1]->SetNumber(152, 1515, timeSec % 10); timeSec /= 10;
	m_pSprite[ST_SEC2]->SetNumber(152, 1515, timeSec % 10);
}

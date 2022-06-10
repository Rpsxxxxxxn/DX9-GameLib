//********************************************************************************
//
// FadeAni.cpp
//
// フェードアニメーションクラス
//
// Date: 2019/07/30 更新
//
//********************************************************************************

//================================================================================
// インクルード
//================================================================================
#include "FadeAni.hpp"

using namespace FileConst;

const DWORD MAX_ALPHA = 255;
const DWORD HALF_ALPHA = MAX_ALPHA / 2;
const float VOLUME_LERP_SPEED = 0.05f;

//================================================================================
// クラス定義
//================================================================================

//--------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------
CFadeAni::CFadeAni()
	: m_dwAlpha(MAX_ALPHA)
	, m_dwSpeed(3)
	, m_fadeVolume(0.0f)
	, m_soundVolume(0.0f)
	, m_isFadeEnd(false)
	, m_fadeMode(F_NONE)
{
}

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
CFadeAni::~CFadeAni()
{
}

//--------------------------------------------------------------------------------
// ファイル読み込み
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CFadeAni::LoadFile(CSystem * pSystem)
{
	pSystem->LoadTexture(TEXTURE_FADE, "Resource/Textures/Fade/Fade.png");
}

//--------------------------------------------------------------------------------
// 初期化
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CFadeAni::Initialize(CSystem * pSystem)
{
	auto window = pSystem->GetWindowSettings();
	auto texData = pSystem->GetTextureData(TEXTURE_FADE);

	SetRect(&m_rSour, 0, 0, texData->uiWidth, texData->uiHeight);
	SetRect(&m_rDest, 0, 0, window.Width, window.Height);
}

//--------------------------------------------------------------------------------
// 描画
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CFadeAni::Draw(CSystem * pSystem)
{
	Update(pSystem);

	pSystem->SpriteBegin();
	pSystem->DrawSprite(&m_rSour, pSystem->GetTexture(TEXTURE_FADE), &m_rDest, m_dwAlpha);
	pSystem->SpriteClose();
}

//--------------------------------------------------------------------------------
// フェードの速度を設定
// 引　数：const DWORD	スピード
//--------------------------------------------------------------------------------
void CFadeAni::SetFadeSpeed(const DWORD dwSpeed)
{
	m_dwSpeed = dwSpeed;
}

//--------------------------------------------------------------------------------
// フェードの仕方設定
// 引　数：Fade	フェードモード
//--------------------------------------------------------------------------------
void CFadeAni::SetFadeMode(FadeMode mode)
{
	m_fadeMode = mode;
}

//--------------------------------------------------------------------------------
// フェードが終了したことを知らせる
// 戻り値：const bool フェード終了
//--------------------------------------------------------------------------------
bool CFadeAni::IsFadeEnd() const
{
	return m_isFadeEnd;
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// フェード開始
// 備　考：だんだん暗くなる
//--------------------------------------------------------------------------------
void CFadeAni::FadeInStart(CSystem * pSystem)
{
	if (m_dwAlpha < MAX_ALPHA)
	{
		m_fadeVolume = 0.0f;
		m_dwAlpha += m_dwSpeed;
	}
	else {
		m_fadeMode = F_NONE;
		m_isFadeEnd = true;
	}
}

//--------------------------------------------------------------------------------
// フェード開始
// 備　考：だんだん明るくなる
//--------------------------------------------------------------------------------
void CFadeAni::FadeOutStart(CSystem * pSystem)
{
	if (m_dwAlpha > 0)
	{
		m_fadeVolume = 1.0f;
		m_dwAlpha -= m_dwSpeed;
	}
	else {
		m_fadeMode = F_NONE;
		m_isFadeEnd = true;
	}
}

//--------------------------------------------------------------------------------
// フェード開始
// 備　考：だんだん暗くなる(半分)
//--------------------------------------------------------------------------------
void CFadeAni::HalfFadeInStart(CSystem * pSystem)
{
	if (m_dwAlpha < HALF_ALPHA)
	{
		m_fadeVolume = 0.5f;
		m_dwAlpha += m_dwSpeed;
	}
	else {
		m_fadeMode = F_NONE;
		m_isFadeEnd = true;
	}
}

//--------------------------------------------------------------------------------
// フェード開始
// 備　考：だんだん明るくなる(半分)
//--------------------------------------------------------------------------------
void CFadeAni::HalfFadeOutStart(CSystem * pSystem)
{
	if (m_dwAlpha > HALF_ALPHA)
	{
		m_fadeVolume = 1.0f;
		m_dwAlpha -= m_dwSpeed;
	}
	else {
		m_fadeMode = F_NONE;
		m_isFadeEnd = true;
	}
}

//--------------------------------------------------------------------------------
// サウンド
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CFadeAni::SoundManager(CSystem* pSystem)
{
	m_soundVolume = VectorCalculate::Lerp(
		m_soundVolume,
		m_fadeVolume,
		VOLUME_LERP_SPEED);

	pSystem->SetVolume(m_soundVolume);
}

//--------------------------------------------------------------------------------
// 更新
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CFadeAni::Update(CSystem * pSystem)
{
	m_isFadeEnd = false;

	switch (m_fadeMode)
	{
	case FadeMode::F_IN: FadeInStart(pSystem); break;
	case FadeMode::F_OUT: FadeOutStart(pSystem); break;
	case FadeMode::F_HALF_IN: HalfFadeInStart(pSystem);	break;
	case FadeMode::F_HALF_OUT: HalfFadeOutStart(pSystem); break;
	}

	SoundManager(pSystem);
}
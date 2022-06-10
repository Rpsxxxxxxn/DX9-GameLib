//********************************************************************************
//
// Pause.hpp
//
// ゲーム内の中断処理
//
// Date: 2019/07/30 更新
//
//********************************************************************************

//================================================================================
// インクルード
//================================================================================
#include "Pause.hpp"

using namespace FileConst;

//================================================================================
// クラス定義
//================================================================================

//--------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------
CPause::CPause()
	: m_angle(0.0f)
	, m_isPauseKey(false)
	, m_isUpKey(false)
	, m_isDownKey(false)
	, m_isEnter(false)
	, m_isNowSleep(false)
	, m_isNowPause(false)
	, m_isChangePause(false)
	, m_isTitle(false)
	, m_enableFadeStart(false)
	, m_nowSelect(SP_NONE)
	, m_determination(SP_NONE)
	, m_isJoyStickUpSync(false)
	, m_isJoyStickDownSync(false)
{
}

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
CPause::~CPause()
{
	for (auto i = 0; i < ST_MAX; i++)
	{
		SafeDelete(m_pSprite[i]);
	}
}

//--------------------------------------------------------------------------------
// ファイル読み込み
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CPause::LoadFile(CSystem * pSystem)
{
	pSystem->LoadTexture(TEXTURE_CURSOR,		"Resource/Textures/Title/Cursor.png");
	pSystem->LoadTexture(TEXTURE_PAUSE_LOGO,	"Resource/Textures/Pause/Pause.png");
	pSystem->LoadTexture(TEXTURE_PAUSE_TITLE,	"Resource/Textures/Pause/Title.png");
	pSystem->LoadTexture(TEXTURE_PAUSE_BACK,	"Resource/Textures/Pause/Back.png");
	pSystem->AddSe(SE_SELECT,					"Resource/Sounds/Se/se_select.wav");
	pSystem->AddSe(SE_ENTER,					"Resource/Sounds/Se/se_next.wav");
}

//--------------------------------------------------------------------------------
// 初期化
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CPause::Initialize(CSystem * pSystem)
{
	m_halfWidth = pSystem->GetWindowSettings().Width / 2.0f;
	m_halfHeight = pSystem->GetWindowSettings().Height / 2.0f;

	m_pSprite[ST_PAUSE] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_PAUSE_LOGO));
	m_pSprite[ST_BACK] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_PAUSE_BACK));
	m_pSprite[ST_TITLE] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_PAUSE_TITLE));
	m_pSprite[ST_L_CURSOR] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_CURSOR));
	m_pSprite[ST_R_CURSOR] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_CURSOR));

	m_pSprite[ST_PAUSE]->SetPosition(Vector2(0.0f, m_halfHeight - 200.0f));
	m_pSprite[ST_PAUSE]->SetAlpha(0);
	m_pSprite[ST_BACK]->SetPosition(Vector2(0.0f, m_halfHeight + 55.0f));
	m_pSprite[ST_BACK]->SetAlpha(0);
	m_pSprite[ST_TITLE]->SetPosition(Vector2(0.0f, m_halfHeight + 185.0f));
	m_pSprite[ST_TITLE]->SetAlpha(0);
	m_pSprite[ST_L_CURSOR]->SetPosition(Vector2(m_halfWidth, m_halfHeight + 85.0f));
	m_pSprite[ST_R_CURSOR]->SetPosition(Vector2(m_halfWidth, m_halfHeight + 85.0f));
	m_pSprite[ST_L_CURSOR]->SetAlpha(0);
	m_pSprite[ST_R_CURSOR]->SetAlpha(0);
}

//--------------------------------------------------------------------------------
// 更新
// 引　数：CSystem*			システムクラスのポインタ
// 　　　　CPlayer*			プレイヤークラスのポインタ
// 　　　　CEnemyManager*	エネミークラスのポインタ
// 　　　　CGameTimer*		ゲームタイマークラスのポインタ
// 　　　　CFadeAni*		フェード管理クラスのポインタ
//--------------------------------------------------------------------------------
void CPause::Update(CSystem * pSystem, CPlayer* pPlayer, CAIManager* pAI, CGameTimer* pTimer, CFadeAni* pFade)
{
	KeyInput( pSystem );

	Controller( pSystem );

	CursorRotate();

	FadeManager(pFade);

	UpdateSleepObject( pPlayer, pAI, pTimer );
}

//--------------------------------------------------------------------------------
// 描画
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CPause::Draw(CSystem * pSystem)
{
	for (auto i = 0; i < ST_MAX; i++)
	{
		m_pSprite[i]->Draw( pSystem );
	}
}

//--------------------------------------------------------------------------------
// タイトルへ戻る
// 戻り値：const bool	選択後
//--------------------------------------------------------------------------------
bool CPause::IsTitle() const
{
	return m_isTitle;
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// 上一度のみ
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
bool CPause::JoyStickUpSync(CSystem * pSystem)
{
	if (pSystem->GetJoyStickState(KeyConfig::LEFTVER) < -0.1f)
	{
		if (!m_isJoyStickUpSync)
		{
			m_isJoyStickUpSync = true;
			return true;
		}
	}
	else {
		m_isJoyStickUpSync = false;
	}

	return false;
}

//--------------------------------------------------------------------------------
// 下一度のみ
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
bool CPause::JoyStickDownSync(CSystem * pSystem)
{
	if (pSystem->GetJoyStickState(KeyConfig::LEFTVER) > 0.1f)
	{
		if (!m_isJoyStickDownSync)
		{
			m_isJoyStickDownSync = true;
			return true;
		}
	}
	else {
		m_isJoyStickDownSync = false;
	}

	return false;
}

//--------------------------------------------------------------------------------
// キー入力
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CPause::KeyInput(CSystem * pSystem)
{
	m_isPauseKey = pSystem->IsKeyPressed(DIK_ESCAPE) || pSystem->IsPadPressed(KeyConfig::OPTION);
	m_isUpKey = pSystem->IsKeyPressed(DIK_UP) || JoyStickUpSync(pSystem);
	m_isDownKey = pSystem->IsKeyPressed(DIK_DOWN) || JoyStickDownSync(pSystem);
	m_isEnter = pSystem->IsKeyPressed(DIK_RETURN) || pSystem->IsPadPressed(KeyConfig::CIRCLE);
}

//--------------------------------------------------------------------------------
// ポーズコントローラー
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CPause::Controller(CSystem * pSystem)
{
	if ( m_isPauseKey )
	{
		m_isNowSleep = !m_isNowSleep; 
		m_isChangePause = true;
		m_nowSelect = SP_BACK;
		m_determination = SP_NONE;
	}

	if ( !m_isNowSleep ) return;

	if ( m_isUpKey )
	{
		pSystem->PlaySe(SE_SELECT);
		m_nowSelect = SP_BACK;
	}

	if ( m_isDownKey )
	{
		pSystem->PlaySe(SE_SELECT);
		m_nowSelect = SP_TITLE;
	}

	if ( m_isEnter )
	{
		pSystem->PlaySe(SE_ENTER);
		m_determination = m_nowSelect;
	}

	if ( m_isUpKey || m_isDownKey )
	{
		switch (m_nowSelect)
		{
		case CPause::SP_BACK: SetCursorPos(85.0f); break;
		case CPause::SP_TITLE: SetCursorPos(215.0f); break;
		}
	}

	if ( m_determination == SP_BACK )
	{
		m_isNowSleep = false;
		m_isChangePause = true;
	}
}

//--------------------------------------------------------------------------------
// フェード管理
//--------------------------------------------------------------------------------
void CPause::FadeManager(CFadeAni * pFade)
{
	if (m_isChangePause)
	{
		if (m_isNowSleep)
		{
			pFade->SetFadeMode( FadeMode::F_HALF_IN );
		}
		else {
			pFade->SetFadeMode( FadeMode::F_OUT );
		}

		ChangePause();
	}

	if (m_determination == SP_TITLE && !m_enableFadeStart)
	{
		pFade->SetFadeMode( FadeMode::F_IN );
		PauseTexture(false);
		CursorTexture(false);
		SelectTexture(false);
		m_enableFadeStart = true;
	}

	if ( m_enableFadeStart && pFade->IsFadeEnd() )
	{
		m_isTitle = true;
	}
}

//--------------------------------------------------------------------------------
// カーソルの位置
//--------------------------------------------------------------------------------
void CPause::SetCursorPos(const float height)
{
	m_nowCursorHeight = height;
	m_pSprite[ST_L_CURSOR]->SetNewPosition(Vector2(m_halfWidth - 300.0f, m_halfHeight + height));
	m_pSprite[ST_R_CURSOR]->SetNewPosition(Vector2(m_halfWidth + 250.0f, m_halfHeight + height));
}

//--------------------------------------------------------------------------------
// カーソルの回転
//--------------------------------------------------------------------------------
void CPause::CursorRotate()
{
	m_angle += 0.03f;

	m_pSprite[ST_L_CURSOR]->SetAngle(m_angle);
	m_pSprite[ST_R_CURSOR]->SetAngle(-m_angle);
}

//--------------------------------------------------------------------------------
// ポーズの変更
//--------------------------------------------------------------------------------
void CPause::ChangePause()
{
	if (!m_isChangePause) return;

	PauseTexture(m_isNowSleep);

	CursorTexture(m_isNowSleep);
	
	SelectTexture(m_isNowSleep);

	m_isChangePause = false;
}

//--------------------------------------------------------------------------------
// ポーズテクスチャ表示&非表示
//--------------------------------------------------------------------------------
void CPause::PauseTexture(const bool show)
{
	if (show)
	{
		m_pSprite[ST_PAUSE]->SetNewPosition(Vector2(m_halfWidth - 150.0f, m_halfHeight - 200.0f));
		m_pSprite[ST_PAUSE]->SetNewAlpha(255);
	}
	else 
	{
		m_pSprite[ST_PAUSE]->SetNewPosition(Vector2(0.0f, m_halfHeight - 200.0f));
		m_pSprite[ST_PAUSE]->SetNewAlpha(0);
	}
}

//--------------------------------------------------------------------------------
// カーソルテクスチャ表示&非表示
//--------------------------------------------------------------------------------
void CPause::CursorTexture(const bool show)
{
	if (show)
	{
		m_pSprite[ST_L_CURSOR]->SetNewPosition(Vector2(m_halfWidth - 300.0f, m_halfHeight + 85.0f));
		m_pSprite[ST_R_CURSOR]->SetNewPosition(Vector2(m_halfWidth + 250.0f, m_halfHeight + 85.0f));
		m_pSprite[ST_L_CURSOR]->SetNewAlpha(255);
		m_pSprite[ST_R_CURSOR]->SetNewAlpha(255);
	}
	else
	{
		m_pSprite[ST_L_CURSOR]->SetNewPosition(Vector2(m_halfWidth - 114.0f, m_halfHeight + m_nowCursorHeight));
		m_pSprite[ST_R_CURSOR]->SetNewPosition(Vector2(m_halfWidth - 114.0f, m_halfHeight + m_nowCursorHeight));
		m_pSprite[ST_L_CURSOR]->SetNewAlpha(0);
		m_pSprite[ST_R_CURSOR]->SetNewAlpha(0);
	}
}

//--------------------------------------------------------------------------------
// セレクトテクスチャ表示&非表示
//--------------------------------------------------------------------------------
void CPause::SelectTexture(const bool show)
{
	if (show)
	{
		m_pSprite[ST_BACK]->SetNewPosition(Vector2(m_halfWidth - 114.0f, m_halfHeight + 75.0f));
		m_pSprite[ST_TITLE]->SetNewPosition(Vector2(m_halfWidth - 114.0f, m_halfHeight + 205.0f));
		m_pSprite[ST_BACK]->SetNewAlpha(255);
		m_pSprite[ST_TITLE]->SetNewAlpha(255);
	}
	else
	{
		m_pSprite[ST_BACK]->SetNewPosition(Vector2(0.0f, m_halfHeight + 75.0f));
		m_pSprite[ST_TITLE]->SetNewPosition(Vector2(0.0f, m_halfHeight + 205.0f));
		m_pSprite[ST_BACK]->SetNewAlpha(0);
		m_pSprite[ST_TITLE]->SetNewAlpha(0);
	}
}

//--------------------------------------------------------------------------------
// オブジェクトの動きを止める処理
//--------------------------------------------------------------------------------
void CPause::UpdateSleepObject(CPlayer* pPlayer, CAIManager* pAI, CGameTimer* pTimer)
{
	pPlayer->SetSleep(m_isNowSleep);
	pTimer->SetSleep(m_isNowSleep);
	pAI->SetSleep(m_isNowSleep);
}

//********************************************************************************
//
// Pause.hpp
//
// ゲーム内の中断処理
//
// Date: 2019/07/30 更新
//
//********************************************************************************

#pragma once

//================================================================================
// インクルード
//================================================================================
#include "../../../GameManager/GameManager.h"
#include "../../../ShareObject/FadeAni/FadeAni.hpp"
#include "../../Timer/GameTimer.hpp"
#include "../../Player/Player.hpp"
#include "../../AI/AIManager.hpp"

//================================================================================
// クラス定義
//================================================================================
class CPause
{
public:

	//--------------------------------------------------------------------------------
	// コンストラクタ
	//--------------------------------------------------------------------------------
	CPause();

	//--------------------------------------------------------------------------------
	// デストラクタ
	//--------------------------------------------------------------------------------
	~CPause();

	//--------------------------------------------------------------------------------
	// ファイル読み込み
	// 引　数：CSystem*	システムクラスのポインタ
	//--------------------------------------------------------------------------------
	void LoadFile(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// 初期化
	// 引　数：CSystem*	システムクラスのポインタ
	//--------------------------------------------------------------------------------
	void Initialize(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// 更新
	// 引　数：CSystem*			システムクラスのポインタ
	// 　　　　CPlayer*			プレイヤークラスのポインタ
	// 　　　　CEnemyManager*	エネミークラスのポインタ
	// 　　　　CGameTimer*		ゲームタイマークラスのポインタ
	// 　　　　CFadeAni*		フェード管理クラスのポインタ
	//--------------------------------------------------------------------------------
	void Update(CSystem* pSystem, CPlayer* pPlayer, CAIManager* pAI, CGameTimer* pTimer, CFadeAni* pFade);

	//--------------------------------------------------------------------------------
	// 描画
	// 引　数：CSystem*	システムクラスのポインタ
	//--------------------------------------------------------------------------------
	void Draw(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// タイトルへ戻る
	// 戻り値：const bool	選択後
	//--------------------------------------------------------------------------------
	bool IsTitle() const;

private:

	//--------------------------------------------------------------------------------
	// 上一度のみ
	// 引　数：CSystem*	システムクラスのポインタ
	//--------------------------------------------------------------------------------
	bool JoyStickUpSync(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// 下一度のみ
	// 引　数：CSystem*	システムクラスのポインタ
	//--------------------------------------------------------------------------------
	bool JoyStickDownSync(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// キー入力
	// 引　数：CSystem*	システムクラスのポインタ
	//--------------------------------------------------------------------------------
	void KeyInput(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// ポーズコントローラー
	// 引　数：CSystem*	システムクラスのポインタ
	//--------------------------------------------------------------------------------
	void Controller(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// フェード管理
	//--------------------------------------------------------------------------------
	void FadeManager(CFadeAni* pFade);

	//--------------------------------------------------------------------------------
	// カーソルの位置
	//--------------------------------------------------------------------------------
	void SetCursorPos(const float height);

	//--------------------------------------------------------------------------------
	// カーソルの回転
	//--------------------------------------------------------------------------------
	void CursorRotate();

	//--------------------------------------------------------------------------------
	// ポーズの変更
	//--------------------------------------------------------------------------------
	void ChangePause();

	//--------------------------------------------------------------------------------
	// ポーズテクスチャ表示&非表示
	//--------------------------------------------------------------------------------
	void PauseTexture(const bool show);

	//--------------------------------------------------------------------------------
	// カーソルテクスチャ表示&非表示
	//--------------------------------------------------------------------------------
	void CursorTexture(const bool show);

	//--------------------------------------------------------------------------------
	// セレクトテクスチャ表示&非表示
	//--------------------------------------------------------------------------------
	void SelectTexture(const bool show);

	//--------------------------------------------------------------------------------
	// オブジェクトの動きを止める処理
	//--------------------------------------------------------------------------------
	void UpdateSleepObject(CPlayer* pPlayer, CAIManager* pAI, CGameTimer* pTimer);

	enum SelectPause
	{
		SP_BACK,
		SP_TITLE,
		SP_NONE,
	};

	enum SpriteTexture
	{
		ST_PAUSE,
		ST_BACK,
		ST_TITLE,
		ST_L_CURSOR,
		ST_R_CURSOR,
		ST_MAX,
	};

	CSpriteAnimation* m_pSprite[ST_MAX];

	SelectPause m_nowSelect;
	SelectPause m_determination;

	float m_angle;
	float m_halfWidth;
	float m_halfHeight;
	float m_nowCursorHeight;

	bool m_isPauseKey;
	bool m_isUpKey;
	bool m_isDownKey;
	bool m_isEnter;

	bool m_isNowSleep;
	bool m_isNowPause;
	bool m_isChangePause;
	bool m_isTitle;
	bool m_enableFadeStart;
	bool m_isJoyStickUpSync;
	bool m_isJoyStickDownSync;
};
//********************************************************************************
//
// CStartSelect.hpp
//
// タイトル画面のセレクト
//
// Date: 2019/07/30 更新
//
//********************************************************************************

#pragma once

//================================================================================
// インクルード
//================================================================================
#include "../../GameManager/GameManager.h"

//================================================================================
// クラス定義
//================================================================================
class CStartSelect
{
public:

	//--------------------------------------------------------------------------------
	// スタートセレクター
	//--------------------------------------------------------------------------------
	enum StartSelect
	{
		SS_START,
		SS_EXIT,
		SS_NONE,
	};

	//--------------------------------------------------------------------------------
	// コンストラクタ
	//--------------------------------------------------------------------------------
	CStartSelect();

	//--------------------------------------------------------------------------------
	// デストラクタ
	//--------------------------------------------------------------------------------
	~CStartSelect();

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
	// 引　数：CSystem*	システムクラスのポインタ
	//--------------------------------------------------------------------------------
	void Update(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// 描画
	// 引　数：CSystem*	システムクラスのポインタ
	//--------------------------------------------------------------------------------
	void Draw(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// 決定後の選択項目
	// 戻り値：const StartSelect	選択済み
	//--------------------------------------------------------------------------------
	StartSelect GetSelected() const;

	//--------------------------------------------------------------------------------
	// 表示情報のセット(表示)
	//--------------------------------------------------------------------------------
	void SetShowSelect();

	//--------------------------------------------------------------------------------
	// 表示情報のセット(隠す)
	//--------------------------------------------------------------------------------
	void SetHideSelect();

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
	// キーボード入力＆ジョイスティック入力処理
	// 引　数：CSystem*	システムクラスのポインタ
	//--------------------------------------------------------------------------------
	void KeyInput(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// 選択操作
	// 引　数：CSystem*	システムクラスのポインタ
	//--------------------------------------------------------------------------------
	void SelectController(CSystem* pSystem);

	//--------------------------------------------------------------------------------
	// 表示アニメーション
	//--------------------------------------------------------------------------------
	void ShowAnimation();

	//--------------------------------------------------------------------------------
	// 隠すアニメーション
	//--------------------------------------------------------------------------------
	void HideAnimation();

	//--------------------------------------------------------------------------------
	// カーソルの回転
	//--------------------------------------------------------------------------------
	void CursorRotate();

	//--------------------------------------------------------------------------------
	// カーソルの位置
	//--------------------------------------------------------------------------------
	void SetCursorPos(const float height);

	//--------------------------------------------------------------------------------
	// ロゴテクスチャ
	//--------------------------------------------------------------------------------
	void LogoTexture(const bool show);

	//--------------------------------------------------------------------------------
	// ソードテクスチャ
	//--------------------------------------------------------------------------------
	void SwordTexture(const bool show);

	//--------------------------------------------------------------------------------
	// 選択テクスチャ
	//--------------------------------------------------------------------------------
	void SelectTexture(const bool show);

	//--------------------------------------------------------------------------------
	// カーソルテクスチャ
	//--------------------------------------------------------------------------------
	void CursorTexture(const bool show);

	enum SpriteTexture
	{
		ST_SWORD,
		ST_MASTERLOGO,
		ST_SWORDLOGO,
		ST_R_CURSOR,
		ST_L_CURSOR,
		ST_EXIT,
		ST_START,
		ST_GUIDE,
		ST_MAX,
	};

	CSpriteAnimation* m_pSprite[ST_MAX];
	StartSelect m_nowSelect;
	StartSelect m_determination;

	Vector2 m_vecCurPos;

	int m_nowCount;

	float m_angle;
	float m_halfWidth;
	float m_halfHeight;
	float m_cursorHeight;

	bool m_enableShowAni;
	bool m_enableHideAni;
	bool m_isVisible;
	bool m_isDownKey;
	bool m_isUpKey;

	bool m_isUp;
	bool m_isDown;
	bool m_isEnter;
	bool m_isJoyStickUpSync;
	bool m_isJoyStickDownSync;
};
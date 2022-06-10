//*************************************************************
//
// SpriteValue.cpp
//
// 数値の描画
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "SpriteValue.h"

//-------------------------------------------------------------
// コンストラクタ
// 引　数：std::string	ディレクトリ
//-------------------------------------------------------------
CSpriteValue::CSpriteValue(TEXTURE_DATA * pTextureData)
	: m_pTexData(pTextureData)
	, m_nowDigit(0)
	, m_oldDigit(0)
	, m_alpha(255)
	, m_vecPos(Vector2(0.0f, 0.0f))
	, m_vecSize(Vector2(0.0f, 0.0f))
	, m_endTime(0.0)
	, m_elapsedTime(0.0)
	, m_angle(0.0f)
	, m_isDestroy(false)
{
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CSpriteValue::~CSpriteValue()
{
	for (auto val : m_vecSprite)
	{
		SafeDelete(val);
	}
	m_vecSprite.clear();
}

//-------------------------------------------------------------
// 描画
// 引　数：Engine*	エンジンクラスのポインタ
//-------------------------------------------------------------
void CSpriteValue::Draw(CSystem * pSystem)
{
	m_elapsedTime += pSystem->GetDeltaTime();

	if (m_elapsedTime > m_endTime && m_endTime != 0)
	{
		m_isDestroy = true;
	}

	//現在の桁数と過去の桁数を比べて大きければ増やす
	if (m_nowDigit > m_oldDigit)
	{
		for (int i = m_oldDigit; i < m_nowDigit; i++)
		{
			CSpriteAnimation* pWork = new CSpriteAnimation(m_pTexData);
			pWork->SetPosition(Vector2(m_vecPos.x - i * (m_vecSize.x / 3), m_vecPos.y));
			pWork->SetSize((UINT)m_vecSize.x, (UINT)m_vecSize.y);
			m_vecSprite.push_back(pWork);
		}
	}
	//過去の桁数のほうが大きければ削除する
	else if (m_nowDigit < m_oldDigit)
	{
		for (auto it = m_vecSprite.begin() + m_nowDigit;
			it != m_vecSprite.begin() + m_oldDigit;
			it++)
		{
			SafeDelete(*it);
		}

		m_vecSprite.erase(
			m_vecSprite.begin() + m_nowDigit,
			m_vecSprite.begin() + m_oldDigit);
	}

	//通常通り更新
	for (size_t i = 0; i < m_vecSprite.size(); i++)
	{
		m_vecSprite[i]->SetPosition(Vector2(m_vecPos.x - i * (m_vecSize.x / 3), m_vecPos.y));
		m_vecSprite[i]->SetNumber(m_pTexData->uiWidth, m_pTexData->uiHeight, m_num[i]);
		m_vecSprite[i]->Draw(pSystem);
	}

	m_oldDigit = m_nowDigit;
}

//-------------------------------------------------------------
// 描画する数値の設定
// 引　数：const int 数値
//-------------------------------------------------------------
void CSpriteValue::SetValue(const int value)
{
	m_nowDigit = CalcDigit(value);
}

//-------------------------------------------------------------
// 位置の設定
// 引　数：const Vector2	3次元ベクトルの位置
//-------------------------------------------------------------
void CSpriteValue::SetPosition(const Vector2 vecPos)
{
	m_vecPos = vecPos;
}

//-------------------------------------------------------------
// 透明度の設定
// 引　数：const int	透明度(０　～　２５５)
//-------------------------------------------------------------
void CSpriteValue::SetAlpha(const int alpha)
{
	m_alpha = alpha;
}

//-------------------------------------------------------------
// 回転角度の設定
// 引　数：const float	ラジアン角
//-------------------------------------------------------------
void CSpriteValue::SetAngle(const float angle)
{
	m_angle = angle;
}

//-------------------------------------------------------------
// 大きさの設定
// 引　数：const UINT	横幅のUINT
// 　　　　const UINT	縦幅のUINT
//-------------------------------------------------------------
void CSpriteValue::SetSize(const UINT width, const UINT height)
{
	m_vecSize.x = static_cast<float>(width);
	m_vecSize.y = static_cast<float>(height);
}

//-------------------------------------------------------------
// 位置の設定
// 引　数：const Vector2	3次元ベクトルの位置
// 備　考：アニメーション使用時に利用
//-------------------------------------------------------------
void CSpriteValue::SetNewPosition(const Vector2 vecPos)
{
	m_vecPos = vecPos;

	for (size_t i = 0; i < m_vecSprite.size(); i++)
	{
		m_vecSprite[i]->SetNewPosition(
			Vector2(m_vecPos.x - i * (m_vecSize.x / 3), m_vecPos.y));
	}
}

//-------------------------------------------------------------
// 透明度の設定
// 引　数：const int	透明度(０　～　２５５)
// 備　考：アニメーション使用時に利用
//-------------------------------------------------------------
void CSpriteValue::SetNewAlpha(const int alpha)
{
	m_alpha = alpha;

	for (size_t i = 0; i < m_vecSprite.size(); i++)
	{
		m_vecSprite[i]->SetNewAlpha(m_alpha);
	}
}

//-------------------------------------------------------------
// 回転角度の設定
// 引　数：const float	ラジアン角
// 備　考：アニメーション使用時に利用
//-------------------------------------------------------------
void CSpriteValue::SetNewAngle(const float angle)
{
	m_angle = angle;
}

//-------------------------------------------------------------
// 大きさの設定
// 引　数：const UINT	横幅のUINT
// 　　　　const UINT	縦幅のUINT
// 備　考：アニメーション使用時に利用
//-------------------------------------------------------------
void CSpriteValue::SetNewSize(const UINT width, const UINT height)
{
	m_vecSize.x = static_cast<float>(width);
	m_vecSize.y = static_cast<float>(height);
}

//-------------------------------------------------------------
// アニメーション終了時間
// 引　数：const DWORD	終了時間(ms)
// 備　考：msなので [1秒] -> [1000ms]
//-------------------------------------------------------------
void CSpriteValue::SetEndTime(const double time)
{
	m_endTime = time;

	for (size_t i = 0; i < m_vecSprite.size(); i++)
	{
		m_vecSprite[i]->SetEndTime(time);
	}
}

//-------------------------------------------------------------
// 生存時間終了確認
// 戻り値：const bool	生存していなければTRUE
//-------------------------------------------------------------
bool CSpriteValue::IsDestroy() const
{
	return m_isDestroy;
}

//*************************************************************
//
// private
//
//*************************************************************

//-------------------------------------------------------------
// 桁の計算
// 引　数：const int 数値
//-------------------------------------------------------------
BYTE CSpriteValue::CalcDigit(const int val)
{
	//数値格納
	int workVal = val;
	BYTE digit = 0;
	while (workVal != 0)
	{
		workVal /= 10;
		digit++;
	}

	//数値格納
	workVal = val;

	//桁数補正(0という数値が反応しない設定なので 0ならば1を加える。)
	digit = digit != 0 ? digit : 1;

	for (int i = 0; i < digit; i++)
	{
		m_num[i] = (workVal % 10);
		workVal /= 10;
	}

	return digit;
}
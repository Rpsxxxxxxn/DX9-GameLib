//*************************************************************
//
// Sprite.cpp
//
// 描画クラス
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "Sprite.h"

//=============================================================
// クラス定義
//=============================================================

//-------------------------------------------------------------
// シングルトン生成
//-------------------------------------------------------------
CSprite * CSprite::Create()
{
	static CSprite instance;
	return &instance;
}

//-------------------------------------------------------------
// 初期化
// 引　数：LPDIRECT3DDEVICE9	デバイスのポインタ
//-------------------------------------------------------------
void CSprite::Initialize(LPDIRECT3DDEVICE9 pDevice)
{
	HRESULT hr = D3DXCreateSprite(pDevice, &m_pSprite);
	if (FAILED(hr)) return;
}

//-------------------------------------------------------------
// 描画
// 引　数：RECT*				ソース画像のマスク
// 　　　　LPDIRECT3DTEXTURE9	テクスチャのポインタ
// 　　　　RECT*				描画先の位置
// 　　　　DWORD				透明度
//-------------------------------------------------------------
void CSprite::Draw(
	RECT * pSrcRect, 
	LPDIRECT3DTEXTURE9 pTexture,
	RECT * pDstRect, 
	DWORD dwColor,
	float angle
)
{
	Matrix mWorld, mTrans, mScale, mRot;

	Vector2 Scale(
		(float)(pDstRect->right - pDstRect->left) / (float)(pSrcRect->right - pSrcRect->left),
		(float)(pDstRect->bottom - pDstRect->top) / (float)(pSrcRect->bottom - pSrcRect->top)
	);

	Vector2 SrcRectCenter = 
		Vector2((pSrcRect->right - pSrcRect->left) / 2.0f, (pSrcRect->bottom - pSrcRect->top) / 2.0f);

	Vector2 DstRectCenter = 
		Vector2((pDstRect->right - pDstRect->left) / 2.0f, (pDstRect->bottom - pDstRect->top) / 2.0f);

	Vector3 SrcCenter = Vector3(SrcRectCenter.x, SrcRectCenter.y, 0.0f);

	D3DXMatrixScaling(&mScale, Scale.x, Scale.y, 1.0f);

	D3DXMatrixRotationZ(&mRot, angle);

	D3DXMatrixTranslation(&mTrans, (float)pDstRect->left + DstRectCenter.x, (float)pDstRect->top + DstRectCenter.y, 0.0f);

	mWorld = mScale * mRot * mTrans;

	m_pSprite->SetTransform(&mWorld);

	m_pSprite->Draw(pTexture, pSrcRect, &SrcCenter, NULL, D3DCOLOR_ARGB(dwColor & 0x000000FF, 255, 255, 255));

	D3DXMatrixIdentity(&mWorld);

	m_pSprite->SetTransform(&mWorld);
}

//-------------------------------------------------------------
// スプライトレンダリング開始
// 備　考：描画前に呼び出しを必ず
//-------------------------------------------------------------
void CSprite::SpriteBegin()
{
	m_pSprite->Begin(0);
}

//-------------------------------------------------------------
// スプライトレンダリング終了
// 備　考：描画後に呼び出しを必ず
//-------------------------------------------------------------
void CSprite::SpriteClose()
{
	m_pSprite->End();
}

//-------------------------------------------------------------
// スプライトのポインタ取得
//-------------------------------------------------------------
LPD3DXSPRITE CSprite::GetSprite()
{
	return m_pSprite;
}

//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
CSprite::CSprite()
	: m_pSprite(nullptr)
{
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CSprite::~CSprite()
{
	SafeRelease(m_pSprite);
}

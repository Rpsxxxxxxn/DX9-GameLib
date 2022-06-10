//*************************************************************
//
// Billboard.hpp
//
// メッシュ生成(常にこちらを向いている)
//
// Date: 2019/10/14 更新
//
//*************************************************************

//=============================================================
// マクロ定義
//=============================================================
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//=============================================================
// インクルード
//=============================================================
#include "Billboard.h"

//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
CBillboard::CBillboard()
	: m_pVB(nullptr)
	, m_vecPos(Vector3(0.0f, 0.0f, 0.0f))
	, m_vecSize(Vector2(1.0f, 1.0f))
	, m_color(0xffffffff)
{
	D3DXMatrixIdentity(&m_matWorld);
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CBillboard::~CBillboard()
{
	SafeRelease(m_pVB);
}

//-------------------------------------------------------------
// 初期化
// 引　数：LPDIRECT3DDEVICE9	デバイスのポインタ
// 　　　　LPDIRECT3DTEXTURE9	テクスチャのポインタ
// 　　　　BillboardType		ビルボード頂点生成の種類
//-------------------------------------------------------------
void CBillboard::Initialize(
	CSystem* pSystem,
	LPDIRECT3DTEXTURE9 pTexture, 
	BillboardType type
)
{
	LPDIRECT3DDEVICE9 pDevice = pSystem->GetDevice();

	m_type = type;

	m_pTexture = pTexture;

	pDevice->CreateVertexBuffer(
		sizeof(Vertex3D) * 4,
		0,
		FVF_VERTEX_3D,
		D3DPOOL_DEFAULT,
		&m_pVB,
		NULL);
}

//-------------------------------------------------------------
// 描画
// 引　数：LPDIRECT3DDEVICE9	デバイスのポインタ
// 　　　　Matrix*				ビューの逆行列
//-------------------------------------------------------------
void CBillboard::Draw(CSystem* pSystem, Matrix* pInvView)
{
	LPDIRECT3DDEVICE9 pDevice = pSystem->GetDevice();

	Update(pInvView);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	pDevice->SetRenderState(D3DRS_LIGHTING, false);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	pDevice->SetFVF(FVF_VERTEX_3D);
	pDevice->SetTexture(0, m_pTexture);
	pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	pDevice->SetStreamSource(0, m_pVB, 0, sizeof(Vertex3D));
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	pDevice->SetTexture(0, nullptr);

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

//-------------------------------------------------------------
// 色の設定
// 引　数：D3DXCOLOR	RGBA色
//-------------------------------------------------------------
void CBillboard::SetColor(D3DXCOLOR color)
{
	m_color = color;
}

//-------------------------------------------------------------
// 位置ベクトルの設定
// 引　数：Vector3*		位置ベクトルのポインタ
//-------------------------------------------------------------
void CBillboard::SetPosition(const Vector3 * vecPos)
{
	m_vecPos = *vecPos;
}

//-------------------------------------------------------------
// 拡大ベクトルの設定
// 引　数：Vector3*		拡大ベクトルのポインタ
//-------------------------------------------------------------
void CBillboard::SetSize(const Vector2 * vecSize)
{
	m_vecSize = *vecSize;
}

//-------------------------------------------------------------
// 進行状況の設定
// 引　数：float		パーセント
//-------------------------------------------------------------
void CBillboard::SetProgress(const float gauge)
{
	m_sizeX = gauge;
}

//*************************************************************
//
// private
//
//*************************************************************

//-------------------------------------------------------------
// 更新
//-------------------------------------------------------------
void CBillboard::Update(Matrix* pInvView)
{
	switch (m_type)
	{
	case BILL_CE:	GenerateVertexCE(); break;
	case BILL_LU:	GenerateVertexLU(); break;
	case BILL_CEG:	GenerateVertexCEG(); break;
	}

	Matrix matTrans;
	D3DXMatrixTranslation(&matTrans, m_vecPos.x, m_vecPos.y, m_vecPos.z);
	m_matWorld = *pInvView * matTrans;
}

//-------------------------------------------------------------
// ビルボード(中央生成)
//-------------------------------------------------------------
void CBillboard::GenerateVertexCE()
{
	Vertex3D *pVertex;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	{
		pVertex[0].vtx = Vector3(m_vecSize.x * 0.5f, -m_vecSize.y * 0.5f, 0.0f);
		pVertex[1].vtx = Vector3(m_vecSize.x * 0.5f, m_vecSize.y * 0.5f, 0.0f);
		pVertex[2].vtx = Vector3(-m_vecSize.x * 0.5f, -m_vecSize.y * 0.5f, 0.0f);
		pVertex[3].vtx = Vector3(-m_vecSize.x * 0.5f, m_vecSize.y * 0.5f, 0.0f);

		pVertex[0].nor =
		pVertex[1].nor =
		pVertex[2].nor =
		pVertex[3].nor = Vector3(0.0f, 0.0f, -1.0f);

		pVertex[0].dif =
		pVertex[1].dif =
		pVertex[2].dif =
		pVertex[3].dif = m_color;

		pVertex[0].tex = Vector2(1.0f, 0.0f);
		pVertex[1].tex = Vector2(1.0f, 1.0f);
		pVertex[2].tex = Vector2(0.0f, 0.0f);
		pVertex[3].tex = Vector2(0.0f, 1.0f);
	}

	m_pVB->Unlock();
}

//-------------------------------------------------------------
// ビルボード(左上生成)
//-------------------------------------------------------------
void CBillboard::GenerateVertexLU()
{
	Vertex3D *pVertex;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	{
		pVertex[0].vtx = Vector3(m_vecSize.x, 0.0f, 0.0f);
		pVertex[1].vtx = Vector3(m_vecSize.x, m_vecSize.y, 0.0f);
		pVertex[2].vtx = Vector3(0.0f, 0.0f, 0.0f);
		pVertex[3].vtx = Vector3(0.0f, m_vecSize.y, 0.0f);

		pVertex[0].nor = 
		pVertex[1].nor = 
		pVertex[2].nor = 
		pVertex[3].nor = Vector3(0.0f, 0.0f, -1.0f);

		pVertex[0].dif = 
		pVertex[1].dif = 
		pVertex[2].dif = 
		pVertex[3].dif = m_color;

		pVertex[0].tex = Vector2(1.0f, 0.0f);
		pVertex[1].tex = Vector2(1.0f, 1.0f);
		pVertex[2].tex = Vector2(0.0f, 0.0f);
		pVertex[3].tex = Vector2(0.0f, 1.0f);
	}

	m_pVB->Unlock();
}

//-------------------------------------------------------------
// ビルボード(中央生成ゲージ用)
//-------------------------------------------------------------
void CBillboard::GenerateVertexCEG()
{
	Vertex3D *pVertex;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	{
		pVertex[0].vtx = Vector3(-m_vecSize.x * 0.5f + m_sizeX, -m_vecSize.y * 0.5f, 0.0f);
		pVertex[1].vtx = Vector3(-m_vecSize.x * 0.5f + m_sizeX, m_vecSize.y * 0.5f, 0.0f);
		pVertex[2].vtx = Vector3(-m_vecSize.x * 0.5f, -m_vecSize.y * 0.5f, 0.0f);
		pVertex[3].vtx = Vector3(-m_vecSize.x * 0.5f, m_vecSize.y * 0.5f, 0.0f);

		pVertex[0].nor =
		pVertex[1].nor =
		pVertex[2].nor =
		pVertex[3].nor = Vector3(0.0f, 0.0f, 1.0f);

		pVertex[0].dif =
		pVertex[1].dif =
		pVertex[2].dif =
		pVertex[3].dif = m_color;

		pVertex[0].tex = Vector2(1.0f, 0.0f);
		pVertex[1].tex = Vector2(1.0f, 1.0f);
		pVertex[2].tex = Vector2(0.0f, 0.0f);
		pVertex[3].tex = Vector2(0.0f, 1.0f);
	}

	m_pVB->Unlock();
}

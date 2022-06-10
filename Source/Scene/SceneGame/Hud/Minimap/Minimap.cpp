//********************************************************************************
//
// CMinimap.hpp
//
// ミニマップクラス
//
// Date: 2019/07/30 更新
//
//********************************************************************************

//================================================================================
// インクルード
//================================================================================
#include "Minimap.hpp"

using namespace FileConst;

//================================================================================
// クラス定義
//================================================================================

//--------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------
CMinimap::CMinimap()
{
}

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
CMinimap::~CMinimap()
{
	for (int i = 0; i < MAX; i++)
	{
		SafeDelete(m_pSprite[i]);
	}
}

//--------------------------------------------------------------------------------
// ファイル読み込み
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CMinimap::LoadFile(CSystem * pSystem)
{
	pSystem->LoadTexture(TEXTURE_MINIMAP,		"Resource/Textures/Game/Minimap.png");
	pSystem->LoadTexture(TEXTURE_MINIMAP_ENEMY, "Resource/Textures/Game/MinimapNotFound.png");
	pSystem->LoadTexture(TEXTURE_MINIMAP_BOSS,	"Resource/Textures/Game/MinimapFound.png");
}

//--------------------------------------------------------------------------------
// 初期化
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CMinimap::Initialize(CSystem * pSystem)
{
	auto windowWidth = (float)pSystem->GetWindowSettings().Width;
	auto mapSourSizeX = pSystem->GetTextureData(TEXTURE_MINIMAP)->uiWidth;
	auto mapSourSizeY = pSystem->GetTextureData(TEXTURE_MINIMAP)->uiHeight;

	m_pSprite[MINIMAP_BACK] = new CSpriteAnimation(pSystem->GetTextureData(TEXTURE_MINIMAP));

	auto drawWidth = windowWidth - mapSourSizeX;
	m_pSprite[MINIMAP_BACK]->SetPosition(Vector2(drawWidth, 10.0f));

	m_minimapCenterX = drawWidth + ((float)mapSourSizeX / 2.0f);
	m_minimapCenterY = ((float)mapSourSizeY / 2.0f);
}

//--------------------------------------------------------------------------------
// 描画
// 引　数：CSystem*			システムクラスのポインタ
// 　　　　CPlayerCamera*	カメラクラスのポインタ
// 　　　　CPlayer*			プレイヤークラスのポインタ
// 　　　　CAIManager*		AI管理
//--------------------------------------------------------------------------------
void CMinimap::Draw(
	CSystem * pSystem, 
	CPlayerCamera * pCamera,
	CPlayer * pPlayer,
	CAIManager* pAi
)
{
	m_pSprite[MINIMAP_BACK]->Draw(pSystem);

	auto camAngle = pCamera->GetAngle().y;

	for (auto enemy : pAi->GetEnemyList())
	{
		auto vecPos = enemy->GetPosition();
		auto toPos = vecPos - pPlayer->GetPosition();
		auto angle = enemy->GetRotate().y + -camAngle;
		auto dist = VectorCalculate::Magnitude(toPos);

		if (dist < 30.0f)
		{
			CompressAnglePosition(&vecPos, toPos, camAngle, dist * 3.0f);
			DrawPosition(pSystem, &vecPos, angle + D3DXToRadian(180), true);
		}
		else {
			CompressAnglePosition(&vecPos, toPos, camAngle, 90.0f);
			DrawPosition(pSystem, &vecPos, angle + D3DXToRadian(180), true);
		}
	}
}

//********************************************************************************
//
// Private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// ポジションの描画 (Enemy)
// 引　数：CSystem*　	３Ｄエンジンクラスへのポインタ
//		 　Vector3*		位置ベクトルポインタ
// 　　　　float		回転角度
// 　　　　bool			デフォルトTRUE -> ボスならばFALSE
//--------------------------------------------------------------------------------
void CMinimap::DrawPosition(CSystem * pSystem, Vector3 * pVecPos, float fAngle, bool bEnemy)
{
	RECT rSour, rDest;

	auto vecPos = *pVecPos;

	auto posX = (int)vecPos.x;
	auto posY = (int)vecPos.z;

	SetRect(&rSour, 0, 0, 18, 18);

	SetRect(&rDest,
		posX - 8,
		posY - 8,
		posX + 8,
		posY + 8
	);

	pSystem->SpriteBegin();

	if (bEnemy)
	{
		pSystem->DrawSprite(
			&rSour,
			pSystem->GetTexture(TEXTURE_MINIMAP_ENEMY),
			&rDest,
			255, 
			fAngle);
	}
	else {
		pSystem->DrawSprite(
			&rSour, 
			pSystem->GetTexture(TEXTURE_MINIMAP_BOSS),
			&rDest, 
			255, 
			fAngle);
	}

	pSystem->SpriteClose();
}

//--------------------------------------------------------------------------------
// 圧縮ポジション
// 引　数：Vector3*　		位置ベクトルのポインタ
//		 ：Vector3			位置ベクトル
//		 ：float			回転
//		 ：float			距離
//--------------------------------------------------------------------------------
void CMinimap::CompressAnglePosition(Vector3 * pVecPos, Vector3 vecPos, float fAngle, float fDistance)
{
	float angle = atan2(vecPos.x, vecPos.z); //角度
	angle += -fAngle - D3DX_PI * 1.5f;
	float posX = fDistance * cosf(angle);
	float posY = fDistance * sinf(angle);
	pVecPos->x = m_minimapCenterX + posX;
	pVecPos->z = m_minimapCenterY + posY;
}

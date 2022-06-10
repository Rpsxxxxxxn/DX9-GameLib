//********************************************************************************
//
// Damage.hpp
//
// ダメージ量表示用クラス
//
// Date: 2019/07/30 更新
//
//********************************************************************************

//================================================================================
// インクルード
//================================================================================
#include "Damage.hpp"

using namespace FileConst;

//================================================================================
// クラス定義
//================================================================================

//--------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------
CShowDamage::CShowDamage()
{
}

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
CShowDamage::~CShowDamage()
{
	for (auto w : m_vecSprite)
	{
		SafeDelete(w.pSpriteValue);
	}
	m_vecSprite.clear();
}

//--------------------------------------------------------------------------------
// ファイル読み込み
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CShowDamage::LoadFile(CSystem * pSystem)
{
	pSystem->LoadTexture(TEXTURE_NUMBER, "Resource/Textures/Share/Number.png");
}

//--------------------------------------------------------------------------------
// 更新
// 引　数：CSystem*			システムクラスのポインタ
// 　　　　CPlayerCamera	カメラクラスのポインタ
//--------------------------------------------------------------------------------
void CShowDamage::Update(CSystem * pSystem, CPlayerCamera* pCamera)
{
	EraseDamage();

	auto device = pSystem->GetDevice();
	auto camera = pCamera->GetCamera();

	for (auto w : m_vecSprite)
	{
		Vector3 screenPos = Calculate::WorldToScreen(device, &camera->GetViewMatrix(), &camera->GetProjectionMatrix(), &w.vecPos);

		w.pSpriteValue->SetPosition(Vector2(screenPos.x, screenPos.y));
	}
}

//--------------------------------------------------------------------------------
// 描画
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CShowDamage::Draw(CSystem * pSystem)
{
	for (auto w : m_vecSprite)
	{
		w.pSpriteValue->Draw(pSystem);
	}
}

//--------------------------------------------------------------------------------
// ダメージ表示の追加
// 引　数：CSystem*			システムクラスのポインタ
// 　　　　const Vector3	表示する位置ベクトル
// 　　　　const int		ダメージ量
//--------------------------------------------------------------------------------
void CShowDamage::AddDamage(CSystem* pSystem, const Vector3 vecPos, const int damage)
{
	DamageData w;

	w.vecPos.x = vecPos.x + Calculate::Random(-0.3f, 0.3f);
	w.vecPos.y = vecPos.y + Calculate::Random(-0.3f, 0.3f);
	w.vecPos.z = vecPos.z + Calculate::Random(-0.3f, 0.3f);

	w.pSpriteValue = new CSpriteValue(pSystem->GetTextureData(TEXTURE_NUMBER));

	w.pSpriteValue->SetSize(60, 60);
	w.pSpriteValue->SetValue(damage);
	w.pSpriteValue->SetEndTime(1000);
	w.pSpriteValue->SetNewAlpha(0);

	m_vecSprite.push_back(w);
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// ダメージ表示の削除
//--------------------------------------------------------------------------------
void CShowDamage::EraseDamage()
{
	for (auto itr = begin(m_vecSprite); itr != end(m_vecSprite); )
	{
		if ((*itr).pSpriteValue->IsDestroy())
		{
			SafeDelete((*itr).pSpriteValue);
	
			itr = m_vecSprite.erase(itr);
		}
		else { itr++; };
	}
}

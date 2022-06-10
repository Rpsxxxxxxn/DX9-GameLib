//********************************************************************************
//
// Sword.hpp
//
// 剣のクラス
//
// Date: 2019/07/30 更新
//
//********************************************************************************

//================================================================================
// インクルード
//================================================================================
#include "Sword.hpp"

using namespace FileConst;

//--------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------
CSword::CSword()
	: m_pModel(new CXFileModel)
	, m_pAttackBox(new CBoundingBox)
{
}

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
CSword::~CSword()
{
	SafeDelete(m_pModel);
	SafeDelete(m_pAttackBox);
}

//--------------------------------------------------------------------------------
// 初期化
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CSword::Initialize(CSystem * pSystem)
{
	m_pModel->SetModel(pSystem->GetXFile(MODEL_SWORD));

	auto max = Vector3( 0.25f,  0.25f,  0.0f);
	auto min = Vector3(-0.25f, -0.25f, -1.5f);
	m_pAttackBox->Create(pSystem, &max, &min, false);
}

//--------------------------------------------------------------------------------
// 描画
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CSword::ZDraw(CSystem * pSystem)
{
	m_pModel->Draw(pSystem, Shader::DEPTH, m_isVisible, false);
}

//--------------------------------------------------------------------------------
// 描画
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CSword::Draw(CSystem * pSystem)
{
	m_pModel->Draw(pSystem, Shader::SHADOW, m_isVisible, !m_isSleep);
	m_pAttackBox->Draw(pSystem);
}

//--------------------------------------------------------------------------------
// 世界行列の設定
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CSword::SetTransform(Matrix * pMatWorld)
{
	m_pModel->SetTransform(pMatWorld);
	m_pAttackBox->SetTransform(pMatWorld);
}
//*************************************************************
//
// Audio.cpp
//
// オーディオ再生用クラス
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "XFileModel.h"

//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
CXFileModel::CXFileModel()
	: m_pXFile(nullptr)
	, m_pAnimationController(nullptr)
	, m_nowAnimationSet(0)
	, m_numAnimationSets(0)
	, m_advanceTime(0.0)
	, m_isLoadedSkinMesh(false)
	, m_meshAlpha(1.0f)
{
	D3DXMatrixIdentity(&m_matWorld);
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CXFileModel::~CXFileModel()
{
	m_vecAnim.clear();
}

//-------------------------------------------------------------
// モデルの設定
//-------------------------------------------------------------
void CXFileModel::SetModel(CXFile * pModel)
{
	//スキンメッシュの設定
	m_pXFile = pModel;

	//コントローラーの取得
	m_pAnimationController = m_pXFile->GetAnimationController();

	//アニメーションコントローラーの設定
	if (m_pAnimationController) 
	{
		m_vecAnim.clear();

		m_numAnimationSets = m_pAnimationController->GetNumAnimationSets();

		for (UINT i = 0; i < m_numAnimationSets; i++) {
			m_vecAnim.push_back(make_unique<AnimDesc>());

			m_pAnimationController->GetAnimationSet(i, &(m_vecAnim[i]->pAnimSet));
			m_vecAnim[i]->trackSpeed = static_cast<FLOAT>(m_vecAnim[i]->pAnimSet->GetPeriod());
			m_vecAnim[i]->shiftTime = 0.1f;
		}
	}

	//ロード済み
	m_isLoadedSkinMesh = true;
}

//-------------------------------------------------------------
// 世界行列の設定
//-------------------------------------------------------------
void CXFileModel::SetTransform(Matrix * pMatWorld)
{
	m_matWorld = *pMatWorld;
}

//-------------------------------------------------------------
// 描画
// 引　数：bool	視錘台カリング用
// 　　　　bool	アニメーション進行
//-------------------------------------------------------------
void CXFileModel::Draw(CSystem* pSystem, DWORD value, bool bVisible, bool bAnimation)
{
	if (bAnimation)
	{
		this->AdvanceTime();
	}

	if(bVisible)
	{
		pSystem->SetShaderState(value);
		m_pXFile->Draw(&m_matWorld, m_meshAlpha);
	}
}

//-------------------------------------------------------------
// フレーム名から変換マトリックスを取得
// 引　数：std::string  フレーム名
// 　　　　LPD3DXMATRIX 変換マトリックスのポインタ
//-------------------------------------------------------------
void CXFileModel::GetMatrixFromFrameName(string strFrameName, Matrix * pMatWorld)
{
	m_pXFile->GetMatrixFromFrameName(strFrameName, pMatWorld);
}

//-------------------------------------------------------------
// アニメーションの時間設定
// 引　数：const double	１フレーム間の時間
//-------------------------------------------------------------
void CXFileModel::SetAdvanceTime(const double deltaTime)
{
	m_advanceTime = deltaTime;
}

//-------------------------------------------------------------
// アニメーションの変更
// 引　数：const UINT	アニメーションの番号
//-------------------------------------------------------------
void CXFileModel::SetChangeAnimation(const UINT animID)
{
	//アニメーションの最大数よりも小さいか？
	if (m_vecAnim.size() <= animID) 
	{
		return;
	}

	//現在のアニメーションと同じかどうか
	if (m_nowAnimationSet == animID) 
	{
		return;
	}

	//現在のアニメーションセットの設定値を取得
	D3DXTRACK_DESC TrackDesc;
	m_pAnimationController->GetTrackDesc(0, &TrackDesc);

	//今のアニメーションをトラック1に移動
	m_pAnimationController->SetTrackAnimationSet(1, m_vecAnim[m_nowAnimationSet]->pAnimSet);
	m_pAnimationController->SetTrackDesc(1, &TrackDesc);

	//新しいアニメーションセットをトラック0に設定
	m_pAnimationController->SetTrackAnimationSet(0, m_vecAnim[animID]->pAnimSet);

	//トラックスピードの設定
	m_pAnimationController->SetTrackSpeed(0, m_vecAnim[animID]->trackSpeed);

	//トラックの合成を許可
	m_pAnimationController->SetTrackEnable(0, true);
	m_pAnimationController->SetTrackEnable(1, true);

	//トラックの位置を最初にもどす
	m_pAnimationController->SetTrackPosition(0, 0);

	//ウェイト時間を初期化
	m_vecAnim[animID]->curWeightTime = 0.0f;

	//現在のアニメーション番号を切り替え
	m_nowAnimationSet = animID;
}

//-------------------------------------------------------------
// アニメーションループ時間の設定
// 引　数：const UINT	アニメーションの番号
// 　　　　const float	ms
//-------------------------------------------------------------
void CXFileModel::SetLoopTime(const UINT animID, const float time)
{
	if (m_vecAnim.size() <= animID) 
	{
		return;
	}

	//トラックスピード算出
	FLOAT defTime = static_cast<FLOAT>(m_vecAnim[animID]->pAnimSet->GetPeriod());
	m_vecAnim[animID]->loopTime = time;
	m_vecAnim[animID]->trackSpeed = defTime / time;
}

//-------------------------------------------------------------
// アニメーション補間時間の設定
// 引　数：const UINT	アニメーションの番号
// 　　　　const float	間隔
//-------------------------------------------------------------
void CXFileModel::SetShiftTime(const UINT animID, const float interval)
{
	if (m_vecAnim.size() <= animID) 
	{
		return;
	}

	m_vecAnim[animID]->shiftTime = interval;
}

//-------------------------------------------------------------
// ターゲットベクトル
// 引　数：const Vector3	ターゲットベクトル
//-------------------------------------------------------------
void CXFileModel::SetTargetVector(const UINT index, const Vector3 vecTarget)
{
	m_pXFile->SetTargetVector(index, vecTarget);
}

//-------------------------------------------------------------
// ボーンフレームの設定
// 引　数：const string ボーン名1
// 　　　　const string ボーン名2
// 　　　　const string ボーン名3
//-------------------------------------------------------------
void CXFileModel::SetBoneFrame(string bone1, string bone2, string bone3)
{
	m_pXFile->SetBoneFrame(bone1, bone2, bone3);
}

//-------------------------------------------------------------
// メッシュの透明度を設定します
// 引　数：float 透明度(1.0f ~ 0.0f) 
//-------------------------------------------------------------
void CXFileModel::SetMeshAlpha(const float value)
{
	m_meshAlpha = value;
}

//-------------------------------------------------------------
// モデル内部アニメーション経過時間の取得
// 戻り値：double	タイムの取得
//-------------------------------------------------------------
double CXFileModel::GetTime() const
{
	return m_pAnimationController->GetTime();
}

//-------------------------------------------------------------
// ボーンの位置ベクトルの取得
//-------------------------------------------------------------
Vector3 CXFileModel::GetBonePos(const UINT index) const
{
	return m_pXFile->GetBonePos(index);
}

//*************************************************************
//
// private
//
//*************************************************************

//-------------------------------------------------------------
// アニメーションの再生
//-------------------------------------------------------------
void CXFileModel::AdvanceTime()
{
	if (m_pAnimationController)
	{
		m_vecAnim[m_nowAnimationSet]->curWeightTime += static_cast <float>(m_advanceTime);

		if (m_vecAnim[m_nowAnimationSet]->curWeightTime <= m_vecAnim[m_nowAnimationSet]->shiftTime) {

			FLOAT Weight = m_vecAnim[m_nowAnimationSet]->curWeightTime / m_vecAnim[m_nowAnimationSet]->shiftTime;
			m_pAnimationController->SetTrackWeight(0, Weight);
			m_pAnimationController->SetTrackWeight(1, 1 - Weight);
		}
		else {
			m_pAnimationController->SetTrackWeight(0, 1.0f);
			m_pAnimationController->SetTrackEnable(1, false);
		}

		//時間の更新
		m_pAnimationController->AdvanceTime(m_advanceTime, nullptr);
	}
}

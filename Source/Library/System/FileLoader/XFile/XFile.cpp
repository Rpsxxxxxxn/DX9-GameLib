//*************************************************************
//
// FileBase.cpp
//
// XFileファイル読み込み用クラス
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "XFile.h"

//=============================================================
// クラス定義
//=============================================================

//-------------------------------------------------------------
// コンストラクタ
// 引　数：IDirect3DDevice9*	デバイスのポインタ
// 　　　　string				ディレクトリ
//-------------------------------------------------------------
CXFile::CXFile(IDirect3DDevice9* pDevice, string strFileName)
	: CFileBase(pDevice)
	, m_pIK(nullptr)
	, m_pAllocateHierarchy(nullptr)
	, m_pFrame(nullptr)
	, m_isLoaded(false)
	, m_isIK(false)
{
	D3DXMatrixIdentity(&m_matWorld);

	this->Initialize(strFileName);
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CXFile::~CXFile()
{
	this->Release();
}

//-------------------------------------------------------------
// 初期化
// 引　数：string	ファイルディレクトリ
//-------------------------------------------------------------
void CXFile::Initialize(string path)
{
	//ヒエラルキークラスの生成
	m_pAllocateHierarchy = new CAllocateHierarchy();

	//フレームクラスの生成
	m_pFrame = new CXFrame();

	auto pFrameRoot = m_pFrame->GetFrame();

	//モデルデータの読み込み
	auto hr = D3DXLoadMeshHierarchyFromX((LPCSTR)path.c_str(), D3DXMESH_MANAGED, m_pDevice, m_pAllocateHierarchy, NULL, pFrameRoot, &m_pAnimCtrl);

	//ボーンマトリックスの設定
	m_pFrame->SetupBoneMatrixPointers(*pFrameRoot);

	if (m_pAnimCtrl)
	{
		m_pIK = new CXInverseKinematics(m_pFrame);
		m_isIK = true;
	}

	//ロード済み
	m_isLoaded = true;
}

//-------------------------------------------------------------
// 解放
//-------------------------------------------------------------
void CXFile::Release()
{
	D3DXFrameDestroy(*(m_pFrame->GetFrame()), m_pAllocateHierarchy);
	SafeDelete(m_pAllocateHierarchy);
	SafeDelete(m_pFrame);
	SafeDelete(m_pIK);
	SafeRelease(m_pAnimCtrl);
}

//-------------------------------------------------------------
// 描画
//-------------------------------------------------------------
void CXFile::Draw(Matrix* pWorld, float meshAlpha)
{
	m_pFrame->UpdateFrameMatrices(*(m_pFrame->GetFrame()), pWorld);

	if (m_isIK)
	{
		m_pIK->UpdateIK();
	}

	m_pFrame->DrawFrame(m_pDevice, *(m_pFrame->GetFrame()), pWorld, meshAlpha);
}

//-------------------------------------------------------------
// フレーム名から変換マトリックスを取得
// 引　数：string	フレームの名前
// 　　　　Matrix	行列のポインタ取得
// 備　考：フレーム名からマトリックス行列を取得します
//-------------------------------------------------------------
void CXFile::GetMatrixFromFrameName(string strFrameName, Matrix * pMatWorld)
{
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)m_pFrame->GetFrameName(strFrameName.c_str());

	*pMatWorld = pFrame->CombinedTransformationMatrix;
}

//-------------------------------------------------------------
// ターゲットベクトル
// 引　数：const Vector3	ターゲットベクトル
//-------------------------------------------------------------
void CXFile::SetTargetVector(const UINT index, const Vector3 vecTarget)
{
	if (m_isIK) m_pIK->SetTargetVector(index, vecTarget);
}

//-------------------------------------------------------------
// IK用ボーン名設定
// 引　数：const string ボーン名1
// 　　　　const string ボーン名2
// 　　　　const string ボーン名3
//-------------------------------------------------------------
void CXFile::SetBoneFrame(string bone1, string bone2, string bone3)
{
	if(m_isIK) m_pIK->SetBoneFrame(bone1, bone2, bone3);
}

//-------------------------------------------------------------
// ボーンの位置ベクトルの取得
//-------------------------------------------------------------
Vector3 CXFile::GetBonePos(const UINT index) const
{
	if (m_isIK)
	{
		return m_pIK->GetBonePos(index);
	}

	return Vector3(0.0f, 0.0f, 0.0f);
}

//-------------------------------------------------------------
// アニメーションコントローラーの取得
// 備　考：アニメーションのコントローラーを取得します
//-------------------------------------------------------------
ID3DXAnimationController * CXFile::GetAnimationController()
{
	if (!m_pAnimCtrl)
	{
		return nullptr;
	}

	ID3DXAnimationController* pResult;

	auto maxNumAnimOut = m_pAnimCtrl->GetMaxNumAnimationOutputs();
	auto maxNumAnimSets = m_pAnimCtrl->GetMaxNumAnimationSets();
	auto maxNumTracks = m_pAnimCtrl->GetMaxNumTracks();
	auto maxNumEvents = m_pAnimCtrl->GetMaxNumEvents();

	m_pAnimCtrl->CloneAnimationController(maxNumAnimOut, maxNumAnimSets, maxNumTracks, maxNumEvents, &pResult);

	return pResult;
}

//-------------------------------------------------------------
// ルートメッシュの取得
//-------------------------------------------------------------
LPD3DXMESH CXFile::GetRootMesh()
{
	return m_pFrame->GetRootMesh();
}

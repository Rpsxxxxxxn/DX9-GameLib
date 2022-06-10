//*************************************************************
//
// ShaderManager.cpp
//
// シェーダクラス管理
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "ShaderManager.h"

//-------------------------------------------------------------
// 初期化
// 引　数：CWindowSystem*	ウィンドウシステムクラスのポインタ
//-------------------------------------------------------------
void CShaderManager::Initialize(
	LPDIRECT3DDEVICE9 pDevice,
	CWindowSystem * pWindowSystem
)
{
	m_pDevice = pDevice;

	auto& ws_ = pWindowSystem->GetWindowSettings();

	auto* pDepth = new CShadowDepth(ws_.Width, ws_.Height);
	pDepth->Initialize(pDevice);
	this->AddShader(pDepth);

	auto* pShadow = new CShadowMap;
	pShadow->Initialize(pDevice);
	this->AddShader(pShadow);

	auto* pDefault = new CDefaultShader;
	pDefault->Initialize(pDevice);
	this->AddShader(pDefault);
}

//-------------------------------------------------------------
// 更新
//-------------------------------------------------------------
void CShaderManager::Update()
{
	for (auto itr = m_vecShader.begin(); itr != m_vecShader.end(); ++itr)
	{
		(*itr)->Update();
		(*itr)->SetValue("g_vLDir", m_vecLightDir);
		(*itr)->SetValue("g_vEyePos", m_vecCameraEye);
		(*itr)->SetValue("g_mView", m_matView);
		(*itr)->SetValue("g_mProj", m_matProj);
		(*itr)->SetValue("g_mLBias", m_matLBias);
		(*itr)->SetValue("g_mLVP", m_matLViewProj, 3);
	}
}

//-------------------------------------------------------------
// シェーダの追加
//-------------------------------------------------------------
void CShaderManager::AddShader(CShaderBase* pShader)
{
	m_vecShader.push_back(pShader);
}

//-------------------------------------------------------------
// シェーダの描画設定
// 引　数：ShaderType シェーダの種類
// 備　考：メッシュの描画前に呼び出しをしてください。
//-------------------------------------------------------------
void CShaderManager::SetShaderState(Shader Value)
{
	m_nowShaderState = Value;
}

//-------------------------------------------------------------
// 現在のシェーダ状態の取得
// 戻り値：ShaderType シェーダの種類
// 備　考：描画前に設定変更のため呼び出し。
//-------------------------------------------------------------
Shader CShaderManager::GetShaderState() const
{
	return m_nowShaderState;
}

//-------------------------------------------------------------
// シェーダの取得
// 引　数：const int 
//-------------------------------------------------------------
CShaderBase* CShaderManager::GetShader(const UINT index)
{
	if (index >= m_vecShader.size()) return nullptr;

	return m_vecShader[index];
}

//-------------------------------------------------------------
// シェーダのパラメータ設定
// 引　数：D3DXMATRIX* ビュー行列の設定 
//-------------------------------------------------------------
void CShaderManager::SetMatView(const Matrix& pMatView)
{
	m_matView = pMatView;
}

//-------------------------------------------------------------
// シェーダのパラメータ設定
// 引　数：D3DXMATRIX* プロジェクション行列の設定 
//-------------------------------------------------------------
void CShaderManager::SetMatProj(const Matrix& pMatProj)
{
	m_matProj = pMatProj;
}

//-------------------------------------------------------------
// シェーダのパラメータ設定
// 引　数：D3DXMATRIX* ライトビュー行列の設定 
//-------------------------------------------------------------
void CShaderManager::SetMatLView(const Matrix& pMatView)
{
	m_matLView = pMatView;
}

//-------------------------------------------------------------
// シェーダのパラメータ設定
// 引　数：D3DXMATRIX* プロジェクション行列の設定 
//-------------------------------------------------------------
void CShaderManager::SetMatLProj(const Matrix& pMatProj)
{
	m_matLProj = pMatProj;
}

//-------------------------------------------------------------
// シェーダのパラメータ設定
// 引　数：D3DXMATRIX* ライトビュープロジェクション行列の設定 
//-------------------------------------------------------------
void CShaderManager::SetMatLViewProj(Matrix* pMatViewProj)
{
	m_matLViewProj = pMatViewProj;
}

//-------------------------------------------------------------
// シェーダのパラメータ設定
// 引　数：D3DXMATRIX* シャドウ微調整用行列
//-------------------------------------------------------------
void CShaderManager::SetMatLBias(const Matrix& pMatProj)
{
	m_matLBias = pMatProj;
}

//-------------------------------------------------------------
// シェーダのパラメータ設定
// 引　数：D3DXMATRIX* ライト視点3次元ベクトルの設定 
//-------------------------------------------------------------
void CShaderManager::SetLightEye(const Vector3* pVecEye)
{
	Vector3 eye = *pVecEye;
	Vector3 dir = eye;
	D3DXVec3Normalize(&dir, &dir);
	m_vecLightDir = Vector4(dir, 1.0f);
}

//-------------------------------------------------------------
// シェーダのパラメータ設定
// 引　数：D3DXMATRIX* カメラ視点3次元ベクトルの設定 
//-------------------------------------------------------------
void CShaderManager::SetCameraEye(const Vector3* pVecEye)
{
	m_vecCameraEye = Vector4(*pVecEye, 1.0f);
}

//=============================================================
//
// Private
//
//=============================================================

//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
CShaderManager::CShaderManager()
	: m_nowShaderState(Shader::DEPTH)
{
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CShaderManager::~CShaderManager()
{
	if (!m_vecShader.empty())
	{
		for (auto shader : m_vecShader)
		{
			if (shader != nullptr)
			{
				delete shader;
				shader = nullptr;
			}
		}
		m_vecShader.clear();
	}
}

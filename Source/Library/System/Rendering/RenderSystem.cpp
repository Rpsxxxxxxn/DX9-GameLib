//*************************************************************
//
// RenderSystem.cpp
//
// DirectX9初期化用クラス
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "RenderSystem.h"

//-------------------------------------------------------------
// シングルトン生成
//-------------------------------------------------------------
CRenderSystem * CRenderSystem::Create()
{
	static CRenderSystem instance;
	return &instance;
}

//-------------------------------------------------------------
// 初期化
// 引　数：CWindowSystem	ウィンドウシステムクラスのポインタ
//-------------------------------------------------------------
void CRenderSystem::Initialize(CWindowSystem* pWindowSystem)
{
	//ウィンドウハンドルの取得
	auto hWnd = pWindowSystem->GetHWND();

	//ウィンドウ設定の取得
	auto ws_ = pWindowSystem->GetWindowSettings();

	//SDKバージョンの設定
	m_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);

	//DirectXパラメータ設定用クラス
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth			= ws_.Width;
	d3dpp.BackBufferHeight			= ws_.Height;
	d3dpp.BackBufferFormat			= D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount			= 1;
	d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality		= 0;
	d3dpp.SwapEffect				= D3DSWAPEFFECT_FLIP;
	d3dpp.hDeviceWindow				= NULL;
	d3dpp.Windowed					= ws_.isWindowMode;
	d3dpp.EnableAutoDepthStencil	= TRUE;
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;
	d3dpp.Flags						= 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;

	// デバイスの作成
	if (FAILED(m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice)))
		if (FAILED(m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice)))
			if (FAILED(m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice)))
				if (FAILED(m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice)))
					MessageBox(hWnd, "デバイスの生成に失敗しました。", "ERROR", MB_OK);

	ImGui_Start::ImGui_Init(hWnd ,m_pDevice);
}

//-------------------------------------------------------------
// 解放
//-------------------------------------------------------------
void CRenderSystem::Release()
{
	SafeRelease(m_pDevice);
	SafeRelease(m_pDirect3D);
}

//-------------------------------------------------------------
// デバイスの取得
// 戻り値：const LPDIRECT3DDEVICE9	デバイスのポインタ
//-------------------------------------------------------------
LPDIRECT3DDEVICE9 CRenderSystem::GetDevice() const
{
	return m_pDevice;
}

//-------------------------------------------------------------
// バックバッファのクリア
// 引　数：const DWORD	色の指定
//-------------------------------------------------------------
void CRenderSystem::Clear(const DWORD value)
{
	m_pDevice->Clear(
		0, 
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		value,
		1.0f,
		0);
}

//-------------------------------------------------------------
// レンダリング開始
//-------------------------------------------------------------
void CRenderSystem::BeginRender()
{
	m_pDevice->BeginScene();
}

//-------------------------------------------------------------
// レンダリング終了
//-------------------------------------------------------------
void CRenderSystem::EndRender()
{
	m_pDevice->EndScene();
}

//-------------------------------------------------------------
// 画面スワップ
//-------------------------------------------------------------
void CRenderSystem::Present()
{
	m_pDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

//-------------------------------------------------------------
// スプライトレンダリング開始
// 備　考：描画前に呼び出しを必ず
//-------------------------------------------------------------
void CRenderSystem::SpriteBegin()
{
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

//-------------------------------------------------------------
// スプライトレンダリング終了
// 備　考：描画後に呼び出しを必ず
//-------------------------------------------------------------
void CRenderSystem::SpriteClose()
{
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
CRenderSystem::CRenderSystem()
	: m_pDevice(nullptr)
	, m_pDirect3D(nullptr)
{
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CRenderSystem::~CRenderSystem()
{
	this->Release();
}

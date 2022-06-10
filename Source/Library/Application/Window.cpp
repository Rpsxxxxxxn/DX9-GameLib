//*************************************************************
//
// Window.cpp
//
// ウィンドウ管理
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "Window.h"

//=============================================================
// クラス定義
//=============================================================

//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
CWindowSystem::CWindowSystem()
	: m_strClassName("DX9-GameLib")
{
	this->LoadFile();
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CWindowSystem::~CWindowSystem() {}

//-------------------------------------------------------------
// 初期化
// 引　数：HINSTANCE	インスタンスハンドル
// 　　　　const int	アプリケーション初期表示方法
//-------------------------------------------------------------
void CWindowSystem::Initialize(HINSTANCE hInst, const int nShowCmd)
{
	if (MessageBox(nullptr, "フルスクリーンモードで起動しますか？", "起動確認", MB_YESNO) == IDYES)
	{
		m_windowSettings.Width = GetSystemMetrics(SM_CXSCREEN);
		m_windowSettings.Height = GetSystemMetrics(SM_CYSCREEN);
		m_windowSettings.isWindowMode = false;
	}

	//ウィンドウ登録
	this->WindowRegister(hInst, nShowCmd);

	//ウィンドウ生成
	this->WindowCreate(hInst, nShowCmd);
}

//-------------------------------------------------------------
// ウィンドウが処理されているか
// 戻り値：BOOL 閉じられればFALSE
//-------------------------------------------------------------
bool CWindowSystem::IsLoop()
{
	if (PeekMessage(&m_msg, NULL, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&m_msg);

		DispatchMessage(&m_msg);
	}

	if (m_msg.message != WM_QUIT)
	{
		return true;
	}

	return false;
}

//-------------------------------------------------------------
// ウィンドウ設定の取得
// 戻り値：WindowSettings*	ウィンドウ設定
//-------------------------------------------------------------
WindowSettings CWindowSystem::GetWindowSettings() const
{
	return m_windowSettings;
}

//-------------------------------------------------------------
// ウィンドウハンドルの取得
// 戻り値：HWND	ウィンドウハンドル
//-------------------------------------------------------------
HWND CWindowSystem::GetHWND() const
{
	return m_hWnd;
}

//-------------------------------------------------------------
// インスタンスアプリケーションの取得
//-------------------------------------------------------------
HINSTANCE CWindowSystem::GetInstApp()
{
	return m_hInstance;
}

//-------------------------------------------------------------
// ウィンドウ登録
// 引　数：HINSTANCE	インスタンスハンドル
// 　　　　const int	アプリケーション初期表示方法
//-------------------------------------------------------------
void CWindowSystem::WindowRegister(HINSTANCE hInst, const int nShowCmd)
{
	//ウィンドウクラス
	WNDCLASS wc;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.hInstance = hInst;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_strClassName.c_str();
	wc.lpfnWndProc = WindowProc;

	//ウィンドウ２重確認
	if (FindWindow(
		m_strClassName.c_str(), 
		m_windowSettings.Title.c_str()) != NULL)
	{
		MessageBox(NULL, "すでに起動しています！", "2重起動確認", MB_OK);
		exit(EOF);
	}

	//ウィンドウ登録
	if (!RegisterClass(&wc)) 
	{
		MessageBox(NULL, "ウィンドウ生成に失敗", "エラー", MB_OK);
	}
}

//-------------------------------------------------------------
// ウィンドウ生成
// 引　数：HINSTANCE	インスタンスハンドル
// 　　　　const int	アプリケーション初期表示方法
//-------------------------------------------------------------
void CWindowSystem::WindowCreate(HINSTANCE hInst, const int nShowCmd)
{
	//ウィンドウ中央設定
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	RECT rect = { 0, 0, (LONG)m_windowSettings.Width, (LONG)m_windowSettings.Height };
	AdjustWindowRect(&rect, WS_CAPTION | WS_SYSMENU, false);
	int windowWidth = rect.right - rect.left;
	int windowHeight = rect.bottom - rect.top;
	int windowX = (screenWidth - windowWidth) / 2;
	int windowY = (screenHeight - windowHeight) / 2;

	//ウィンドウ生成
	m_hWnd = CreateWindow(
		m_strClassName.c_str(), 
		m_windowSettings.Title.c_str(),
		WS_CAPTION | WS_SYSMENU,
		windowX, 
		windowY,
		windowWidth, 
		windowHeight,
		NULL, 
		NULL,
		hInst,
		NULL
	);

	//生成失敗時
	if (!m_hWnd) 
	{
		MessageBox(NULL, "HWNDない", "エラー", MB_OK);
	}

	//インスタンスアプリケーション
	m_hInstance = hInst;

	//ウィンドウの表示
	ShowWindow(m_hWnd, nShowCmd);
	UpdateWindow(m_hWnd);
}

//-------------------------------------------------------------
// ウィンドウプロシージャ
// 引　数：HWND			ウィンドウハンドル
// 　　　　UINT			メッセージ
// 　　　　WPARAM		パラメータ
// 　　　　LPARAM		パラメータ
//-------------------------------------------------------------
LRESULT CWindowSystem::WindowProc(
	HWND hWnd,
	UINT uMsg, 
	WPARAM wParam,
	LPARAM lParam
)
{
	ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

	switch (uMsg) 
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
#ifndef _DEBUG
	case WM_SETCURSOR:
		ShowCursor(false);
		return 0;
#endif
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0L;
}

//-------------------------------------------------------------
// パラメータの読み込み
//-------------------------------------------------------------
void CWindowSystem::LoadFile()
{
	ifstream ifs("Resource/Data/Window/WindowSettings.txt");
	if (!ifs)
	{
		this->InitFile();
		this->LoadFile();
		return;
	}

	string str = "";
	vector<string> strAry;
	while (getline(ifs, str)) //説明文
	{
		if (getline(ifs, str)) //数値
		{
			strAry.push_back(str);
		}
	}

	m_windowSettings.Title			= strAry[0];
	m_windowSettings.Width			= (UINT)atoi(strAry[1].c_str());
	m_windowSettings.Height			= (UINT)atoi(strAry[2].c_str());
	m_windowSettings.isWindowMode	= (bool)atoi(strAry[3].c_str());

	ifs.close();
}

//-------------------------------------------------------------
// パラメータの読み込みができなかったとき用(初期化)
//-------------------------------------------------------------
void CWindowSystem::InitFile()
{
	ofstream ofs;
	ofs.open("Resource/Data/Window/WindowSettings.txt");

	ofs << "WindowTitle: (Default: DX9_GameLib)" << endl;
	ofs << "DX9_GameLib" << endl;
	ofs << "WindowWidth: (Default: 1280)" << endl;
	ofs << "1280" << std::endl;
	ofs << "WindowHeight: (Default: 720)" << endl;
	ofs << "720" << std::endl;
	ofs << "IsWindowMode: (Default: 1 => true | 0 => false)" << endl;
	ofs << "1" << std::endl;

	ofs.close();
}
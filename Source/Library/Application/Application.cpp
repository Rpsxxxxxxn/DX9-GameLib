//*************************************************************
//
// Application.cpp
//
// 全体のメインクラス
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// クラス定義
//=============================================================
#include "Application.h"
#include "../Utility/Singleton.h"

//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
CApplication::CApplication()
	: m_pClsWindowSystem(new CWindowSystem)
	, m_pClsSystem(new CSystem)
{
	CSceneManager::Create();
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CApplication::~CApplication()
{
	CSingletonFinalizer::Finalize();
}

//-------------------------------------------------------------
// 初期化
// 引　数：HINSTANCE	インスタンスハンドル
// 　　　　const int	アプリケーション初期表示方法
//-------------------------------------------------------------
void CApplication::Initialize(HINSTANCE hInst, const int nShowCmd)
{
	//ウィンドウ初期化
	m_pClsWindowSystem->Initialize(hInst, nShowCmd);

	//システムクラスの初期化
	m_pClsSystem->Initialize(m_pClsWindowSystem);

	//シーンオブジェクト管理の初期化
	CSceneManager::GetInstance()->Initialize(m_pClsSystem);
}

//-------------------------------------------------------------
// メインループ
//-------------------------------------------------------------
void CApplication::MainLoop()
{
	while (m_pClsWindowSystem->IsLoop())
	{
		if (!CSceneManager::GetInstance()->ChangeScene())
			break;

		Update();
		Render();
	}
}

//-------------------------------------------------------------
// 解放
//-------------------------------------------------------------
void CApplication::ShutDown()
{
	//システムクラスのシャットダウン
	m_pClsSystem->ShutDown();

	//ImGuiの終了
	ImGui_Start::ShutDown();

	//シーンオブジェクト管理の破棄
	CSceneManager::Destroy();

	//ウィンドウシステムクラスの解放
	SafeDelete(m_pClsWindowSystem);

	//システムクラスの解放
	SafeDelete(m_pClsSystem);
}

//-------------------------------------------------------------
// 更新
//-------------------------------------------------------------
void CApplication::Update()
{
	//毎フレームのカウンター
	m_pClsSystem->UpdateTimer();

	//ImGui用フレーム生成
	ImGui_Start::Begin();

	//システムクラスの更新
	m_pClsSystem->Update();

	//シーン管理の更新
	CSceneManager::GetInstance()->Update();
}

//-------------------------------------------------------------
// 描画
//-------------------------------------------------------------
void CApplication::Render()
{
	//バックバッファのクリア
	m_pClsSystem->Clear(0xFF000000);

	//レンダリング開始
	m_pClsSystem->BeginRender();

	//シーン管理の描画
	CSceneManager::GetInstance()->Draw();

	//レンダリングの終了
	m_pClsSystem->EndRender();

	//ImGuiレンダリング
	ImGui_Start::End();

	//バックバッファをフロントバッファに持ってくる
	m_pClsSystem->Present();
}

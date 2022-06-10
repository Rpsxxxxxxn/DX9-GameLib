//*************************************************************
//
// Imgui開始時の関数
//
// imgui_start.cpp
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "imgui_start.h"

//-------------------------------------------------------------
// ImGui_Init 初期化
// 引　数：HWND					ウィンドウハンドル
// 　　　　LPDIRECT3DDEVICE9	デバイスのポインタ
//-------------------------------------------------------------
void ImGui_Start::ImGui_Init(
	HWND hWnd,
	LPDIRECT3DDEVICE9 pDevice
)
{
#ifdef DEBUG_MODE
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF(
		"Include/ImGui/msgothic.ttc", 
		14.0f,
		nullptr,
		io.Fonts->GetGlyphRangesJapanese());
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);
#endif
}

//-------------------------------------------------------------
// ImGUIフレームの生成
//-------------------------------------------------------------
void ImGui_Start::Begin()
{
#ifdef DEBUG_MODE
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Text(u8"FPS： %.1f | DeltaTime： %.1f ms", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
#endif
}

//-------------------------------------------------------------
// 描画
//-------------------------------------------------------------
void ImGui_Start::End()
{
#ifdef DEBUG_MODE
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
#endif
}

//-------------------------------------------------------------
// ImGuiの終了
// 備　考：ImGuiの破棄、プログラム終了前に呼び出し
//-------------------------------------------------------------
void ImGui_Start::ShutDown()
{
#ifdef DEBUG_MODE
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif
}
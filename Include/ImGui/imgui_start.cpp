//*************************************************************
//
// Imgui�J�n���̊֐�
//
// imgui_start.cpp
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "imgui_start.h"

//-------------------------------------------------------------
// ImGui_Init ������
// ���@���FHWND					�E�B���h�E�n���h��
// �@�@�@�@LPDIRECT3DDEVICE9	�f�o�C�X�̃|�C���^
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
// ImGUI�t���[���̐���
//-------------------------------------------------------------
void ImGui_Start::Begin()
{
#ifdef DEBUG_MODE
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Text(u8"FPS�F %.1f | DeltaTime�F %.1f ms", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
#endif
}

//-------------------------------------------------------------
// �`��
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
// ImGui�̏I��
// ���@�l�FImGui�̔j���A�v���O�����I���O�ɌĂяo��
//-------------------------------------------------------------
void ImGui_Start::ShutDown()
{
#ifdef DEBUG_MODE
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif
}
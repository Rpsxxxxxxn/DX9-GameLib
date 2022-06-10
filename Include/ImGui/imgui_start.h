//*************************************************************
//
// Imgui�J�n���̊֐�
//
// imgui_start.h
//
//*************************************************************

#pragma once

//=============================================================
// �}�N����`
//=============================================================
#if defined(DEBUG) | defined(_DEBUG)
#define DEBUG_MODE 
#endif

//=============================================================
// �C���N���[�h
//=============================================================
#include <Windows.h>
#include <d3dx9.h>
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

//=============================================================
// ���O���
//=============================================================
namespace ImGui_Start
{
	//-------------------------------------------------------------
	// ImGui_Init ������
	// ���@���FHWND					�E�B���h�E�n���h��
	// �@�@�@�@LPDIRECT3DDEVICE9	�f�o�C�X�̃|�C���^
	//-------------------------------------------------------------
	void ImGui_Init(HWND hWnd, LPDIRECT3DDEVICE9 pDevice);

	//-------------------------------------------------------------
	// ImGUI�t���[���̐���
	//-------------------------------------------------------------
	void Begin();

	//-------------------------------------------------------------
	// �`��
	//-------------------------------------------------------------
	void End();

	//-------------------------------------------------------------
	// ImGui�̏I��
	// ���@�l�FImGui�̔j���A�v���O�����I���O�ɌĂяo��
	//-------------------------------------------------------------
	void ShutDown();
}

//-------------------------------------------------------------
// �}�E�X�A�L�[�{�[�h��Ή������邽�߂̃n���h��
// ���@���FHWND		�n���h��
// �@�@�@�@UINT		���b�Z�[�W
// �@�@�@�@WPARAM	�p�����[�^
// �@�@�@�@LPARAM	�p�����[�^
//-------------------------------------------------------------
extern IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(
	HWND hWnd, 
	UINT msg, 
	WPARAM wParam, 
	LPARAM lParam);
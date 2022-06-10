//*************************************************************
//
// DebugText.h
//
// Debug�p�N���X
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

#pragma once

//=============================================================
// �C���N���[�h
//=============================================================
#include "../Common.h"

//=============================================================
// �N���X��`
//=============================================================
class CDebugText
{
public:
	//-------------------------------------------------------------
	// �V�����E�B���h�E�̐���
	//-------------------------------------------------------------
	static bool NewWindowStart(string);
	static void NewWindowEnd();

	//-------------------------------------------------------------
	// �c���[�\��
	//-------------------------------------------------------------
	static bool TreeStart(string);
	static void TreeEnd();

	//-------------------------------------------------------------
	// �V�X�e���f�o�b�O�p
	//-------------------------------------------------------------
	static void DebugInfo(string);

	//-------------------------------------------------------------
	// �X���C�_�[���l
	//-------------------------------------------------------------
	static void SliderValue(string, Vector2*, float, float);
	static void SliderValue(string, Vector3*, float, float);
	static void SliderValue(string, Vector4*, float, float);

	//-------------------------------------------------------------
	// ���͐��l
	//-------------------------------------------------------------
	static void InputValue(string, Vector2*);
	static void InputValue(string, Vector3*);
	static void InputValue(string, Vector4*);

	//-------------------------------------------------------------
	// �h���b�O���l
	//-------------------------------------------------------------
	static void DragValue(string, Vector2*);
	static void DragValue(string, Vector3*);
	static void DragValue(string, Vector4*);
};
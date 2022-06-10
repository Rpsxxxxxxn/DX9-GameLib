//*************************************************************
//
// DebugText.cpp
//
// Debug�p�N���X
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "DebugText.h"

//=============================================================
// �N���X��`
//=============================================================

//-------------------------------------------------------------
// �V�����E�B���h�E�̐���
//-------------------------------------------------------------
bool CDebugText::NewWindowStart(string name)
{
#ifdef DEBUG_MODE
	return ImGui::Begin(name.c_str());
#else
	return false;
#endif
}

void CDebugText::NewWindowEnd()
{
#ifdef DEBUG_MODE
	ImGui::End();
#endif
}

//-------------------------------------------------------------
// �c���[�\��
//-------------------------------------------------------------
bool CDebugText::TreeStart(string name)
{
#ifdef DEBUG_MODE
	return ImGui::TreeNode(name.c_str());
#else
	return false;
#endif
}

void CDebugText::TreeEnd()
{
#ifdef DEBUG_MODE
	ImGui::TreePop();
#endif
}

//-------------------------------------------------------------
// �V�X�e���f�o�b�O�p
//-------------------------------------------------------------
void CDebugText::DebugInfo(string text)
{
#ifdef DEBUG_MODE
	CDebugText::NewWindowStart("DebugWindow");

	ImGui::Text(text.c_str());

	CDebugText::NewWindowEnd();
#endif
}

//-------------------------------------------------------------
// �X���C�_�[���l
//-------------------------------------------------------------
void CDebugText::SliderValue(string name, Vector2* val, float minVal, float maxVal)
{
#ifdef DEBUG_MODE
	float* v[2] = { &val->x, &val->y };
	ImGui::SliderFloat2(name.c_str(), *v, minVal, maxVal);
#endif
}

void CDebugText::SliderValue(string name, Vector3* val, float minVal, float maxVal)
{
#ifdef DEBUG_MODE
	float* v[3] = { &val->x,&val->y,&val->z };
	ImGui::SliderFloat3(name.c_str(), *v, minVal, maxVal);
#endif
}

void CDebugText::SliderValue(string name, Vector4* val, float minVal, float maxVal)
{
#ifdef DEBUG_MODE
	float* v[4] = { &val->x,&val->y,&val->z,&val->w };
	ImGui::SliderFloat4(name.c_str(), *v, minVal, maxVal);
#endif
}

//-------------------------------------------------------------
// ���͐��l
//-------------------------------------------------------------
void CDebugText::InputValue(string name, Vector2* val)
{
#ifdef DEBUG_MODE
	float* v[2] = { &val->x,&val->y };
	ImGui::InputFloat2(name.c_str(), *v);
#endif
}

void CDebugText::InputValue(string name, Vector3* val)
{
#ifdef DEBUG_MODE
	float* v[3] = { &val->x,&val->y,&val->z };
	ImGui::InputFloat3(name.c_str(), *v);
#endif
}

void CDebugText::InputValue(string name, Vector4* val)
{
#ifdef DEBUG_MODE
	float* v[4] = { &val->x,&val->y,&val->z,&val->w };
	ImGui::InputFloat4(name.c_str(), *v);
#endif
}

//-------------------------------------------------------------
// �h���b�O���l
//-------------------------------------------------------------
void CDebugText::DragValue(string name, Vector2* val)
{
#ifdef DEBUG_MODE
	float* v[2] = { &val->x,&val->y };
	ImGui::DragFloat2(name.c_str(), *v);
#endif
}

void CDebugText::DragValue(string name, Vector3* val)
{
#ifdef DEBUG_MODE
	float* v[3] = { &val->x,&val->y,&val->z };
	ImGui::DragFloat3(name.c_str(), *v);
#endif
}

void CDebugText::DragValue(string name, Vector4* val)
{
#ifdef DEBUG_MODE
	float* v[4] = { &val->x,&val->y,&val->z,&val->w };
	ImGui::DragFloat4(name.c_str(), *v);
#endif
}

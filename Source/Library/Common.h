//*************************************************************
//
// �V�X�e���̂��߂̃��C�u�����܂Ƃ�
//
// Common.h
//
//*************************************************************

#pragma once

//=============================================================
// �}�N����`
//=============================================================
#define	DIRECTINPUT_VERSION	0x0800

//=============================================================
// ���C�u����
//=============================================================
#pragma comment(lib,"d3d9.lib") 
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")

//=============================================================
// �C���N���[�h
//=============================================================
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <dinput.h>
#include <Windows.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <vector>
#include <memory>
#include <time.h>
#include <random>
#include <algorithm>
#include <functional>
#include <xaudio2.h>
#include <mmsystem.h>
#include <chrono>
#include <mutex>
#include <imgui_start.h>

//=============================================================
// ���O���
//=============================================================
using namespace std;

//=============================================================
// ���O�̕ύX
//=============================================================
typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;
typedef D3DXVECTOR4 Vector4;
typedef D3DXMATRIX  Matrix;
typedef D3DXQUATERNION Quaternion;

//=============================================================
// ���S�J���e���v���[�g
//=============================================================
template <typename T>
inline void SafeDelete(T*& p) {
	if (p != nullptr) {
		delete (p);
		(p) = nullptr;
	}
}

template <typename T>
inline void SafeDeleteArray(T*& p) {
	if (p != nullptr) {
		delete[](p);
		(p) = nullptr;
	}
}

template <typename T>
inline void SafeRelease(T*& p) {
	if (p != nullptr) {
		(p)->Release();
		(p) = nullptr;
	}
}

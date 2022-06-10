//*************************************************************
//
// Window.cpp
//
// �E�B���h�E�Ǘ�
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "Window.h"

//=============================================================
// �N���X��`
//=============================================================

//-------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------
CWindowSystem::CWindowSystem()
	: m_strClassName("DX9-GameLib")
{
	this->LoadFile();
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
CWindowSystem::~CWindowSystem() {}

//-------------------------------------------------------------
// ������
// ���@���FHINSTANCE	�C���X�^���X�n���h��
// �@�@�@�@const int	�A�v���P�[�V���������\�����@
//-------------------------------------------------------------
void CWindowSystem::Initialize(HINSTANCE hInst, const int nShowCmd)
{
	if (MessageBox(nullptr, "�t���X�N���[�����[�h�ŋN�����܂����H", "�N���m�F", MB_YESNO) == IDYES)
	{
		m_windowSettings.Width = GetSystemMetrics(SM_CXSCREEN);
		m_windowSettings.Height = GetSystemMetrics(SM_CYSCREEN);
		m_windowSettings.isWindowMode = false;
	}

	//�E�B���h�E�o�^
	this->WindowRegister(hInst, nShowCmd);

	//�E�B���h�E����
	this->WindowCreate(hInst, nShowCmd);
}

//-------------------------------------------------------------
// �E�B���h�E����������Ă��邩
// �߂�l�FBOOL �������FALSE
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
// �E�B���h�E�ݒ�̎擾
// �߂�l�FWindowSettings*	�E�B���h�E�ݒ�
//-------------------------------------------------------------
WindowSettings CWindowSystem::GetWindowSettings() const
{
	return m_windowSettings;
}

//-------------------------------------------------------------
// �E�B���h�E�n���h���̎擾
// �߂�l�FHWND	�E�B���h�E�n���h��
//-------------------------------------------------------------
HWND CWindowSystem::GetHWND() const
{
	return m_hWnd;
}

//-------------------------------------------------------------
// �C���X�^���X�A�v���P�[�V�����̎擾
//-------------------------------------------------------------
HINSTANCE CWindowSystem::GetInstApp()
{
	return m_hInstance;
}

//-------------------------------------------------------------
// �E�B���h�E�o�^
// ���@���FHINSTANCE	�C���X�^���X�n���h��
// �@�@�@�@const int	�A�v���P�[�V���������\�����@
//-------------------------------------------------------------
void CWindowSystem::WindowRegister(HINSTANCE hInst, const int nShowCmd)
{
	//�E�B���h�E�N���X
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

	//�E�B���h�E�Q�d�m�F
	if (FindWindow(
		m_strClassName.c_str(), 
		m_windowSettings.Title.c_str()) != NULL)
	{
		MessageBox(NULL, "���łɋN�����Ă��܂��I", "2�d�N���m�F", MB_OK);
		exit(EOF);
	}

	//�E�B���h�E�o�^
	if (!RegisterClass(&wc)) 
	{
		MessageBox(NULL, "�E�B���h�E�����Ɏ��s", "�G���[", MB_OK);
	}
}

//-------------------------------------------------------------
// �E�B���h�E����
// ���@���FHINSTANCE	�C���X�^���X�n���h��
// �@�@�@�@const int	�A�v���P�[�V���������\�����@
//-------------------------------------------------------------
void CWindowSystem::WindowCreate(HINSTANCE hInst, const int nShowCmd)
{
	//�E�B���h�E�����ݒ�
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	RECT rect = { 0, 0, (LONG)m_windowSettings.Width, (LONG)m_windowSettings.Height };
	AdjustWindowRect(&rect, WS_CAPTION | WS_SYSMENU, false);
	int windowWidth = rect.right - rect.left;
	int windowHeight = rect.bottom - rect.top;
	int windowX = (screenWidth - windowWidth) / 2;
	int windowY = (screenHeight - windowHeight) / 2;

	//�E�B���h�E����
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

	//�������s��
	if (!m_hWnd) 
	{
		MessageBox(NULL, "HWND�Ȃ�", "�G���[", MB_OK);
	}

	//�C���X�^���X�A�v���P�[�V����
	m_hInstance = hInst;

	//�E�B���h�E�̕\��
	ShowWindow(m_hWnd, nShowCmd);
	UpdateWindow(m_hWnd);
}

//-------------------------------------------------------------
// �E�B���h�E�v���V�[�W��
// ���@���FHWND			�E�B���h�E�n���h��
// �@�@�@�@UINT			���b�Z�[�W
// �@�@�@�@WPARAM		�p�����[�^
// �@�@�@�@LPARAM		�p�����[�^
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
// �p�����[�^�̓ǂݍ���
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
	while (getline(ifs, str)) //������
	{
		if (getline(ifs, str)) //���l
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
// �p�����[�^�̓ǂݍ��݂��ł��Ȃ������Ƃ��p(������)
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
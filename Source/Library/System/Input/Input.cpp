//*************************************************************
//
// Input.cpp
//
// ���̓N���X
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "Input.h"

//-------------------------------------------------------------
// �V���O���g������
//-------------------------------------------------------------
CInput * CInput::Create()
{
	static CInput instance;
	return &instance;
}

//-------------------------------------------------------------
// ������
// ���@���FCWindowSystem*	�E�B���h�E�V�X�e���N���X�̃|�C���^
//-------------------------------------------------------------
void CInput::Initialize(CWindowSystem * pWindowSystem)
{
	//�E�B���h�E�n���h���̎擾
	HWND hWnd = pWindowSystem->GetHWND();

	HINSTANCE hInst = pWindowSystem->GetInstApp();

	HRESULT hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
	if (FAILED(hr)) MessageBox(nullptr, "�f�o�C�X�쐬�Ɏ��s", "�G���[", MB_OK);

	//�L�[�{�[�h�̏�����
	InitializeKeyboard(hWnd);

	//�}�E�X�̏�����
	InitializeMouse(hWnd);

	//�W���C�X�e�B�b�N�̏�����
	InitializeJoyStick(hWnd);
}

//-------------------------------------------------------------
// �X�V
//-------------------------------------------------------------
void CInput::Update()
{
	HRESULT hr;

	memcpy(m_keyOldBuf, m_keyNowBuf, sizeof(m_keyNowBuf));
	ZeroMemory(m_keyNowBuf, sizeof(m_keyNowBuf));

	hr = m_pKeyBoard->GetDeviceState(sizeof(m_keyNowBuf), m_keyNowBuf);
	if (FAILED(hr)) while (m_pKeyBoard->Acquire() == DIERR_INPUTLOST);

	hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState);
	if (FAILED(hr)) while (m_pMouse->Acquire() == DIERR_INPUTLOST);

	UpdateJoyStick();
}

//-------------------------------------------------------------
// ���
//-------------------------------------------------------------
void CInput::Release()
{
}

//-------------------------------------------------------------
// �L�[���͏��̎擾
// ���@���FBYTE	�L�[�R�[�h
// ���@�l�F�L�[�̂������ςȂ�
//-------------------------------------------------------------
bool CInput::IsKeyDown(const BYTE keyCode)
{
	return m_keyNowBuf[keyCode] == 0x80 ? true : false;
}

//-------------------------------------------------------------
// �L�[���͏��̎擾
// ���@���FBYTE	�L�[�R�[�h
// ���@�l�F�L�[�̈�x�̂�
//-------------------------------------------------------------
bool CInput::IsKeyPressed(const BYTE keyCode)
{
	if (m_keyNowBuf[keyCode] == 0x80)
	{
		//�O��̃L�[�t���O��1����Ȃ��������
		if (m_keyOldBuf[keyCode] != 0x80)
		{
			return true;
		}
	}

	return false;
}

//-------------------------------------------------------------
// �L�[���͏��̎擾
// ���@���FBYTE	�L�[�R�[�h
// ���@�l�F�L�[�̂������ςȂ�
//-------------------------------------------------------------
bool CInput::IsMouseDown(const BYTE mouseButton)
{
	return m_mouseState.rgbButtons[mouseButton] == 0x80 ? true : false;
}

//-------------------------------------------------------------
// �L�[���͏��̎擾
// ���@���FBYTE	�L�[�R�[�h
// ���@�l�F�L�[�̈�x�̂�
//-------------------------------------------------------------
bool CInput::IsMousePressed(const BYTE mouseButton)
{
	return false;
}

//-------------------------------------------------------------
// �}�E�X�̈ړ���
// �߂�l�FPOINT	X,Y
//-------------------------------------------------------------
POINT CInput::GetMouseMove() const
{
	POINT p;

	p.x = m_mouseState.lX;
	p.y = m_mouseState.lY;

	return p;
}

//-------------------------------------------------------------
// �}�E�X�̃z�C�[��
// �߂�l�Fint		�z�C�[��
//-------------------------------------------------------------
int CInput::GetMouseWheel() const
{
	return m_mouseState.lZ;
}

//-------------------------------------------------------------
// �p�b�h�̎擾
// ���@���Fint	�L�[�R�[�h
// ���@�l�F�L�[�̂������ςȂ�
//-------------------------------------------------------------
bool CInput::IsPadDown(const int padButton)
{
	return m_joyPad[padButton];
}

//-------------------------------------------------------------
// �p�b�h�̎擾
// ���@���Fint	�L�[�R�[�h
// ���@�l�F�L�[�̈�x�̂�
//-------------------------------------------------------------
bool CInput::IsPadPressed(const int padButton)
{
	if (m_joyPad[padButton])
	{
		if (m_joyPad[padButton] != m_oldJoyPad[padButton])
		{
			return true;
		}
	}
	return false;
}

//-------------------------------------------------------------
// �W���C�X�e�B�b�N�̎擾
// ���@���Fint	�L�[�R�[�h
// ���@�l�F�L�[�̈�x�̂�
//-------------------------------------------------------------
float CInput::GetJoyStickState(const int joyStick)
{
	return m_joyStick[joyStick];
}

//-------------------------------------------------------------
// �R���g���[���[���ڑ�����Ă��邩�H
//-------------------------------------------------------------
bool CInput::IsControllerConnect()
{
	return m_pJoystick != nullptr;
}

//-------------------------------------------------------------
// �L�[�{�[�h������
//-------------------------------------------------------------
HRESULT CInput::InitializeKeyboard(HWND hWnd)
{
	HRESULT hr;

	hr = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, NULL);
	if (FAILED(hr)) MessageBox(nullptr, "�L�[�{�[�h�쐬�Ɏ��s", "�G���[", MB_OK);

	hr = m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) MessageBox(nullptr, "�t�H�[�}�b�g�̐ݒ�Ɏ��s", "�G���[", MB_OK);

	hr = m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr)) MessageBox(nullptr, "�������x���̐ݒ�Ɏ��s", "�G���[", MB_OK);

	memset(m_keyNowBuf, 0, sizeof(m_keyNowBuf));
	memset(m_keyOldBuf, 0, sizeof(m_keyOldBuf));

	m_pKeyBoard->Acquire();

	return S_OK;
}

//-------------------------------------------------------------
// �}�E�X������
// ���@���FHWND	�E�B���h�E�n���h��
//-------------------------------------------------------------
HRESULT CInput::InitializeMouse(HWND hWnd)
{
	HRESULT hr;

	hr = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	if (FAILED(hr)) MessageBox(nullptr, "�f�o�C�X�쐬�Ɏ��s", "�G���[", MB_OK);

	hr = m_pMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr)) MessageBox(nullptr, "�t�H�[�}�b�g�̐ݒ�Ɏ��s", "�G���[", MB_OK);

	hr = m_pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr)) MessageBox(nullptr, "�������x���̐ݒ�Ɏ��s", "�G���[", MB_OK);

	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = DIPROPAXISMODE_REL;

	hr = m_pMouse->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
	if (FAILED(hr)) MessageBox(nullptr, "�f�o�C�X�̐ݒ�Ɏ��s", "�G���[", MB_OK);

	m_pMouse->Acquire();

	return S_OK;
}

//-------------------------------------------------------------
// �W���C�X�e�B�b�N������
// ���@���FHWND	�E�B���h�E�n���h��
//-------------------------------------------------------------
HRESULT CInput::InitializeJoyStick(HWND hWnd)
{
	HRESULT hr;

	hr = m_pDirectInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, (VOID*)this, DIEDFL_ATTACHEDONLY);

	if (m_pJoystick == nullptr) return E_FAIL;

	hr = m_pJoystick->SetDataFormat(&c_dfDIJoystick);
	if (FAILED(hr)) MessageBox(hWnd, "�t�H�[�}�b�g���ݒ�ł��܂���", "�G���[", MB_OK);

	hr = m_pJoystick->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr)) MessageBox(hWnd, "�������x���̐ݒ�Ɏ��s.", "�G���[", MB_OK);

	hr = m_pJoystick->EnumObjects(EnumAxesCallback, (VOID*)this, DIDFT_AXIS);
	if (FAILED(hr)) MessageBox(hWnd, "�v���p�e�B�̐ݒ�Ɏ��s", "�G���[", MB_OK);

	if (FAILED(m_pJoystick->Poll()))
	{
		HRESULT hr = m_pJoystick->Acquire();

		while (hr == DIERR_INPUTLOST)
			hr = m_pJoystick->Acquire();
	}

	return S_OK;
}

//-------------------------------------------------------------
// �W���C�X�e�B�b�N�X�V
//-------------------------------------------------------------
bool CInput::UpdateJoyStick()
{
	HRESULT hr;

	if (nullptr == m_pJoystick) return FALSE;

	memcpy(m_oldJoyPad, m_joyPad, sizeof(m_joyPad));

	hr = m_pJoystick->Poll();
	if (FAILED(hr)) return FALSE;

	hr = m_pJoystick->GetDeviceState(sizeof(DIJOYSTATE), &m_joyState);
	if (FAILED(hr)) while (m_pJoystick->Acquire() == DIERR_INPUTLOST);

	for (int i = 0; i < KeyConfig::TRACKPAD; i++)
	{
		m_joyPad[i] = (bool)(m_joyState.rgbButtons[i] & 0x80);
	}
	
	m_joyStick[KeyConfig::LEFTHOR ] = (float)m_joyState.lX / 1000.0f;
	m_joyStick[KeyConfig::LEFTVER ] = (float)m_joyState.lY / 1000.0f;
	m_joyStick[KeyConfig::RIGHTHOR] = (float)m_joyState.lZ / 1000.0f;
	m_joyStick[KeyConfig::RIGHTVER] = (float)m_joyState.lRz / 1000.0f;

	return TRUE;
}

//-------------------------------------------------------------
// �W���C�X�e�B�b�N�p�b�h�̃R�[���o�b�N�֐�
//-------------------------------------------------------------
BOOL CInput::EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, void* pContext)
{
	HRESULT hr;

	CInput* pThis = (CInput*)pContext;

	hr = pThis->m_pDirectInput->CreateDevice(pdidInstance->guidInstance, &pThis->m_pJoystick, NULL);
	if (FAILED(hr)) return DIENUM_CONTINUE;

	return DIENUM_STOP;
}

//-------------------------------------------------------------
// �W���C�X�e�B�b�N���̃R�[���o�b�N�֐�
//-------------------------------------------------------------
BOOL CInput::EnumAxesCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, void* pContext)
{
	HRESULT hr;

	CInput* pThis = (CInput*)pContext;

	DIPROPRANGE diprg;
	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = 0 - 1000;
	diprg.lMax = 0 + 1000;

	hr = pThis->m_pJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr)) return DIENUM_STOP;

	return DIENUM_CONTINUE;
}

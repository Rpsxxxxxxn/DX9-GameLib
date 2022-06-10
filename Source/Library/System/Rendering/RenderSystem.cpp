//*************************************************************
//
// RenderSystem.cpp
//
// DirectX9�������p�N���X
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "RenderSystem.h"

//-------------------------------------------------------------
// �V���O���g������
//-------------------------------------------------------------
CRenderSystem * CRenderSystem::Create()
{
	static CRenderSystem instance;
	return &instance;
}

//-------------------------------------------------------------
// ������
// ���@���FCWindowSystem	�E�B���h�E�V�X�e���N���X�̃|�C���^
//-------------------------------------------------------------
void CRenderSystem::Initialize(CWindowSystem* pWindowSystem)
{
	//�E�B���h�E�n���h���̎擾
	auto hWnd = pWindowSystem->GetHWND();

	//�E�B���h�E�ݒ�̎擾
	auto ws_ = pWindowSystem->GetWindowSettings();

	//SDK�o�[�W�����̐ݒ�
	m_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);

	//DirectX�p�����[�^�ݒ�p�N���X
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth			= ws_.Width;
	d3dpp.BackBufferHeight			= ws_.Height;
	d3dpp.BackBufferFormat			= D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount			= 1;
	d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality		= 0;
	d3dpp.SwapEffect				= D3DSWAPEFFECT_FLIP;
	d3dpp.hDeviceWindow				= NULL;
	d3dpp.Windowed					= ws_.isWindowMode;
	d3dpp.EnableAutoDepthStencil	= TRUE;
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;
	d3dpp.Flags						= 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;

	// �f�o�C�X�̍쐬
	if (FAILED(m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice)))
		if (FAILED(m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice)))
			if (FAILED(m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice)))
				if (FAILED(m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice)))
					MessageBox(hWnd, "�f�o�C�X�̐����Ɏ��s���܂����B", "ERROR", MB_OK);

	ImGui_Start::ImGui_Init(hWnd ,m_pDevice);
}

//-------------------------------------------------------------
// ���
//-------------------------------------------------------------
void CRenderSystem::Release()
{
	SafeRelease(m_pDevice);
	SafeRelease(m_pDirect3D);
}

//-------------------------------------------------------------
// �f�o�C�X�̎擾
// �߂�l�Fconst LPDIRECT3DDEVICE9	�f�o�C�X�̃|�C���^
//-------------------------------------------------------------
LPDIRECT3DDEVICE9 CRenderSystem::GetDevice() const
{
	return m_pDevice;
}

//-------------------------------------------------------------
// �o�b�N�o�b�t�@�̃N���A
// ���@���Fconst DWORD	�F�̎w��
//-------------------------------------------------------------
void CRenderSystem::Clear(const DWORD value)
{
	m_pDevice->Clear(
		0, 
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		value,
		1.0f,
		0);
}

//-------------------------------------------------------------
// �����_�����O�J�n
//-------------------------------------------------------------
void CRenderSystem::BeginRender()
{
	m_pDevice->BeginScene();
}

//-------------------------------------------------------------
// �����_�����O�I��
//-------------------------------------------------------------
void CRenderSystem::EndRender()
{
	m_pDevice->EndScene();
}

//-------------------------------------------------------------
// ��ʃX���b�v
//-------------------------------------------------------------
void CRenderSystem::Present()
{
	m_pDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

//-------------------------------------------------------------
// �X�v���C�g�����_�����O�J�n
// ���@�l�F�`��O�ɌĂяo����K��
//-------------------------------------------------------------
void CRenderSystem::SpriteBegin()
{
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

//-------------------------------------------------------------
// �X�v���C�g�����_�����O�I��
// ���@�l�F�`���ɌĂяo����K��
//-------------------------------------------------------------
void CRenderSystem::SpriteClose()
{
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

//-------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------
CRenderSystem::CRenderSystem()
	: m_pDevice(nullptr)
	, m_pDirect3D(nullptr)
{
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
CRenderSystem::~CRenderSystem()
{
	this->Release();
}

//*************************************************************
//
// System.cpp
//
// �t���[�����[�N�̃��C���V�X�e��
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "System.h"

//=============================================================
// �N���X��`
//=============================================================

//-------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------
CSystem::CSystem()
{
	//�f�o�b�O���O�N���X�̐���
	m_pClsDebugLog = CDebugLog::Create();

	//�����_�����O�N���X�̐���
	m_pClsRenderSystem = CRenderSystem::Create();

	//�t�@�C���Ǘ��N���X�̐���
	m_pClsFileManager = CFileManager::Create();

	//���̓N���X�̐���
	m_pClsInput = CInput::Create();

	//�^�C�}�[�N���X�̐���
	m_pClsTimer = CTimer::Create();

	//�I�[�f�B�I�N���X�̐���
	m_pClsAudio = CAudio::Create();

	//�摜�`��̐���
	m_pClsSprite = CSprite::Create();

	//�G�t�F�N�g�N���X�̐���
	m_pClsEffectSystem = CEffectSystem::Create();
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
CSystem::~CSystem() {}

//-------------------------------------------------------------
// ������
// ���@���FCWindowSystem*	�E�B���h�E�V�X�e���N���X�̃|�C���^
//-------------------------------------------------------------
void CSystem::Initialize(CWindowSystem* pWindowSystem)
{
	//�E�B���h�E�n���h���̎擾
	m_hWnd = pWindowSystem->GetHWND();

	//�E�B���h�E�ݒ�̃|�C���^�n��
	m_pWindowSettings = pWindowSystem->GetWindowSettings();

	//�����_�����O�N���X�̏�����
	m_pClsRenderSystem->Initialize(pWindowSystem);

	//�t�@�C���Ǘ��N���X�̏�����
	m_pClsFileManager->Initialize(m_pClsRenderSystem->GetDevice());

	//���̓N���X�̏�����
	m_pClsInput->Initialize(pWindowSystem);

	//�G�t�F�N�g�N���X�̏�����
	m_pClsEffectSystem->Initialize(m_pClsRenderSystem->GetDevice(), 2000);

	//�I�[�f�B�I�N���X�̏�����
	m_pClsAudio->Initialize();

	//�`��N���X�̏�����
	m_pClsSprite->Initialize(m_pClsRenderSystem->GetDevice());

	//�V�F�[�_�Ǘ��̏�����
	CShaderManager::GetInstance().Initialize(m_pClsRenderSystem->GetDevice(), pWindowSystem);

	//�r���[�|�[�g�̏�����
	InitializeViewPort();
}

//-------------------------------------------------------------
// �X�V
//-------------------------------------------------------------
void CSystem::Update()
{
	m_pClsInput->Update();
	m_pClsAudio->Update();
	m_pClsDebugLog->Update();
}

//-------------------------------------------------------------
// ���
//-------------------------------------------------------------
void CSystem::ShutDown()
{
	m_pClsRenderSystem->Release();
	m_pClsFileManager->Release();
	m_pClsInput->Release();
}

//*************************************************************
//
// �t�@�C���Ǘ�
//
//*************************************************************

//-------------------------------------------------------------
// XFile�t�@�C���ǂݍ���
// ���@���Fconst int 	�t�@�C���̌Œ�ԍ�
// �@�@�@�@const string	�t�@�C����
//-------------------------------------------------------------
void CSystem::LoadXFile(const int index, const string strFileName)
{
	m_pClsFileManager->LoadXFile(index, strFileName);
}

//-------------------------------------------------------------
// Texture�t�@�C���ǂݍ���
// ���@���Fconst int 	�t�@�C���̌Œ�ԍ�
// �@�@�@�@const string	�t�@�C����
//-------------------------------------------------------------
void CSystem::LoadTexture(const int index, const string strFileName)
{
	m_pClsFileManager->LoadTexture(index, strFileName);
}

//-------------------------------------------------------------
// �t�@�C���̉��
//-------------------------------------------------------------
void CSystem::ReleaseAllFile()
{
	m_pClsFileManager->Release();
}

//-------------------------------------------------------------
// ���[�h�����t�@�C���̉��(�ȃ�����)
// ���@���Fconst int 	�t�@�C���̌Œ�ԍ�
//-------------------------------------------------------------
void CSystem::EraseData(const int index)
{
	m_pClsFileManager->EraseData(index);
}

//-------------------------------------------------------------
// �e�N�X�`���̃|�C���^�擾
// �߂�l�FLPDIRECT3DTEXTURE9	�e�N�X�`��
//-------------------------------------------------------------
LPDIRECT3DTEXTURE9 CSystem::GetTexture(const int index)
{
	return m_pClsFileManager->GetTexture(index);
}

//-------------------------------------------------------------
// �e�N�X�`���\���̂̎擾
// �߂�l�FTEXTURE_DATA*		�e�N�X�`��
//-------------------------------------------------------------
TEXTURE_DATA * CSystem::GetTextureData(const int index)
{
	return m_pClsFileManager->GetTextureData(index);
}

//-------------------------------------------------------------
// XFile�N���X�̃|�C���^�擾
// �߂�l�FCXFile*	XFile���f���N���X
//-------------------------------------------------------------
CXFile * CSystem::GetXFile(const int index)
{
	return m_pClsFileManager->GetXFile(index);
}

//-------------------------------------------------------------
// �f�o�C�X�̎擾
// �߂�l�Fconst IDirect3DDevice9*	�f�o�C�X�̃|�C���^
//-------------------------------------------------------------
LPDIRECT3DDEVICE9 CSystem::GetDevice() const
{
	return m_pClsRenderSystem->GetDevice();
}

//*************************************************************
//
// �I�[�f�B�I�V�X�e��
//
//*************************************************************

//-------------------------------------------------------------
// Bgm�t�@�C���ǂݍ���
// ���@���Fconst int 	�t�@�C���̌Œ�ԍ�
// �@�@�@�@const string	�t�@�C����
//-------------------------------------------------------------
void CSystem::AddBgm(const int index, const string strFileName)
{
	m_pClsAudio->AddBgm(index, strFileName);
}

//-------------------------------------------------------------
// Se�t�@�C���ǂݍ���
// ���@���Fconst int 	�t�@�C���̌Œ�ԍ�
// �@�@�@�@const string	�t�@�C����
//-------------------------------------------------------------
void CSystem::AddSe(const int index, const string strFileName)
{
	m_pClsAudio->AddSe(index, strFileName);
}

//-------------------------------------------------------------
// Bgm�̍Đ�
// ���@���Fconst string	�ǉ��������O
//-------------------------------------------------------------
void CSystem::PlayBgm(const int index)
{
	m_pClsAudio->PlayBgm(index);
}

//-------------------------------------------------------------
// Se�̍Đ�
// ���@���Fconst string	�ǉ��������O
//-------------------------------------------------------------
void CSystem::PlaySe(const int index)
{
	m_pClsAudio->PlaySe(index);
}

//-------------------------------------------------------------
// Bgm�̒�~
// ���@���Fconst string	�ǉ��������O
//-------------------------------------------------------------
void CSystem::StopBgm(const int index)
{
	m_pClsAudio->StopBgm(index);
}

//-------------------------------------------------------------
// �����߂�
// ���@���Fconst string	�ǉ�����Bgm��
//-------------------------------------------------------------
void CSystem::RewindBgm(const int index)
{
	m_pClsAudio->RewindBgm(index);
}

//-------------------------------------------------------------
// �t�@�C���̉��
//-------------------------------------------------------------
void CSystem::ReleaseAllSound()
{
	m_pClsAudio->Release();
}

//-------------------------------------------------------------
// �{�����[���̐ݒ�
// ���@���Ffloat	�{�����[���̐ݒ�
//-------------------------------------------------------------
void CSystem::SetVolume(float volume)
{
	m_pClsAudio->SetVolume(volume);
}

//*************************************************************
//
// �摜�`��Ɋւ���
//
//*************************************************************

//-------------------------------------------------------------
// �`��
// ���@���FRECT*				�\�[�X�摜�̃}�X�N
// �@�@�@�@LPDIRECT3DTEXTURE9	�e�N�X�`���̃|�C���^
// �@�@�@�@RECT*				�`���̈ʒu
// �@�@�@�@DWORD				�����x
//-------------------------------------------------------------
void CSystem::DrawSprite(
	RECT * pSrcRect,
	LPDIRECT3DTEXTURE9 pTexture,
	RECT * pDstRect,
	DWORD dwColor,
	float angle
)
{
	m_pClsSprite->
		Draw(pSrcRect, pTexture, pDstRect, dwColor, angle);
}

//-------------------------------------------------------------
// �X�v���C�g�����_�����O�J�n
// ���@�l�F�`��O�ɌĂяo����K��
//-------------------------------------------------------------
void CSystem::SpriteBegin()
{
	//�����_�����O�ݒ�
	m_pClsRenderSystem->SpriteBegin();

	//�`��J�n
	m_pClsSprite->SpriteBegin();
}

//-------------------------------------------------------------
// �X�v���C�g�����_�����O�I��
// ���@�l�F�`���ɌĂяo����K��
//-------------------------------------------------------------
void CSystem::SpriteClose()
{
	//�`��I��
	m_pClsSprite->SpriteClose();

	//�����_�����O�ݒ�̏�����
	m_pClsRenderSystem->SpriteClose();
}

//*************************************************************
//
// �G�t�F�N�g�V�X�e��
//
//*************************************************************

//-------------------------------------------------------------
// �f�[�^�̐ݒ�
// ���@���Fconst float ����p�i���W�A���p�j
// �@�@�@�@const float �A�X�y�N�g�� (width / height)
// �@�@�@�@const float �O���N���b�v��
// �@�@�@�@const float ����N���b�v��
//-------------------------------------------------------------
void CSystem::InitProjection(
	const float fov,
	const float aspect,
	const float nearClip, 
	const float farClip
)
{
	m_pClsEffectSystem->
		InitProjection(fov, aspect, nearClip, farClip);
}

//-------------------------------------------------------------
// �G�t�F�N�g�̒ǉ�
// ���@���FEFK_CHAR*	�t�@�C����
//-------------------------------------------------------------
void CSystem::AddEffect(const EFK_CHAR * strFileName)
{
	m_pClsEffectSystem->AddEffect(strFileName);
}

//-------------------------------------------------------------
// �G�t�F�N�g�̒ǉ�
// ���@���Fconst int	�z��̔Ԗ�
// �@�@�@�@EFK_CHAR*	�t�@�C����
//-------------------------------------------------------------
void CSystem::AddEffect(const int index, const EFK_CHAR* strFileName)
{
	m_pClsEffectSystem->AddEffect(index, strFileName);
}

//-------------------------------------------------------------
// �G�t�F�N�g�̍X�V
// ���@���FCCamera*		�J�����̃|�C���^
//-------------------------------------------------------------
void CSystem::UpdateEffect(CCamera * pCamera)
{
	m_pClsEffectSystem->Update(pCamera);
}

//-------------------------------------------------------------
// �G�t�F�N�g�̍Đ�
// ���@���Fconst int	�z��̔Ԗ�
// �@�@�@�@D3DXVECTOR3	3�����x�N�g��
//-------------------------------------------------------------
void CSystem::PlayEffect(const int index, D3DXVECTOR3& VecPos)
{
	m_pClsEffectSystem->Play(index, VecPos);
}

//-------------------------------------------------------------
// �G�t�F�N�g�̍Đ�
// ���@���Fconst int	�z��̔Ԗ�
// �@�@�@�@D3DXVECTOR3*	3�����x�N�g���|�C���^
//-------------------------------------------------------------
void CSystem::PlayEffect(const int index, D3DXVECTOR3* pVecPos)
{
	m_pClsEffectSystem->Play(index, pVecPos);
}

//-------------------------------------------------------------
// �J�������W�̍X�V
// ���@���FCamera*	�J�����N���X�̃|�C���^
//-------------------------------------------------------------
void CSystem::SetCameraMatrix(CCamera * pCamera)
{
	m_pClsEffectSystem->Update(pCamera);
}

//-------------------------------------------------------------
// �G�t�F�N�g�`��
//-------------------------------------------------------------
void CSystem::DrawEffect()
{
	m_pClsEffectSystem->Draw();
}

//*************************************************************
//
// �����_�����O�V�X�e��
//
//*************************************************************

//-------------------------------------------------------------
// �o�b�N�o�b�t�@�̃N���A
// ���@���Fconst DWORD	�F�̎w��
//-------------------------------------------------------------
void CSystem::Clear(const DWORD value)
{
	m_pClsRenderSystem->Clear(value);
}

//-------------------------------------------------------------
// �����_�����O�J�n
//-------------------------------------------------------------
void CSystem::BeginRender()
{
	m_pClsRenderSystem->BeginRender();
}

//-------------------------------------------------------------
// �����_�����O�I��
//-------------------------------------------------------------
void CSystem::EndRender()
{
	m_pClsRenderSystem->EndRender();
}

//-------------------------------------------------------------
// ��ʃX���b�v
//-------------------------------------------------------------
void CSystem::Present()
{
	m_pClsRenderSystem->Present();
}

//-------------------------------------------------------------
// �V�F�[�_�̕`��ݒ�
// ���@���FShaderType �V�F�[�_�̎��
// ���@�l�F���b�V���̕`��O�ɌĂяo�������Ă��������B
//-------------------------------------------------------------
void CSystem::SetShaderState(const DWORD value)
{
	CShaderManager::GetInstance().SetShaderState((Shader)value);
}

//*************************************************************
//
// �E�B���h�E�ݒ�
//
//*************************************************************

//-------------------------------------------------------------
// �E�B���h�E�ݒ�̎擾
// �߂�l�Fconst WindowSettings*	�E�B���h�E�\���̂̃|�C���^
//-------------------------------------------------------------
WindowSettings CSystem::GetWindowSettings() const
{
	return m_pWindowSettings;
}

//*************************************************************
//
// �C���v�b�g
//
//*************************************************************

//-------------------------------------------------------------
// �L�[���͏��̎擾
// ���@���FBYTE	�L�[�R�[�h
// ���@�l�F�L�[�̂������ςȂ�
//-------------------------------------------------------------
bool CSystem::IsKeyDown(const BYTE keyCode)
{
	return m_pClsInput->IsKeyDown(keyCode);
}

//-------------------------------------------------------------
// �L�[���͏��̎擾
// ���@���FBYTE	�L�[�R�[�h
// ���@�l�F�L�[�̈�x�̂�
//-------------------------------------------------------------
bool CSystem::IsKeyPressed(const BYTE keyCode)
{
	return m_pClsInput->IsKeyPressed(keyCode);
}

//-------------------------------------------------------------
// �L�[���͏��̎擾
// ���@���FBYTE	�L�[�R�[�h
// ���@�l�F�L�[�̂������ςȂ�
//-------------------------------------------------------------
bool CSystem::IsMouseDown(const BYTE mouseButton)
{
	return m_pClsInput->IsMouseDown(mouseButton);
}

//-------------------------------------------------------------
// �L�[���͏��̎擾
// ���@���FBYTE	�L�[�R�[�h
// ���@�l�F�L�[�̈�x�̂�
//-------------------------------------------------------------
bool CSystem::IsMousePressed(const BYTE mouseButton)
{
	return m_pClsInput->IsMousePressed(mouseButton);
}

//-------------------------------------------------------------
// �}�E�X�̈ʒu
// �߂�l�FPOINT	X,Y
//-------------------------------------------------------------
POINT CSystem::GetMousePos() const
{
	POINT MousePos;

	GetCursorPos(&MousePos);

	ScreenToClient(m_hWnd, &MousePos);

	return MousePos;
}

//-------------------------------------------------------------
// �}�E�X�̈ړ���
// �߂�l�FPOINT	X,Y
//-------------------------------------------------------------
POINT CSystem::GetMouseMove() const
{
	return m_pClsInput->GetMouseMove();
}

//-------------------------------------------------------------
// �}�E�X�̃z�C�[��
// �߂�l�Fint		�z�C�[��
//-------------------------------------------------------------
int CSystem::GetMouseWheel() const
{
	return m_pClsInput->GetMouseWheel();
}

//-------------------------------------------------------------
// �p�b�h�̎擾
// ���@���Fint	�L�[�R�[�h
// ���@�l�F�L�[�̂������ςȂ�
//-------------------------------------------------------------
bool CSystem::IsPadDown(const int padButton)
{
	return m_pClsInput->IsPadDown(padButton);
}

//-------------------------------------------------------------
// �p�b�h�̎擾
// ���@���Fint	�L�[�R�[�h
// ���@�l�F�L�[�̈�x�̂�
//-------------------------------------------------------------
bool CSystem::IsPadPressed(const int padButton)
{
	return m_pClsInput->IsPadPressed(padButton);
}

//-------------------------------------------------------------
// �W���C�X�e�B�b�N�̎擾
// ���@���Fint	�L�[�R�[�h
// ���@�l�F�L�[�̈�x�̂�
//-------------------------------------------------------------
float CSystem::GetJoyStickState(const int joyStick)
{
	return m_pClsInput->GetJoyStickState(joyStick);
}

//-------------------------------------------------------------
// �R���g���[���[���ڑ�����Ă��邩�H
//-------------------------------------------------------------
bool CSystem::IsControllerConnect()
{
	return m_pClsInput->IsControllerConnect();
}

//*************************************************************
//
// ���ԂɊւ���
//
//*************************************************************

//-------------------------------------------------------------
// �^�C�}�[�̍X�V
//-------------------------------------------------------------
void CSystem::UpdateTimer()
{
	m_pClsTimer->Update();
}

//-------------------------------------------------------------
// �P���[�v������̎���
//-------------------------------------------------------------
double CSystem::GetDeltaTime() const
{
	return m_pClsTimer->GetDeltaTime();
}

//*************************************************************
//
// �f�o�b�O�p�Ɋւ���
//
//*************************************************************

//-------------------------------------------------------------
// INFO
// ���@���Fstring	�e�L�X�g
// ���@�l�F[Info]
//-------------------------------------------------------------
void CSystem::DebugInfo(string strInfo)
{
	m_pClsDebugLog->Info(strInfo);
}

//-------------------------------------------------------------
// WARNING
// ���@���Fstring	�e�L�X�g
// ���@�l�F[WARNING]
//-------------------------------------------------------------
void CSystem::DebugWorn(string strWorn)
{
	m_pClsDebugLog->Worn(strWorn);
}

//-------------------------------------------------------------
// ERROR
// ���@���Fstring	�e�L�X�g
// ���@�l�F[ERROR]
//-------------------------------------------------------------
void CSystem::DebugErr(string strError)
{
	m_pClsDebugLog->Err(strError);
}

//-------------------------------------------------------------
// DEBUG
// ���@���Fstring	�e�L�X�g
// ���@�l�F[DEBUG]
//-------------------------------------------------------------
void CSystem::DebugText(string strDebug)
{
	m_pClsDebugLog->Debug(strDebug);
}

//-------------------------------------------------------------
// �r���[�|�[�g�̏�����
//-------------------------------------------------------------
void CSystem::InitializeViewPort()
{
	m_viewport.X = 0;
	m_viewport.Y = 0;
	m_viewport.Width = m_pWindowSettings.Width;
	m_viewport.Height = m_pWindowSettings.Height;
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
	m_pClsRenderSystem->GetDevice()->SetViewport(&m_viewport);
}

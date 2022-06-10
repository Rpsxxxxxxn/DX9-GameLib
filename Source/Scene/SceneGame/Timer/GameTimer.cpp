//********************************************************************************
//
// GameTimer.hpp
//
// �Q�[�����^�C�}�[�N���X
//
// Date: 2019/09/23 �X�V
//
//********************************************************************************

//================================================================================
// �C���N���[�h
//================================================================================
#include "GameTimer.hpp"

using namespace FileConst;

//================================================================================
// �N���X��`
//================================================================================

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
CGameTimer::CGameTimer()
	: m_timer(0.0)
{
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
CGameTimer::~CGameTimer()
{
}

//--------------------------------------------------------------------------------
// �X�V
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CGameTimer::Update(CSystem * pSystem)
{
	if (!m_isSleep)
	{
		m_timer += pSystem->GetDeltaTime();
	}
}
//*************************************************************
//
// ISprite.h
//
// �X�v���C�g�`��̊��N���X
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

#pragma once

//=============================================================
// �C���N���[�h
//=============================================================

//=============================================================
// �O���錾
//=============================================================
class CSystem;

//=============================================================
// �N���X��`
//=============================================================
class ISprite
{
public:
	//-------------------------------------------------------------
	// �R���X�g���N�^
	//-------------------------------------------------------------
	ISprite() {};

	//-------------------------------------------------------------
	// �f�X�g���N�^
	//-------------------------------------------------------------
	virtual ~ISprite() {};

	//-------------------------------------------------------------
	// �`��
	// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
	//-------------------------------------------------------------
	virtual void Draw(CSystem* pSystem) = 0;
};
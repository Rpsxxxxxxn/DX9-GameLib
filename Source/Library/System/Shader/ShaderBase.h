//*************************************************************
//
// ShaderBase.h
//
// �V�F�[�_�N���X(���)
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

#pragma once

//=============================================================
// �C���N���[�h
//=============================================================
#include "../../Common.h"

//=============================================================
// �N���X��`
//=============================================================
class CShaderBase
{
public:

	//-------------------------------------------------------------
	// �R���X�g���N�^
	//-------------------------------------------------------------
	CShaderBase();

	//-------------------------------------------------------------
	// �f�X�g���N�^
	//-------------------------------------------------------------
	virtual ~CShaderBase();

	//-------------------------------------------------------------
	// ����������
	// ���@���FLPDIRECT3DDEVICE9	�f�o�C�X�̃|�C���^
	// �@�@�@�@string				�t�@�C���p�X
	//-------------------------------------------------------------
	virtual void Initialize(LPDIRECT3DDEVICE9 pDevice, const string path);

	//-------------------------------------------------------------
	// �X�V����
	//-------------------------------------------------------------
	virtual void Update() = 0;

	//-------------------------------------------------------------
	// ���l�̐ݒ�
	// ���@���Fstring				�ϐ���
	// �@�@�@�FIDirect3DTexture9	�e�N�X�`���̃|�C���^
	//-------------------------------------------------------------
	void SetValue(string name, IDirect3DTexture9* tex) { m_pEffect->SetTexture(name.c_str(), tex); };
	void SetValue(string name, IDirect3DTexture9& tex) { m_pEffect->SetTexture(name.c_str(), &tex); };

	//-------------------------------------------------------------
	// ���l�̐ݒ�
	// ���@���Fstring				�ϐ���
	// �@�@�@�FD3DXMATRIX			�s��
	//-------------------------------------------------------------
	void SetValue(string name, D3DXMATRIX* mat) { m_pEffect->SetMatrix(name.c_str(), mat); };
	void SetValue(string name, D3DXMATRIX& mat) { m_pEffect->SetMatrix(name.c_str(), &mat); };

	//-------------------------------------------------------------
	// ���l�̐ݒ�
	// ���@���Fstring				�ϐ���
	// �@�@�@�FD3DXVECTOR4			�S�����x�N�g��
	//-------------------------------------------------------------
	void SetValue(string name, D3DXVECTOR4* vec) { m_pEffect->SetVector(name.c_str(), vec); };
	void SetValue(string name, D3DXVECTOR4& vec) { m_pEffect->SetVector(name.c_str(), &vec); };

	//-------------------------------------------------------------
	// ���l�̐ݒ�
	// ���@���Fstring				�ϐ���
	// �@�@�@�FMatrix				�s��
	// �@�@�@�FUINT					�z��
	//-------------------------------------------------------------
	void SetValue(string name, D3DXMATRIX* matAry, UINT cnt) { m_pEffect->SetMatrixArray(name.c_str(), matAry, cnt); };
	void SetValue(string name, D3DXMATRIX& matAry, UINT cnt) { m_pEffect->SetMatrixArray(name.c_str(), &matAry, cnt); };

	//-------------------------------------------------------------
	// ���l�̐ݒ�
	// ���@���Fstring				�ϐ���
	// �@�@�@�FINT					����
	//-------------------------------------------------------------
	void SetValue(string name, INT* num) { m_pEffect->SetInt(name.c_str(), *num); };
	void SetValue(string name, INT& num) { m_pEffect->SetInt(name.c_str(), num); };

	//-------------------------------------------------------------
	// �G�t�F�N�g�̎擾
	// �߂�l�FLPD3DXEFFECT			�G�t�F�N�g
	//-------------------------------------------------------------
	LPD3DXEFFECT GetEffect();

protected:
	LPDIRECT3DDEVICE9 m_pDevice;
	LPD3DXEFFECT m_pEffect;
};
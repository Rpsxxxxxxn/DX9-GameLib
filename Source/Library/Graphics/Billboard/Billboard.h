//*************************************************************
//
// Billboard.hpp
//
// ���b�V������(��ɂ�����������Ă���)
//
// Date: 2019/10/14 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "../../Common.h"
#include "../../System/System.h"

//=============================================================
// �\���̒�`
//=============================================================
struct Vertex3D
{
	Vector3 vtx;
	Vector3 nor;
	D3DCOLOR dif;
	Vector2 tex;
};

//=============================================================
// �r���{�[�h�ݒ�̎��
//=============================================================
enum BillboardType
{
	BILL_CE,
	BILL_LU,
	BILL_CEG,
};

//=============================================================
// �N���X��`
//=============================================================
class CBillboard
{
public:

	//-------------------------------------------------------------
	// �R���X�g���N�^
	//-------------------------------------------------------------
	CBillboard();

	//-------------------------------------------------------------
	// �f�X�g���N�^
	//-------------------------------------------------------------
	~CBillboard();

	//-------------------------------------------------------------
	// ������
	// ���@���FCSystem*				�f�o�C�X�̃|�C���^
	// �@�@�@�@LPDIRECT3DTEXTURE9	�e�N�X�`���̃|�C���^
	// �@�@�@�@BillboardType		�r���{�[�h���_�����̎��
	//-------------------------------------------------------------
	void Initialize(CSystem* pSystem, LPDIRECT3DTEXTURE9 pTexture, BillboardType type);

	//-------------------------------------------------------------
	// �`��
	// ���@���FLPDIRECT3DDEVICE9	�f�o�C�X�̃|�C���^
	// �@�@�@�@Matrix*				�r���[�̋t�s��
	//-------------------------------------------------------------
	void Draw(CSystem* pSystem, Matrix* pInvView);

	//-------------------------------------------------------------
	// �F�̐ݒ�
	// ���@���FD3DXCOLOR	RGBA�F
	//-------------------------------------------------------------
	void SetColor(const D3DXCOLOR color);

	//-------------------------------------------------------------
	// �ʒu�x�N�g���̐ݒ�
	// ���@���FVector3*		�ʒu�x�N�g���̃|�C���^
	//-------------------------------------------------------------
	void SetPosition(const Vector3* vecPos);

	//-------------------------------------------------------------
	// �g��x�N�g���̐ݒ�
	// ���@���FVector3*		�g��x�N�g���̃|�C���^
	//-------------------------------------------------------------
	void SetSize(const Vector2* vecSize);

	//-------------------------------------------------------------
	// �i�s�󋵂̐ݒ�
	// ���@���Ffloat		�p�[�Z���g
	//-------------------------------------------------------------
	void SetProgress(const float gauge);
private:

	//-------------------------------------------------------------
	// �X�V
	//-------------------------------------------------------------
	void Update(Matrix* pInvView);

	//-------------------------------------------------------------
	// �r���{�[�h(��������)
	//-------------------------------------------------------------
	void GenerateVertexCE();

	//-------------------------------------------------------------
	// �r���{�[�h(���㐶��)
	//-------------------------------------------------------------
	void GenerateVertexLU();

	//-------------------------------------------------------------
	// �r���{�[�h(���������Q�[�W�p)
	//-------------------------------------------------------------
	void GenerateVertexCEG();

	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DTEXTURE9 m_pTexture;
	BillboardType m_type;
	D3DXCOLOR m_color;
	Vector2 m_vecSize;
	Vector3 m_vecPos;
	Matrix m_matWorld;

	float m_sizeX;
};
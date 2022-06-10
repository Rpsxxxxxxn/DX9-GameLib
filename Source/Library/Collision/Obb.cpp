//*************************************************************
//
// Obb.cpp
//
// �����蔻��N���X(���I�n�R)
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "Obb.h"

//=============================================================
// �萔��`
//=============================================================
const int CBoundingBox::VERTEX_NUM = 8;
const int CBoundingBox::INDEX_NUM = 16;
const int CBoundingBox::POLYGON_NUM = 12;

//-------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------
CBoundingBox::CBoundingBox()
	: m_pVtxBuf(nullptr)
	, m_pIdxBuf(nullptr)
{
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
CBoundingBox::~CBoundingBox()
{
	SafeRelease(m_pVtxBuf);
	SafeRelease(m_pIdxBuf);
}

//-------------------------------------------------------------
// ������
// ���@���FCSystem* pSystem		�V�X�e���N���X�̃|�C���^
// �@�@�@�@const Vector3*		�ő�l
// �@�@�@�@const Vector3*		�ŏ��l
// �@�@�@�@const bool			�f�o�b�O�p�`��
//-------------------------------------------------------------
void CBoundingBox::Create(
	CSystem* pSystem,
	const Vector3* pVecMax,
	const Vector3* pVecMin,
	const bool showDebug
)
{
	auto pDevice = pSystem->GetDevice();

	m_obbData.vecRad = (*pVecMax - *pVecMin) / 2.0f;

	m_obbData.vecAdjust = *pVecMax - m_obbData.vecRad;

	if (showDebug) 
	{
		auto hr = S_OK;

		//���_�o�b�t�@�̍쐬
		hr = pDevice->CreateVertexBuffer(
			sizeof(Vector3) * VERTEX_NUM, 0, D3DFVF_XYZ, D3DPOOL_DEFAULT, &m_pVtxBuf, NULL);
		if (FAILED(hr)) MessageBox(nullptr, "CreateVertexBuffer", "�G���[", MB_OK);

		//�C���f�b�N�X�o�b�t�@�̍쐬
		hr = pDevice->CreateIndexBuffer(
			sizeof(WORD) * INDEX_NUM, 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIdxBuf, NULL);
		if (FAILED(hr)) MessageBox(nullptr, "CreateIndexBuffer", "�G���[", MB_OK);

		Vector3 vertices[VERTEX_NUM] = 
		{
			Vector3(pVecMin->x, pVecMax->y, pVecMin->z),
			Vector3(pVecMax->x, pVecMax->y, pVecMin->z),
			Vector3(pVecMin->x, pVecMin->y, pVecMin->z),
			Vector3(pVecMax->x, pVecMin->y, pVecMin->z),
			Vector3(pVecMin->x, pVecMax->y, pVecMax->z),
			Vector3(pVecMax->x, pVecMax->y, pVecMax->z),
			Vector3(pVecMin->x, pVecMin->y, pVecMax->z),
			Vector3(pVecMax->x, pVecMin->y, pVecMax->z),
		};

		//���_�̐ݒ�
		void* pVertices;

		hr = m_pVtxBuf->Lock(0, sizeof(vertices), (void**)&pVertices, 0);
		if (FAILED(hr)) MessageBox(nullptr, "VertexLock", "�G���[", MB_OK);

		memcpy(pVertices, vertices, sizeof(vertices));

		m_pVtxBuf->Unlock();

		WORD indices[INDEX_NUM] = 
		{ 
			0, 1, 3,
			2, 0, 4,
			5, 7, 6,
			4, 6, 2,
			3, 7, 5, 
			1 
		};

		//�C���f�b�N�X�̐ݒ�
		void* pIndex;

		hr = m_pIdxBuf->Lock(0, sizeof(indices), (void**)&pIndex, 0);
		if (FAILED(hr)) MessageBox(nullptr, "IndexLock", "�G���[", MB_OK);

		memcpy(pIndex, indices, sizeof(indices));

		m_pIdxBuf->Unlock();
	}
}

//-------------------------------------------------------------
// �`��
// ���@���FCSystem* pSystem		�V�X�e���N���X�̃|�C���^
//-------------------------------------------------------------
void CBoundingBox::Draw(CSystem * pSystem)
{
	if (m_pVtxBuf && m_pIdxBuf)
	{
		auto pDevice = pSystem->GetDevice();
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
		pDevice->SetStreamSource(0, m_pVtxBuf, 0, sizeof(Vector3));
		pDevice->SetIndices(m_pIdxBuf);
		pDevice->SetTexture(0, nullptr);
		pDevice->DrawIndexedPrimitive(D3DPT_LINESTRIP, 0, 0, VERTEX_NUM, 0, INDEX_NUM - 1);
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}

//-------------------------------------------------------------
// ���E�s��̐ݒ�
// ���@���Fconst Matrix*		���E�s��
//-------------------------------------------------------------
void CBoundingBox::SetTransform(const Matrix * pMatWorld)
{
	m_matWorld = *pMatWorld;
}

//-------------------------------------------------------------
// ���E�s��̎擾
// �߂�l�Fconst float			���E�s��
//-------------------------------------------------------------
Matrix CBoundingBox::GetTransform() const
{
	return m_matWorld;
}

//-------------------------------------------------------------
// �ő�l
//-------------------------------------------------------------
Vector3 CBoundingBox::GetMaxVector3()
{
	Vector3 result;
	result.x = m_obbData.vecPos.x + m_obbData.vecRad.x;
	result.y = m_obbData.vecPos.y + m_obbData.vecRad.y;
	result.z = m_obbData.vecPos.z + m_obbData.vecRad.z;
	return result;
}

//-------------------------------------------------------------
// �ŏ��l
//-------------------------------------------------------------
Vector3 CBoundingBox::GetMinVector3()
{
	Vector3 result;
	result.x = m_obbData.vecPos.x - m_obbData.vecRad.x;
	result.y = m_obbData.vecPos.y - m_obbData.vecRad.y;
	result.z = m_obbData.vecPos.z - m_obbData.vecRad.z;
	return result;
}

//-------------------------------------------------------------
// �o�E���f�B���O�{�b�N�X�̃f�[�^
// �߂�l�Fconst ObbData	ObbData�\����
//-------------------------------------------------------------
ObbData* CBoundingBox::GetObb()
{
	return &m_obbData;
}

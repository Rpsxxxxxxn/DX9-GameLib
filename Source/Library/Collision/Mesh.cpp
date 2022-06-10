//*************************************************************
//
// Mesh.cpp
//
// 当たり判定クラス(メッシュ)
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "Mesh.h"

//=============================================================
// クラス定義
//=============================================================

//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
CMesh::CMesh()
	: m_pMeshData(nullptr)
{
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CMesh::~CMesh()
{
	SafeDeleteArray(m_pMeshData);
}

//-------------------------------------------------------------
// 初期化
//-------------------------------------------------------------
void CMesh::Create(CXFile * m_pFrame)
{
	m_pMesh = m_pFrame;

	//ルートメッシュ取得
	LPD3DXMESH pMesh = m_pMesh->GetRootMesh();

	//頂点情報
	DWORD numVtxBytes = pMesh->GetNumBytesPerVertex();

	//ポリゴン数
	m_numFaces = pMesh->GetNumFaces();

	//メッシュでーたの生成
	m_pMeshData = new Mesh_Data[m_numFaces];

	//隣接データ配列の作成
	DWORD* pAdjacency = new DWORD[m_numFaces * 3];

	//隣接データの取得
	pMesh->GenerateAdjacency(0.01f, pAdjacency);

	//頂点バッファのロック
	BYTE* pVtx;
	pMesh->LockVertexBuffer(0, (void**)&pVtx);

	//インデックスバッファのロック
	WORD* pIdx;
	pMesh->LockIndexBuffer(0, (void**)&pIdx);

	//ポリゴンデータセット
	for (DWORD i = 0; i < m_numFaces; i++)
	{
		m_pMeshData[i].vVertex[0] = 
			*(Vector3*)(pVtx + (numVtxBytes * (pIdx)[i * 3 + 0]));
		m_pMeshData[i].vVertex[1] = 
			*(Vector3*)(pVtx + (numVtxBytes * (pIdx)[i * 3 + 1]));
		m_pMeshData[i].vVertex[2] = 
			*(Vector3*)(pVtx + (numVtxBytes * (pIdx)[i * 3 + 2]));

		m_pMeshData[i].vNormal = ComputeNormals(m_pMeshData[i].vVertex);

		m_pMeshData[i].dwAdjacency[0] = pAdjacency[i * 3 + 0];
		m_pMeshData[i].dwAdjacency[1] = pAdjacency[i * 3 + 1];
		m_pMeshData[i].dwAdjacency[2] = pAdjacency[i * 3 + 2];
	}

	pMesh->UnlockVertexBuffer();
	pMesh->UnlockIndexBuffer();

	delete[] pAdjacency;
}

//-------------------------------------------------------------
// 確認フラグを初期化
//-------------------------------------------------------------
void CMesh::ClearCheckFlg()
{
	for (DWORD i = 0; i < m_numFaces; i++) 
	{
		m_pMeshData[i].bChecked = false;
	}
}

//-------------------------------------------------------------
// ポリゴンの法線計算
//-------------------------------------------------------------
Vector3 CMesh::ComputeNormals(const Vector3 vtx[3])
{
	Vector3 vN;
	Vector3 v1 = vtx[1] - vtx[0];
	Vector3 v2 = vtx[2] - vtx[0];

	D3DXVec3Cross(&vN, &v1, &v2);
	D3DXVec3Normalize(&vN, &vN);

	return vN;
}

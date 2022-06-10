//*************************************************************
//
// CollisionCheck.hpp
//
// 当たり判定確認クラス
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "CollisionCheck.h"
#include "../Graphics/Camera/Camera.h"
#include "../../Scene/GameManager/GameObject.h"

//-------------------------------------------------------------
// OBB同士の当たり判定
// 引　数：CBoundingBox*	１つ目のバウンディングボックス
// 　　　　CBoundingBox*　	２つ目のバウンディングボックス
// 戻り値：当たっていればTRUE
//-------------------------------------------------------------
bool CCollisionCheck::IsObb(CBoundingBox * obb1, CBoundingBox * obb2)
{
	ObbData* d1 = obb1->GetObb();
	ObbData* d2 = obb2->GetObb();

	Matrix mat1 = obb1->GetTransform();
	Matrix mat2 = obb2->GetTransform();

	Matrix matWork;
	Vector3 vecWork;

	matWork = mat1;
	matWork._41 = matWork._42 = matWork._43 = 0;

	vecWork = d1->vecAdjust;
	D3DXVec3TransformCoord(&vecWork, &vecWork, &matWork);

	d1->vecPos.x = mat1._41 + vecWork.x;
	d1->vecPos.y = mat1._42 + vecWork.y;
	d1->vecPos.z = mat1._43 + vecWork.z;

	d1->vecAxisX.x = mat1._11; d1->vecAxisX.y = mat1._12; d1->vecAxisX.z = mat1._13;
	d1->vecAxisY.x = mat1._21; d1->vecAxisY.y = mat1._22; d1->vecAxisY.z = mat1._23;
	d1->vecAxisZ.x = mat1._31; d1->vecAxisZ.y = mat1._32; d1->vecAxisZ.z = mat1._33;

	matWork = mat2;
	matWork._41 = matWork._42 = matWork._43 = 0;

	vecWork = d2->vecAdjust;
	D3DXVec3TransformCoord(&vecWork, &vecWork, &matWork);

	d2->vecPos.x = mat2._41 + vecWork.x;
	d2->vecPos.y = mat2._42 + vecWork.y;
	d2->vecPos.z = mat2._43 + vecWork.z;

	d2->vecAxisX.x = mat1._11; d2->vecAxisX.y = mat1._12; d2->vecAxisX.z = mat1._13;
	d2->vecAxisY.x = mat1._21; d2->vecAxisY.y = mat1._22; d2->vecAxisY.z = mat1._23;
	d2->vecAxisZ.x = mat1._31; d2->vecAxisZ.y = mat1._32; d2->vecAxisZ.z = mat1._33;

	//２点間の距離ベクトルを求める
	Vector3 vecDist = d2->vecPos - d1->vecPos;

	//分離軸を比較
	if (!CompareLengthObb(d1, d2, &d1->vecAxisX, &vecDist)) return false;
	if (!CompareLengthObb(d1, d2, &d1->vecAxisY, &vecDist)) return false;
	if (!CompareLengthObb(d1, d2, &d1->vecAxisZ, &vecDist)) return false;
	if (!CompareLengthObb(d1, d2, &d2->vecAxisX, &vecDist)) return false;
	if (!CompareLengthObb(d1, d2, &d2->vecAxisY, &vecDist)) return false;
	if (!CompareLengthObb(d1, d2, &d2->vecAxisZ, &vecDist)) return false;


	//分離軸同士の外積を比較
	Vector3 vecSep;
	D3DXVec3Cross(&vecSep, &d1->vecAxisX, &d2->vecAxisX);
	if (!CompareLengthObb(d1, d2, &vecSep, &vecDist)) return false;
	D3DXVec3Cross(&vecSep, &d1->vecAxisX, &d2->vecAxisY);
	if (!CompareLengthObb(d1, d2, &vecSep, &vecDist)) return false;
	D3DXVec3Cross(&vecSep, &d1->vecAxisX, &d2->vecAxisZ);
	if (!CompareLengthObb(d1, d2, &vecSep, &vecDist)) return false;
	D3DXVec3Cross(&vecSep, &d1->vecAxisY, &d2->vecAxisX);
	if (!CompareLengthObb(d1, d2, &vecSep, &vecDist)) return false;
	D3DXVec3Cross(&vecSep, &d1->vecAxisY, &d2->vecAxisY);
	if (!CompareLengthObb(d1, d2, &vecSep, &vecDist)) return false;
	D3DXVec3Cross(&vecSep, &d1->vecAxisY, &d2->vecAxisZ);
	if (!CompareLengthObb(d1, d2, &vecSep, &vecDist)) return false;
	D3DXVec3Cross(&vecSep, &d1->vecAxisZ, &d2->vecAxisX);
	if (!CompareLengthObb(d1, d2, &vecSep, &vecDist)) return false;
	D3DXVec3Cross(&vecSep, &d1->vecAxisZ, &d2->vecAxisY);
	if (!CompareLengthObb(d1, d2, &vecSep, &vecDist)) return false;
	D3DXVec3Cross(&vecSep, &d1->vecAxisZ, &d2->vecAxisZ);
	if (!CompareLengthObb(d1, d2, &vecSep, &vecDist)) return false;

	return true;
}

//-------------------------------------------------------------
// SPHERE同士の当たり判定
// 引　数：CBoundingSphere*	１つ目のバウンディングスフィア
// 　　　　CBoundingSphere*	２つ目のバウンディングスフィア
// 戻り値：当たっていればTRUE
//-------------------------------------------------------------
bool CCollisionCheck::IsSphere(CSphere* s1, CSphere * s2)
{
	float radius1 = s1->GetRadius();
	float radius2 = s2->GetRadius();

	Matrix mat1 = s1->GetTransform();
	Matrix mat2 = s2->GetTransform();

	Vector3 vecCenter1(mat1._41, mat1._42, mat1._43);
	Vector3 vecCenter2(mat2._41, mat2._42, mat2._43);

	Vector3 vecDist = vecCenter1 - vecCenter2;

	float len = D3DXVec3Length(&vecDist);

	if (len < radius1 + radius2)
	{
		return true;
	}
	return false;
}

//-------------------------------------------------------------
// レイとフィールドの当たり判定
// 引　数：CBoundingSphere*	１つ目のバウンディングスフィア
// 　　　　CBoundingSphere*	２つ目のバウンディングスフィア
// 戻り値：当たっていればTRUE
//-------------------------------------------------------------
void CCollisionCheck::IsField(CRay * p1, CMesh* p2)
{
	if (p1 == nullptr) return;
	if (p2 == nullptr) return;

	auto pMeshData = p2->GetMeshData();
	auto numFaces = p2->GetNumFaces();

	auto vCrossPoint = Vector3(0.0f, 0.0f, 0.0f);
	auto vRayPos = p1->m_position;

	DWORD index;

	if (p1->m_dwCurrent == 0x00000000)
	{
		//全ポリゴンとレイの交差判定を行う
		for (index = 0; index < numFaces; index++)
		{
			if (IntersectPoint(
				&vRayPos, 
				p1->m_direction,
				&pMeshData[index].vNormal,
				pMeshData[index].vVertex,
				&vCrossPoint))
			{
				p1->m_position.y = vCrossPoint.y;
				break;
			}
		}

		if (index < numFaces)
		{
			p1->m_dwCurrent = index;
		}
		else {
			p1->m_dwCurrent = 0xffffffff;
		}
	}

	p2->ClearCheckFlg();

	CallBackFieldIntersect(p1, p2, pMeshData[p1->m_dwCurrent].dwAdjacency[0], &p1->m_dwCurrent);
}

//-------------------------------------------------------------
// 視推台カリング(スフィア)
// 引　数：CCamera*		カメラクラスのポインタ
// 　　　　CGameObject*	オブジェクトクラスのポインタ
// 戻り値：当たっていればTRUE
// 備　考：当たっていれば、VisibleにTRUEが入ります。
//-------------------------------------------------------------
bool CCollisionCheck::IsFrustumAndSphere(CCamera * pCamera, CGameObject * pObj)
{
	Vector3 vecPos = pObj->GetPosition();
	float radius = pObj->GetRadius();

	for (int i = 0; i < 6; i++)
	{
		D3DXPLANE plane = pCamera->GetFrustumPlane(i);

		//平面と点の内積を計算
		if (D3DXPlaneDotCoord(&plane, &vecPos) + radius < 0.0f)
		{
			pObj->SetVisible(false);
			return false;
		}
	}

	pObj->SetVisible(true);
	return true;
}

//-------------------------------------------------------------
// 視推台カリング(ボックス)
// 引　数：CCamera*		カメラクラスのポインタ
// 　　　　CGameObject*	オブジェクトクラスのポインタ
// 戻り値：当たっていればTRUE
//-------------------------------------------------------------
bool CCollisionCheck::IsFrustumAndBox(
	CCamera * pCamera,
	CBoundingBox* pObb, 
	CGameObject * pObj
)
{
	Vector3 pos(0.0f, 0.0f, 0.0f);
	ObbData* obbData = pObb->GetObb();

	obbData->vecPos = pObj->GetPosition();

	for (int i = 0; i < 6; i++)
	{
		D3DXPLANE plane = pCamera->GetFrustumPlane(i);

		pos.x = obbData->vecPos.x - obbData->vecRad.x;
		pos.y = obbData->vecPos.y - obbData->vecRad.y;
		pos.z = obbData->vecPos.z - obbData->vecRad.z;
		if (D3DXPlaneDotCoord(&plane, &pos) >= 0.0f)
		{
			continue;
		}

		pos.x = obbData->vecPos.x + obbData->vecRad.x;
		pos.y = obbData->vecPos.y - obbData->vecRad.y;
		pos.z = obbData->vecPos.z - obbData->vecRad.z;
		if (D3DXPlaneDotCoord(&plane, &pos) >= 0.0f)
		{
			continue;
		}

		pos.x = obbData->vecPos.x - obbData->vecRad.x;
		pos.y = obbData->vecPos.y + obbData->vecRad.y;
		pos.z = obbData->vecPos.z - obbData->vecRad.z;
		if (D3DXPlaneDotCoord(&plane, &pos) >= 0.0f)
		{
			continue;
		}

		pos.x = obbData->vecPos.x + obbData->vecRad.x;
		pos.y = obbData->vecPos.y + obbData->vecRad.y;
		pos.z = obbData->vecPos.z - obbData->vecRad.z;
		if (D3DXPlaneDotCoord(&plane, &pos) >= 0.0f)
		{
			continue;
		}

		pos.x = obbData->vecPos.x - obbData->vecRad.x;
		pos.y = obbData->vecPos.y - obbData->vecRad.y;
		pos.z = obbData->vecPos.z + obbData->vecRad.z;
		if (D3DXPlaneDotCoord(&plane, &pos) >= 0.0f)
		{
			continue;
		}

		pos.x = obbData->vecPos.x + obbData->vecRad.x;
		pos.y = obbData->vecPos.y - obbData->vecRad.y;
		pos.z = obbData->vecPos.z + obbData->vecRad.z;
		if (D3DXPlaneDotCoord(&plane, &pos) >= 0.0f)
		{
			continue;
		}

		pos.x = obbData->vecPos.x - obbData->vecRad.x;
		pos.y = obbData->vecPos.y + obbData->vecRad.y;
		pos.z = obbData->vecPos.z + obbData->vecRad.z;
		if (D3DXPlaneDotCoord(&plane, &pos) >= 0.0f)
		{
			continue;
		}

		pos.x = obbData->vecPos.x + obbData->vecRad.x;
		pos.y = obbData->vecPos.y + obbData->vecRad.y;
		pos.z = obbData->vecPos.z + obbData->vecRad.z;
		if (D3DXPlaneDotCoord(&plane, &pos) >= 0.0f)
		{
			continue;
		}

		pObj->SetVisible(false);
		return false;
	}

	pObj->SetVisible(true); //内側
	return true;
}

//-------------------------------------------------------------
// RayとPolygonの交わる点を求める
// 引　数：Vector3*		位置
// 　　　：Vector3		Ray
// 　　　：Vector3*		法線
// 　　　：Vector3[3]	頂点配列
//-------------------------------------------------------------
bool CCollisionCheck::IntersectPoint(
	const Vector3 * pPos,
	const Vector3 & vRay, 
	const Vector3 * pNormal, 
	const Vector3 vVtx[3], 
	Vector3 * pCrossP
)
{
	Vector3 vWork = vVtx[0] - *pPos;
	float numerator = D3DXVec3Dot(pNormal, &vWork);
	float denominator = D3DXVec3Dot(pNormal, &vRay);
	if (denominator >= 0) return false;

	float t = numerator / denominator;
	Vector3 vP = *pPos + vRay * t;
	vWork = vP - *pPos;

	//レイの逆方向にあるポリゴンの場合、交点無しとする
	Vector3 vNormal;
	D3DXVec3Normalize(&vNormal, &vWork);
	if (D3DXVec3Dot(&vRay, &vNormal) <= 0) return false;

	//交点がポリゴン内にあるかどうか調べる
	Vector3 v1, v2;
	float result;

	v1 = vVtx[1] - vVtx[0];
	v2 = vP - vVtx[1];
	D3DXVec3Cross(&vWork, &v1, &v2);
	result = D3DXVec3Dot(&vWork, pNormal);
	if (result < 0) return false;

	v1 = vVtx[2] - vVtx[1];
	v2 = vP - vVtx[2];
	D3DXVec3Cross(&vWork, &v1, &v2);
	result = D3DXVec3Dot(&vWork, pNormal);
	if (result < 0) return false;

	v1 = vVtx[0] - vVtx[2];
	v2 = vP - vVtx[0];
	D3DXVec3Cross(&vWork, &v1, &v2);
	result = D3DXVec3Dot(&vWork, pNormal);
	if (result < 0) return false;

	if (pCrossP) *pCrossP = vP;

	return true;
}

//-------------------------------------------------------------
// 再帰呼び出し用
// 引　数：Vector3*		位置
// 　　　：Vector3		隣接ポリゴン
// 　　　：DWORD		ポリゴン番号
//-------------------------------------------------------------
bool CCollisionCheck::CallBackFieldIntersect(CRay* pRay, CMesh* pFrame, DWORD index, DWORD * pIndex)
{
	auto pMeshData = pFrame->GetMeshData();

	//インデックスがないとき
	if (index == 0xffffffff)
	{
		return false;
	}

	//チェック済み
	if (pMeshData[index].bChecked)
	{
		return false;
	}

	//チェックしてないポリゴンがある！
	pMeshData[index].bChecked = true;

	auto vPos = pRay->m_position + Vector3(0.0f, 10.0f, 0.0f);
	auto vRay = Vector3(0.0f, -1.0f, 0.0f);
	auto vCrossPoint = Vector3(0.0f, 0.0f, 0.0f);

	//早速当たり判定しますよ
	if (IntersectPoint(&vPos, vRay, &pMeshData[index].vNormal, pMeshData[index].vVertex, &vCrossPoint))
	{
		*pIndex = index;
		pRay->m_position.y = vCrossPoint.y;
		pRay->m_normal = pMeshData[index].vNormal;
		return true;
	}

	//周りの３つのポリゴンを調べていくよ！
	for (int i = 0; i < 3; i++)
	{
		if (CallBackFieldIntersect(
			pRay,
			pFrame, 
			pMeshData[index].dwAdjacency[i], 
			pIndex))
		{
			return true;
		}
	}

	return false;
}

//-------------------------------------------------------------
// OBBの投影距離比較
//-------------------------------------------------------------
bool CCollisionCheck::CompareLengthObb(
	const ObbData* o1,
	const ObbData* o2,
	const Vector3* vecSep,
	const Vector3* vecDist
)
{
	// 分離軸上のAからBの距離
	float length = fabsf(D3DXVec3Dot(vecSep, vecDist));

	// 分離軸上でAから最も遠いAの頂点までの距離
	float lenA = 
		fabsf(D3DXVec3Dot(&o1->vecAxisX, vecSep) * o1->vecRad.x)
		+ fabsf(D3DXVec3Dot(&o1->vecAxisY, vecSep) * o1->vecRad.y)
		+ fabsf(D3DXVec3Dot(&o1->vecAxisZ, vecSep) * o1->vecRad.z);

	// 分離軸上でBから最も遠いBの頂点までの距離
	float lenB =
		fabsf(D3DXVec3Dot(&o2->vecAxisX, vecSep) * o2->vecRad.x)
		+ fabsf(D3DXVec3Dot(&o2->vecAxisY, vecSep) * o2->vecRad.y)
		+ fabsf(D3DXVec3Dot(&o2->vecAxisZ, vecSep) * o2->vecRad.z);

	if (length > lenA + lenB)
	{
		return false;
	}

	return true;
}

//*************************************************************
//
// CollisionCheck.hpp
//
// �����蔻��m�F�N���X
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "CollisionCheck.h"
#include "../Graphics/Camera/Camera.h"
#include "../../Scene/GameManager/GameObject.h"

//-------------------------------------------------------------
// OBB���m�̓����蔻��
// ���@���FCBoundingBox*	�P�ڂ̃o�E���f�B���O�{�b�N�X
// �@�@�@�@CBoundingBox*�@	�Q�ڂ̃o�E���f�B���O�{�b�N�X
// �߂�l�F�������Ă����TRUE
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

	//�Q�_�Ԃ̋����x�N�g�������߂�
	Vector3 vecDist = d2->vecPos - d1->vecPos;

	//���������r
	if (!CompareLengthObb(d1, d2, &d1->vecAxisX, &vecDist)) return false;
	if (!CompareLengthObb(d1, d2, &d1->vecAxisY, &vecDist)) return false;
	if (!CompareLengthObb(d1, d2, &d1->vecAxisZ, &vecDist)) return false;
	if (!CompareLengthObb(d1, d2, &d2->vecAxisX, &vecDist)) return false;
	if (!CompareLengthObb(d1, d2, &d2->vecAxisY, &vecDist)) return false;
	if (!CompareLengthObb(d1, d2, &d2->vecAxisZ, &vecDist)) return false;


	//���������m�̊O�ς��r
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
// SPHERE���m�̓����蔻��
// ���@���FCBoundingSphere*	�P�ڂ̃o�E���f�B���O�X�t�B�A
// �@�@�@�@CBoundingSphere*	�Q�ڂ̃o�E���f�B���O�X�t�B�A
// �߂�l�F�������Ă����TRUE
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
// ���C�ƃt�B�[���h�̓����蔻��
// ���@���FCBoundingSphere*	�P�ڂ̃o�E���f�B���O�X�t�B�A
// �@�@�@�@CBoundingSphere*	�Q�ڂ̃o�E���f�B���O�X�t�B�A
// �߂�l�F�������Ă����TRUE
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
		//�S�|���S���ƃ��C�̌���������s��
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
// ������J�����O(�X�t�B�A)
// ���@���FCCamera*		�J�����N���X�̃|�C���^
// �@�@�@�@CGameObject*	�I�u�W�F�N�g�N���X�̃|�C���^
// �߂�l�F�������Ă����TRUE
// ���@�l�F�������Ă���΁AVisible��TRUE������܂��B
//-------------------------------------------------------------
bool CCollisionCheck::IsFrustumAndSphere(CCamera * pCamera, CGameObject * pObj)
{
	Vector3 vecPos = pObj->GetPosition();
	float radius = pObj->GetRadius();

	for (int i = 0; i < 6; i++)
	{
		D3DXPLANE plane = pCamera->GetFrustumPlane(i);

		//���ʂƓ_�̓��ς��v�Z
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
// ������J�����O(�{�b�N�X)
// ���@���FCCamera*		�J�����N���X�̃|�C���^
// �@�@�@�@CGameObject*	�I�u�W�F�N�g�N���X�̃|�C���^
// �߂�l�F�������Ă����TRUE
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

	pObj->SetVisible(true); //����
	return true;
}

//-------------------------------------------------------------
// Ray��Polygon�̌����_�����߂�
// ���@���FVector3*		�ʒu
// �@�@�@�FVector3		Ray
// �@�@�@�FVector3*		�@��
// �@�@�@�FVector3[3]	���_�z��
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

	//���C�̋t�����ɂ���|���S���̏ꍇ�A��_�����Ƃ���
	Vector3 vNormal;
	D3DXVec3Normalize(&vNormal, &vWork);
	if (D3DXVec3Dot(&vRay, &vNormal) <= 0) return false;

	//��_���|���S�����ɂ��邩�ǂ������ׂ�
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
// �ċA�Ăяo���p
// ���@���FVector3*		�ʒu
// �@�@�@�FVector3		�אڃ|���S��
// �@�@�@�FDWORD		�|���S���ԍ�
//-------------------------------------------------------------
bool CCollisionCheck::CallBackFieldIntersect(CRay* pRay, CMesh* pFrame, DWORD index, DWORD * pIndex)
{
	auto pMeshData = pFrame->GetMeshData();

	//�C���f�b�N�X���Ȃ��Ƃ�
	if (index == 0xffffffff)
	{
		return false;
	}

	//�`�F�b�N�ς�
	if (pMeshData[index].bChecked)
	{
		return false;
	}

	//�`�F�b�N���ĂȂ��|���S��������I
	pMeshData[index].bChecked = true;

	auto vPos = pRay->m_position + Vector3(0.0f, 10.0f, 0.0f);
	auto vRay = Vector3(0.0f, -1.0f, 0.0f);
	auto vCrossPoint = Vector3(0.0f, 0.0f, 0.0f);

	//���������蔻�肵�܂���
	if (IntersectPoint(&vPos, vRay, &pMeshData[index].vNormal, pMeshData[index].vVertex, &vCrossPoint))
	{
		*pIndex = index;
		pRay->m_position.y = vCrossPoint.y;
		pRay->m_normal = pMeshData[index].vNormal;
		return true;
	}

	//����̂R�̃|���S���𒲂ׂĂ�����I
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
// OBB�̓��e������r
//-------------------------------------------------------------
bool CCollisionCheck::CompareLengthObb(
	const ObbData* o1,
	const ObbData* o2,
	const Vector3* vecSep,
	const Vector3* vecDist
)
{
	// ���������A����B�̋���
	float length = fabsf(D3DXVec3Dot(vecSep, vecDist));

	// ���������A����ł�����A�̒��_�܂ł̋���
	float lenA = 
		fabsf(D3DXVec3Dot(&o1->vecAxisX, vecSep) * o1->vecRad.x)
		+ fabsf(D3DXVec3Dot(&o1->vecAxisY, vecSep) * o1->vecRad.y)
		+ fabsf(D3DXVec3Dot(&o1->vecAxisZ, vecSep) * o1->vecRad.z);

	// ���������B����ł�����B�̒��_�܂ł̋���
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

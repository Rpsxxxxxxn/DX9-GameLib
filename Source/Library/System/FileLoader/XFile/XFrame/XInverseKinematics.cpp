//*************************************************************
//
// XInverseKinematics.hpp
//
// �t�^���w(InverseKinematics)
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "XInverseKinematics.h"

const float ADJUST_HEIGHT = 0.3f;

//=============================================================
// �N���X��`
//=============================================================

//-------------------------------------------------------------
// �R���X�g���N�^
// ���@���F���f�����[�g�t���[��
//-------------------------------------------------------------
CXInverseKinematics::CXInverseKinematics(CXFrame * pXFrame)
	: m_pFrame(pXFrame)
{
}

//-------------------------------------------------------------
// IK�̍X�V
//-------------------------------------------------------------
void CXInverseKinematics::UpdateIK()
{
	for (auto w : m_vecBone)
	{
		if (w.m_pBone1 == nullptr) return;

		//�����̂ŏȗ�
		Matrix shoulder = w.m_pBone1->CombinedTransformationMatrix;
		Matrix elbow = w.m_pBone2->CombinedTransformationMatrix;
		Matrix hand = w.m_pBone3->CombinedTransformationMatrix;

		//�ʒu�̎擾
		Vector3 vStartPos = Vector3(shoulder._41, shoulder._42, shoulder._43);
		Vector3 vJointPos = Vector3(elbow._41, elbow._42, elbow._43);
		Vector3 vEndPos = Vector3(hand._41, hand._42, hand._43);

		Vector3 vStartToTarget = w.vecTarget - vStartPos;
		Vector3 vStartToJoint = vJointPos - vStartPos;
		Vector3 vJointToEnd = vEndPos - vJointPos;

		//����
		float fDistStartToTarget = D3DXVec3Length(&vStartToTarget);
		float fDistStartToJoint = D3DXVec3Length(&vStartToJoint);
		float fDistJointToEnd = D3DXVec3Length(&vJointToEnd);

		//���K��
		Vector3 normalizeSTJ = vStartToJoint;
		Vector3 normalizeJTE = vJointToEnd;
		D3DXVec3Normalize(&normalizeSTJ, &normalizeSTJ);
		D3DXVec3Normalize(&normalizeJTE, &normalizeJTE);

		//���݂̌q���ڂ̊p�x�v�Z
		float fCurrentJointAngle = (D3DXVec3Dot(&(-normalizeSTJ), &normalizeJTE));

		if (fabsf(fCurrentJointAngle) < 0.95)
		{
			return;
		}

		fCurrentJointAngle = acosf(fCurrentJointAngle);

		//�Ȃ����Ă���Ō�̍��̉�]�v�Z
		float fWantedJointAngle = 0.0f;

		if (fDistStartToTarget >= fDistStartToJoint + fDistJointToEnd)
		{
			//�͂��Ȃ��ꍇ�͌v�Z�Ȃ�
			fWantedJointAngle = D3DXToRadian(180.0f);
		}
		else {
			//�p�x�v�Z
			float cosAngle = (fDistStartToJoint * fDistStartToJoint
				+ fDistJointToEnd * fDistJointToEnd 
				- fDistStartToTarget * fDistStartToTarget)
				/ (2.0f * fDistStartToJoint * fDistJointToEnd);

			fWantedJointAngle = acosf(cosAngle);
		}

		//��]�s��̌v�Z
		float fDiffJointAngle = fWantedJointAngle - fCurrentJointAngle;
		Matrix mRot;
		D3DXMatrixRotationAxis(&mRot, &Vector3(1.0f, 1.0f, 1.0f), fDiffJointAngle);

		//�I�̍s��ɓK��������
		w.m_pBone2->TransformationMatrix = mRot * w.m_pBone2->TransformationMatrix;

		//�V�����Ō�̈ʒu�̌v�Z
		Matrix mTempMatrix = w.m_pBone2->CombinedTransformationMatrix;
		mTempMatrix._41 = 0.0f;
		mTempMatrix._42 = 0.0f;
		mTempMatrix._43 = 0.0f;
		mTempMatrix._44 = 1.0f;

		Vector3 vWorldHingeAxis;
		Vector3 vNewJointToEnd;
		D3DXVec3TransformCoord(&vWorldHingeAxis, &Vector3(1.0f, 1.0f, 1.0f), &mTempMatrix);
		D3DXMatrixRotationAxis(&mRot, &vWorldHingeAxis, fDiffJointAngle);
		D3DXVec3TransformCoord(&vNewJointToEnd, &vJointToEnd, &mRot);

		Vector3 vNewEndPos;
		D3DXVec3Add(&vNewEndPos, &vNewJointToEnd, &vJointPos);

		//���߂̃{�[����]�v�Z
		Matrix mToLocal;
		D3DXMatrixInverse(&mToLocal, NULL, &w.m_pBone1->CombinedTransformationMatrix);

		Vector3 vLocalNewEnd;
		Vector3 vLocalTarget;
		D3DXVec3TransformCoord(&vLocalNewEnd, &vNewEndPos, &mToLocal);
		D3DXVec3TransformCoord(&vLocalTarget, &w.vecTarget, &mToLocal);
		D3DXVec3Normalize(&vLocalNewEnd, &vLocalNewEnd);
		D3DXVec3Normalize(&vLocalTarget, &vLocalTarget);

		Vector3 vLocalAxis;
		D3DXVec3Cross(&vLocalAxis, &vLocalNewEnd, &vLocalTarget);
		if (D3DXVec3Length(&vLocalAxis) == 0.0f) 
		{
			return;
		}

		D3DXVec3Normalize(&vLocalAxis, &vLocalAxis);
		float fLocalAngle = (D3DXVec3Dot(&vLocalNewEnd, &vLocalTarget));
		if (fabsf(fLocalAngle) >= 0.96)
		{
			return;
		}

		fLocalAngle = acosf(fLocalAngle);

		//�{�[������]�������]��K��������
		D3DXMatrixRotationAxis(&mRot, &vLocalAxis, fLocalAngle);
		w.m_pBone1->CombinedTransformationMatrix = mRot * w.m_pBone1->CombinedTransformationMatrix;
		w.m_pBone1->TransformationMatrix = mRot * w.m_pBone1->TransformationMatrix;

		//�q�{�[���̃}�g���b�N�X�s��X�V
		if (w.m_pBone1->pFrameFirstChild)
		{
			m_pFrame->UpdateFrameMatrices((D3DXFRAME_DERIVED*)w.m_pBone1->pFrameFirstChild, &w.m_pBone1->CombinedTransformationMatrix);
		}
	}
}

//-------------------------------------------------------------
// �^�[�Q�b�g�x�N�g��
// ���@���Fconst Vector3	�^�[�Q�b�g�x�N�g��
//-------------------------------------------------------------
void CXInverseKinematics::SetTargetVector(const UINT index, const Vector3 vecTarget)
{
	if (index >= m_vecBone.size()) return;

	Matrix matWorld = m_vecBone[index].m_pBone3->CombinedTransformationMatrix;

	Vector3 vecFoot = Vector3(matWorld._41, matWorld._42, matWorld._43);

	m_vecBone[index].vecFootPos = vecFoot;

	m_vecBone[index].vecTarget = Vector3(vecFoot.x, vecTarget.y + ADJUST_HEIGHT, vecFoot.z);
}

//-------------------------------------------------------------
// �{�[���t���[���̐ݒ�
// ���@���Fconst string �{�[����1
// �@�@�@�@const string �{�[����2
// �@�@�@�@const string �{�[����3
//-------------------------------------------------------------
void CXInverseKinematics::SetBoneFrame(string bone1, string bone2, string bone3)
{
	LPD3DXFRAME pFR = *m_pFrame->GetFrame();

	IkBone work;

	work.m_pBone1 = (D3DXFRAME_DERIVED*)D3DXFrameFind(pFR, (LPCSTR)bone1.c_str());
	work.m_pBone2 = (D3DXFRAME_DERIVED*)D3DXFrameFind(pFR, (LPCSTR)bone2.c_str());
	work.m_pBone3 = (D3DXFRAME_DERIVED*)D3DXFrameFind(pFR, (LPCSTR)bone3.c_str());
	work.vecTarget = Vector3(0.0f, 0.0f, 0.0f);
	work.vecFootPos = Vector3(0.0f, 0.0f, 0.0f);

	m_vecBone.push_back(work);
}

//-------------------------------------------------------------
// �{�[���̈ʒu�x�N�g���̎擾
//-------------------------------------------------------------
Vector3 CXInverseKinematics::GetBonePos(const UINT index) const
{
	if (index >= m_vecBone.size()) return Vector3(0.0f, 0.0f, 0.0f);

	return m_vecBone[index].vecFootPos;
}

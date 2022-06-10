//*************************************************************
//
// Calculate.cpp
//
// �v�Z�p�N���X
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "Calculate.h"

//-------------------------------------------------------------
// ���[���h���W����X�N���[�����W�ɕϊ�
// ���@���FLPDIRECT3DDEVICE9 �f�o�C�X�̃|�C���^
// �@�@�@�FMatrix			 �r���[�s��
// �@�@�@�FMatrix			 �v���W�F�N�V�����s��
// �@�@�@�FVector3			 �ϊ���3�����x�N�g��
// �߂�l�F�ϊ����3�����x�N�g��(X,Y�ɐ��l������AZ�ɂ�X������܂��B)
//-------------------------------------------------------------
Vector3 Calculate::WorldToScreen(
	LPDIRECT3DDEVICE9 pDevice,
	const D3DXMATRIX* mView,
	const D3DXMATRIX* mProj,
	const Vector3* vFrom
)
{
	D3DXMATRIX mWorld;
	Vector3 Out = { 0.0f,0.0f,0.0f };

	D3DXMatrixIdentity(&mWorld);

	D3DVIEWPORT9 view;

	pDevice->GetViewport(&view);

	D3DXVec3Project(&Out, vFrom, &view, mProj, mView, &mWorld);

	return Out;
}

//-------------------------------------------------------------
// �C�[�W���O�֐�
// ���@���Fdouble ���݂̌o�ߎ���
// �@�@�@�Fdouble �ő厞��
// �@�@�@�Fdouble �ŏI�ʒu
// �@�@�@�Fdouble �����ʒu
// �߂�l�F�o�ߎ��Ԃ̈ʒu
//-------------------------------------------------------------
double Calculate::OutQuart(
	double time, 
	double totaltime,
	double max, 
	double min
)
{
	max -= min;
	time = time / totaltime - 1;
	return -max * (time * time * time * time - 1) + min;
}

//-------------------------------------------------------------
// �C�[�W���O�֐�
// ���@���Fdouble ���݂̌o�ߎ���
// �@�@�@�Fdouble �ő厞��
// �@�@�@�Fdouble �ŏI�ʒu
// �@�@�@�Fdouble �����ʒu
// �߂�l�F�o�ߎ��Ԃ̈ʒu
//-------------------------------------------------------------
double Calculate::InQubic(
	double time,
	double totaltime, 
	double max, 
	double min
)
{
	max -= min;
	time /= totaltime;
	return max * time * time * time * min;
}

//-------------------------------------------------------------
// �͈͂̒��Ō��݂̐i�s�x���Z�o���܂��B
// ���@���Fconst float	�\����̉���
// �@�@�@�Fconst float	�v�Z���̍ő吔�l
// �@�@�@�Fconst float	���݂̐��l
//-------------------------------------------------------------
float Calculate::ProgressBar(
	const float width,
	const float maxValue, 
	const float nowValue
)
{
	return width / maxValue * nowValue;
}

//-------------------------------------------------------------
// �ړ�
//-------------------------------------------------------------
void Calculate::MoveAngle(
	Vector3* pVecPos,
	float fAngle, 
	float fSpeed
)
{
	pVecPos->x += cosf(fAngle) * fSpeed;
	pVecPos->z += sinf(fAngle) * fSpeed;
}

//-------------------------------------------------------------
// �A���O������180�x
// ���@���Ffloat �p�x
// �߂�l�Ffloat ������̊p�x
//-------------------------------------------------------------
float Calculate::PI180(float fAngle)
{
	if (fAngle >= D3DX_PI) {
		fAngle -= D3DX_PI * 2.0f;
	}
	else if (fAngle < -D3DX_PI) {
		fAngle += D3DX_PI * 2.0f;
	}

	return fAngle;
}

//-------------------------------------------------------------
// �A���O������360�x
// ���@���Ffloat �p�x
// �߂�l�Ffloat ������̊p�x
//-------------------------------------------------------------
float Calculate::PI360(float fAngle)
{
	if (fAngle >= D3DX_PI * 2) {
		fAngle -= D3DX_PI * 2.0f;
	}
	else if (fAngle <= 0.0f) {
		fAngle += D3DX_PI * 2.0f;
	}

	return fAngle;
}

//-------------------------------------------------------------
// ���`��ԉ�]
//-------------------------------------------------------------
void Calculate::LerpRot(float from, float speed, float* angle)
{
	float work = Calculate::PI360(from);
	float diff = work - *angle;
	diff = Calculate::PI180(diff);
	*angle += diff * speed;
	*angle = Calculate::PI180(*angle);
}

//-------------------------------------------------------------
// �����_�� ���w�� (�����Z���k�E�c�C�X�^)
// ���@���Fint �ŏ�
// �@�@�@�Fint �ő�
// �߂�l�Fint �����_�����l
//-------------------------------------------------------------
int Calculate::Random(int min, int max)
{
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution< int > rad(min, max);

	return rad(mt);
}

//-------------------------------------------------------------
// �����_�� ���w�� (�����Z���k�E�c�C�X�^)
// ���@���Ffloat �ŏ�
// �@�@�@�Ffloat �ő�
// �߂�l�Ffloat �����_�����l
//-------------------------------------------------------------
float Calculate::Random(float min, float max)
{
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_real_distribution< float > rad(min, max);

	return rad(mt);
}
//*************************************************************
//
// GameObject.h
//
// �I�u�W�F�N�g�N���X�̃e���v���[�g
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

#pragma once

//=============================================================
// �C���N���[�h
//=============================================================
#include "../../Library/System/System.h"
#include "../../Library/Utility/Math/Math.h"
#include "../../Library/Utility/Math/Matrix.h"
#include "../../Library/Utility/Math/Vector.h"

//=============================================================
// �N���X��`
//=============================================================
class CGameObject
{
public:

	//-------------------------------------------------------------
	// �R���X�g���N�^
	//-------------------------------------------------------------
	CGameObject()
		: m_position(VectorCalculate::Zero)
		, m_rotation(VectorCalculate::Zero)
		, m_scaling(VectorCalculate::One)
		, m_radius(0.0f)
		, m_mass(0.0f)
		, m_isAlive(true)
		, m_isVisible(true)
		, m_isSleep(false)
		, m_matWorld(MatrixCalculate::Identity) {};

	//-------------------------------------------------------------
	// �f�X�g���N�^
	//-------------------------------------------------------------
	virtual ~CGameObject() {};

	//-------------------------------------------------------------
	// �t�@�C���ǂݍ���
	// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
	//-------------------------------------------------------------
	virtual void LoadFile(CSystem* pSystem) {};

	//-------------------------------------------------------------
	// ������
	// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
	//-------------------------------------------------------------
	virtual void Initialize(CSystem* pSystem) {};

	//-------------------------------------------------------------
	// �X�V
	// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
	//-------------------------------------------------------------
	virtual void Update(CSystem* pSystem)
	{
		m_matWorld = MatrixCalculate::Transform(m_scaling, m_rotation, m_position);
	}

	//-------------------------------------------------------------
	// �V���h�E�T���v�����O�p
	// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
	//-------------------------------------------------------------
	virtual void ZDraw(CSystem* pSystem) {};

	//-------------------------------------------------------------
	// �`��
	// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
	//-------------------------------------------------------------
	virtual void Draw(CSystem* pSystem) {};

	//-------------------------------------------------------------
	// �Q�b�^�[
	//-------------------------------------------------------------
	Vector3 GetPosition() const { return m_position; };
	Vector3 GetRotate() const { return m_rotation; };
	Vector3 GetScale() const { return m_scaling; };
	float GetRadius() const { return m_radius; };
	bool IsAlive() const { return m_isAlive; };
	bool IsVisible() const { return m_isVisible; };
	bool IsSleep() const { return m_isSleep; };

	//-------------------------------------------------------------
	// �Z�b�^�[
	//-------------------------------------------------------------
	void SetPosition(const Vector3& value) { m_position = value; };
	void SetRotate(const Vector3& value) { m_rotation = value; };
	void SetScale(const Vector3& value) { m_scaling = value; };
	void SetAlive(const bool value) { m_isAlive = value; };
	void SetVisible(const bool value) { m_isVisible = value; };
	void SetSleep(const bool value) { m_isSleep = value; };

protected:
	Vector3 m_position;
	Vector3 m_rotation;
	Vector3 m_scaling;
	Matrix m_matWorld;
	float m_radius;
	float m_mass;
	bool m_isAlive;
	bool m_isVisible;
	bool m_isSleep;
};
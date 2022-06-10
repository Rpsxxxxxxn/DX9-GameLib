//********************************************************************************
//
// Damage.hpp
//
// �_���[�W�ʕ\���p�N���X
//
// Date: 2019/07/30 �X�V
//
//********************************************************************************

//================================================================================
// �C���N���[�h
//================================================================================
#include "Damage.hpp"

using namespace FileConst;

//================================================================================
// �N���X��`
//================================================================================

//--------------------------------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------------------------------
CShowDamage::CShowDamage()
{
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
CShowDamage::~CShowDamage()
{
	for (auto w : m_vecSprite)
	{
		SafeDelete(w.pSpriteValue);
	}
	m_vecSprite.clear();
}

//--------------------------------------------------------------------------------
// �t�@�C���ǂݍ���
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CShowDamage::LoadFile(CSystem * pSystem)
{
	pSystem->LoadTexture(TEXTURE_NUMBER, "Resource/Textures/Share/Number.png");
}

//--------------------------------------------------------------------------------
// �X�V
// ���@���FCSystem*			�V�X�e���N���X�̃|�C���^
// �@�@�@�@CPlayerCamera	�J�����N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CShowDamage::Update(CSystem * pSystem, CPlayerCamera* pCamera)
{
	EraseDamage();

	auto device = pSystem->GetDevice();
	auto camera = pCamera->GetCamera();

	for (auto w : m_vecSprite)
	{
		Vector3 screenPos = Calculate::WorldToScreen(device, &camera->GetViewMatrix(), &camera->GetProjectionMatrix(), &w.vecPos);

		w.pSpriteValue->SetPosition(Vector2(screenPos.x, screenPos.y));
	}
}

//--------------------------------------------------------------------------------
// �`��
// ���@���FCSystem*	�V�X�e���N���X�̃|�C���^
//--------------------------------------------------------------------------------
void CShowDamage::Draw(CSystem * pSystem)
{
	for (auto w : m_vecSprite)
	{
		w.pSpriteValue->Draw(pSystem);
	}
}

//--------------------------------------------------------------------------------
// �_���[�W�\���̒ǉ�
// ���@���FCSystem*			�V�X�e���N���X�̃|�C���^
// �@�@�@�@const Vector3	�\������ʒu�x�N�g��
// �@�@�@�@const int		�_���[�W��
//--------------------------------------------------------------------------------
void CShowDamage::AddDamage(CSystem* pSystem, const Vector3 vecPos, const int damage)
{
	DamageData w;

	w.vecPos.x = vecPos.x + Calculate::Random(-0.3f, 0.3f);
	w.vecPos.y = vecPos.y + Calculate::Random(-0.3f, 0.3f);
	w.vecPos.z = vecPos.z + Calculate::Random(-0.3f, 0.3f);

	w.pSpriteValue = new CSpriteValue(pSystem->GetTextureData(TEXTURE_NUMBER));

	w.pSpriteValue->SetSize(60, 60);
	w.pSpriteValue->SetValue(damage);
	w.pSpriteValue->SetEndTime(1000);
	w.pSpriteValue->SetNewAlpha(0);

	m_vecSprite.push_back(w);
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// �_���[�W�\���̍폜
//--------------------------------------------------------------------------------
void CShowDamage::EraseDamage()
{
	for (auto itr = begin(m_vecSprite); itr != end(m_vecSprite); )
	{
		if ((*itr).pSpriteValue->IsDestroy())
		{
			SafeDelete((*itr).pSpriteValue);
	
			itr = m_vecSprite.erase(itr);
		}
		else { itr++; };
	}
}

//*************************************************************
//
// Singleton.cpp
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "Singleton.h"

std::vector<CSingletonFinalizer::
	FinalizerFunc>CSingletonFinalizer::m_finalizer = {};

//-------------------------------------------------------------
// �t�@�C�i���C�Y�̒ǉ�
//-------------------------------------------------------------
void CSingletonFinalizer::AddFinalizer(FinalizerFunc func)
{
	m_finalizer.push_back(func);
}

//-------------------------------------------------------------
// �t�@�C�i���C�Y�̎��s
//-------------------------------------------------------------
void CSingletonFinalizer::Finalize()
{
	std::reverse(m_finalizer.begin(), m_finalizer.end());
	for (auto& w : m_finalizer)
	{
		w();
	}
}
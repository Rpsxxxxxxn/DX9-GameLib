//*************************************************************
//
// Singleton.inl
//
//*************************************************************

#pragma once

//-------------------------------------------------------------
// �C���N���[�h
//-------------------------------------------------------------
#include "Singleton.h"

template<class Type> std::once_flag CSingleton<Type>::m_onceFlg;
template<class Type> Type* CSingleton<Type>::m_pInstance = nullptr;

//-------------------------------------------------------------
// �C���X�^���X�̎擾
//-------------------------------------------------------------
template<class Type>
inline Type& CSingleton<Type>::GetInstance()
{
	std::call_once(m_onceFlg, Create);
	return *m_pInstance;
}

//-------------------------------------------------------------
// �C���X�^���X�̐���
//-------------------------------------------------------------
template<class Type>
inline void CSingleton<Type>::Create()
{
	m_pInstance = new Type;
	CSingletonFinalizer::AddFinalizer(&CSingleton<Type>::Destroy);
}

//-------------------------------------------------------------
// �C���X�^���X�̔j��
//-------------------------------------------------------------
template<class Type>
inline void CSingleton<Type>::Destroy()
{
	delete m_pInstance;
	m_pInstance = nullptr;
}


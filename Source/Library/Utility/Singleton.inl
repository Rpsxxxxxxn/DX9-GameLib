//*************************************************************
//
// Singleton.inl
//
//*************************************************************

#pragma once

//-------------------------------------------------------------
// インクルード
//-------------------------------------------------------------
#include "Singleton.h"

template<class Type> std::once_flag CSingleton<Type>::m_onceFlg;
template<class Type> Type* CSingleton<Type>::m_pInstance = nullptr;

//-------------------------------------------------------------
// インスタンスの取得
//-------------------------------------------------------------
template<class Type>
inline Type& CSingleton<Type>::GetInstance()
{
	std::call_once(m_onceFlg, Create);
	return *m_pInstance;
}

//-------------------------------------------------------------
// インスタンスの生成
//-------------------------------------------------------------
template<class Type>
inline void CSingleton<Type>::Create()
{
	m_pInstance = new Type;
	CSingletonFinalizer::AddFinalizer(&CSingleton<Type>::Destroy);
}

//-------------------------------------------------------------
// インスタンスの破棄
//-------------------------------------------------------------
template<class Type>
inline void CSingleton<Type>::Destroy()
{
	delete m_pInstance;
	m_pInstance = nullptr;
}


//*************************************************************
//
// Singleton.h 
//
//*************************************************************

#pragma once

//=============================================================
// インクルード
//=============================================================
#include "../Common.h"

class CSingletonFinalizer
{
public:
	using FinalizerFunc = void(*)();
	static std::vector<FinalizerFunc> m_finalizer;

	static void AddFinalizer(FinalizerFunc func);
	static void Finalize();
};

template<class Type>
class CSingleton
{
public:
	static Type& GetInstance();

private:
	static void Create();
	static void Destroy();

	static std::once_flag m_onceFlg;
	static Type* m_pInstance;
};

#include "Singleton.inl"
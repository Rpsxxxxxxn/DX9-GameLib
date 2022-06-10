//*************************************************************
//
// Singleton.cpp
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "Singleton.h"

std::vector<CSingletonFinalizer::
	FinalizerFunc>CSingletonFinalizer::m_finalizer = {};

//-------------------------------------------------------------
// ファイナライズの追加
//-------------------------------------------------------------
void CSingletonFinalizer::AddFinalizer(FinalizerFunc func)
{
	m_finalizer.push_back(func);
}

//-------------------------------------------------------------
// ファイナライズの実行
//-------------------------------------------------------------
void CSingletonFinalizer::Finalize()
{
	std::reverse(m_finalizer.begin(), m_finalizer.end());
	for (auto& w : m_finalizer)
	{
		w();
	}
}
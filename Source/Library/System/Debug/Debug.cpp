//*************************************************************
//
// DebugText.cpp
//
// Debug用クラス
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "Debug.h"

//-------------------------------------------------------------
// シングルトン生成
//-------------------------------------------------------------
CDebugLog * CDebugLog::Create()
{
	static CDebugLog instance;
	return &instance;
}

//-------------------------------------------------------------
// INFO
//-------------------------------------------------------------
void CDebugLog::Info(string strInfo)
{
#ifdef DEBUG_MODE
	string str = "[INFO]" + strInfo;

	m_lstDebug.push_back(str);
#endif
}

//-------------------------------------------------------------
// WARNING
//-------------------------------------------------------------
void CDebugLog::Worn(string strWorn)
{
#ifdef DEBUG_MODE
	string str = "[WORN]" + strWorn;

	m_lstDebug.push_back(str);
#endif
}

//-------------------------------------------------------------
// ERROR
//-------------------------------------------------------------
void CDebugLog::Err(string strError)
{
#ifdef DEBUG_MODE
	string str = "[ERROR]" + strError;

	m_lstDebug.push_back(str);
#endif
}

//-------------------------------------------------------------
// DEBUG
//-------------------------------------------------------------
void CDebugLog::Debug(string strDebug)
{
#ifdef DEBUG_MODE
	string str = "[DEBUG]" + strDebug;

	m_lstDebug.push_back(str);
#endif
}

//-------------------------------------------------------------
// 更新
//-------------------------------------------------------------
void CDebugLog::Update()
{
#ifdef DEBUG_MODE
	while (m_lstDebug.size() > m_showDebugNum)
	{
		m_lstDebug.pop_front();
	}

	if (ImGui::Begin("Console Log"))
	{
		for (auto log : m_lstDebug)
		{
			ImGui::Text(log.c_str());
		}
	}
	ImGui::End();
#endif
}

//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
CDebugLog::CDebugLog()
	: m_showDebugNum(10)
{
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CDebugLog::~CDebugLog()
{
	m_lstDebug.clear();
}

//********************************************************************************
//
// Ranking.cpp
//
// ランキング処理
//
// Date: 2019/10/10 更新
//
//********************************************************************************

//================================================================================
// インクルード
//================================================================================
#include "Ranking.hpp"

using namespace FileConst;

const int CRanking::RANKING_MAX = 5;

//================================================================================
// クラス定義
//================================================================================

//--------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------
CRanking::CRanking()
{
}

//--------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------
CRanking::~CRanking()
{
}

//--------------------------------------------------------------------------------
// ファイル読み込み
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CRanking::LoadFile(CSystem * pSystem)
{
}

//--------------------------------------------------------------------------------
// 初期化
// 引　数：CSystem*	システムクラスのポインタ
//--------------------------------------------------------------------------------
void CRanking::Initialize(CSystem * pSystem)
{
	ReadRandking();
}

//--------------------------------------------------------------------------------
// ランキング中のタイマー取得
// 引　数：const int ランキングの番号
//--------------------------------------------------------------------------------
int CRanking::GetRankTimer(const int index)
{
	return m_ranking[index];
}

//--------------------------------------------------------------------------------
// ランキングに入力したい自己タイム
// 引　数：const int 自己タイム
//--------------------------------------------------------------------------------
void CRanking::SetTime(const int time)
{
	int rank = RANKING_MAX;
	for (int i = 0; i < RANKING_MAX; i++)
	{
		if (time != NULL) {
			if (time < m_ranking[i]) 
			{
				rank = i;
				break;
			}
		}
	}

	//ランクが初期値以外ならば
	if (rank != RANKING_MAX) {
		for (int i = RANKING_MAX - 1; i > rank; i--)
		{
			//入れ替え
			m_ranking[i] = m_ranking[i - 1];
		}
		m_ranking[rank] = time;
	}

	WriteRanking();
}

//********************************************************************************
//
// private
//
//********************************************************************************

//--------------------------------------------------------------------------------
// ランキング読み込み
//--------------------------------------------------------------------------------
void CRanking::ReadRandking()
{
	ifstream ifs;
	ifs.open("Resource/Data/Ranking/ranking.dat", ios::in);
	if (!ifs)
	{
		InitWriteRanking();
		ReadRandking();
		return;
	}

	string str;
	for (int i = 0; i < 5; i++)
	{
		getline(ifs, str);
		m_ranking[i] = atoi(str.c_str());
	}

	ifs.close();
}

//--------------------------------------------------------------------------------
// ランキング書き込み
//--------------------------------------------------------------------------------
void CRanking::WriteRanking()
{
	ofstream ofs;
	ofs.open("Resource/Data/Ranking/ranking.dat", ios::out);

	for (int i = 0; i < 5; i++)
	{
		ofs << m_ranking[i] << endl;
	}

	ofs.close();
}

//--------------------------------------------------------------------------------
// ランキング書き込み
//--------------------------------------------------------------------------------
void CRanking::InitWriteRanking()
{
	ofstream ofs;
	ofs.open("Resource/Data/Ranking/ranking.dat", ios::out);

	for (int i = 0; i < 5; i++)
	{
		ofs << (i + 5) * 30 << endl;
	}

	ofs.close();
}

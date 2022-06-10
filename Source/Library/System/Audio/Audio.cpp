//*************************************************************
//
// Audio.cpp
//
// オーディオ再生用クラス
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "Audio.h"

//-------------------------------------------------------------
// シングルトン生成
//-------------------------------------------------------------
CAudio * CAudio::Create()
{
	static CAudio instance;
	return &instance;
}

//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
CAudio::CAudio()
	: m_pXAudio2(nullptr)
	, m_pXAudioMaster(nullptr)
{
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CAudio::~CAudio()
{
	//全ての音声データを削除
	Release();

	// MasteringVoiceが生成されていれば破棄
	if (m_pXAudioMaster != nullptr)
	{
		m_pXAudioMaster->DestroyVoice();
		m_pXAudioMaster = nullptr;
	}

	// XAudio2が生成されていれば破棄
	SafeRelease(m_pXAudio2);

	// COMの破棄
	CoUninitialize();
}

//-------------------------------------------------------------
// Bgmの追加
//-------------------------------------------------------------
void CAudio::AddBgm(const int index, string strFileName)
{
	auto itr = m_mapBgm.find(index);
	if (itr != m_mapBgm.end()) return;

	auto bgm = new CBgm(m_pXAudio2, strFileName);

	m_mapBgm.insert(make_pair(index, bgm));
}

//-------------------------------------------------------------
// Seの追加
//-------------------------------------------------------------
void CAudio::AddSe(const int index, string strFileName)
{
	auto itr = m_mapSe.find(index);
	if (itr != m_mapSe.end()) return;

	auto se = new CSe(m_pXAudio2, strFileName);

	m_mapSe.insert(make_pair(index, se));
}

//-------------------------------------------------------------
// 初期化
//-------------------------------------------------------------
void CAudio::Initialize()
{
	HRESULT hr;

	// COMライブラリ初期化
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr)) MessageBox(nullptr, "COMライブラリ初期化", "エラー", MB_OK);

	// XAudio2のインスタンスを生成
	hr = XAudio2Create(&m_pXAudio2, 0);
	if (FAILED(hr)) MessageBox(nullptr, "XAudio2のインスタンスを生成", "エラー", MB_OK);

	// MasteringVoiceの生成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pXAudioMaster);
	if (FAILED(hr)) MessageBox(nullptr, "MasteringVoiceの生成", "エラー", MB_OK);

	//音量設定
	m_pXAudioMaster->SetVolume(0.0);
}

//-------------------------------------------------------------
// 更新
//-------------------------------------------------------------
void CAudio::Update()
{
	for (auto obj : m_mapBgm)
	{
		obj.second->Update();
	}

	for (auto obj : m_mapSe)
	{
		obj.second->Update();
	}
}

//-------------------------------------------------------------
// Bgm再生
//-------------------------------------------------------------
void CAudio::PlayBgm(const int index)
{
	m_mapBgm[index]->Play();
}

//-------------------------------------------------------------
// Bgm停止
//-------------------------------------------------------------
void CAudio::StopBgm(const int index)
{
	m_mapBgm[index]->Stop();
}

//-------------------------------------------------------------
// Se再生
// 引　数：string	追加したSe名
//-------------------------------------------------------------
void CAudio::PlaySe(const int index)
{
	m_mapSe[index]->Play();
}

//-------------------------------------------------------------
// Bgmを0にもどす
// 引　数：string	追加したBgm名
//-------------------------------------------------------------
void CAudio::RewindBgm(const int index)
{
	m_mapBgm[index]->Rewind();
}

//-------------------------------------------------------------
// 追加したBgmの削除
// 引　数：const string	追加したBgm名
//-------------------------------------------------------------
void CAudio::EraseBgm(const int index)
{
	//ファイルの検索
	auto itr = m_mapBgm.find(index);
	if (itr == m_mapBgm.end()) return;

	SafeDelete(itr->second);
}

//-------------------------------------------------------------
// 追加したSeの削除
// 引　数：const string	追加したSe名
//-------------------------------------------------------------
void CAudio::EraseSe(const int index)
{
	//ファイルの検索
	auto itr = m_mapSe.find(index);
	if (itr == m_mapSe.end()) return;

	SafeDelete(itr->second);
}

//-------------------------------------------------------------
// 全ての音声データを削除
//-------------------------------------------------------------
void CAudio::Release()
{
	for (auto obj : m_mapBgm)
	{
		SafeDelete(obj.second);
	}
	m_mapBgm.clear();

	for (auto obj : m_mapSe)
	{
		SafeDelete(obj.second);
	}
	m_mapSe.clear();
}

//-------------------------------------------------------------
// ボリュームの設定
//-------------------------------------------------------------
void CAudio::SetVolume(float volume)
{
	m_pXAudioMaster->SetVolume(volume);
}

//-------------------------------------------------------------
// オーディオの取得
// 戻り値：IXAudio2	オーディオクラスのポインタ
//-------------------------------------------------------------
IXAudio2 * CAudio::GetXAudio()
{
	return m_pXAudio2;
};

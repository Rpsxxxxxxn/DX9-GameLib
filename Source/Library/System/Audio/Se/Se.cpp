//*************************************************************
//
// Se.cpp
//
// Seファイル読み込み用クラス
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "Se.h"

//=============================================================
// 定数定義
//=============================================================
const int bufLen = 2;

//-------------------------------------------------------------
// コンストラクタ
// 引　数：IXAudio2*	オーディオインターフェース
// 　　　　string		ファイルパス
//-------------------------------------------------------------
CSe::CSe(IXAudio2* pXAudio, string name)
	: m_pXAudio2(pXAudio)
	, m_mmio(nullptr)
	, bufCnt(0)
	, m_volume(1.0f)
{
	this->Initialize(name);
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CSe::~CSe()
{
	this->Release();
}

//-------------------------------------------------------------
// 初期化
// 引　数：string	ファイルディレクトリ
//-------------------------------------------------------------
void CSe::Initialize(string name)
{
	this->ReadFile(name);
	this->CreateBuffer(m_pData);
}

//-------------------------------------------------------------
// 更新
//-------------------------------------------------------------
void CSe::Update()
{
	for (auto it = begin(m_vecSour); it != end(m_vecSour);)
	{
		XAUDIO2_VOICE_STATE state;
		(*it)->GetState(&state);

		//バッファキューがなくなれば消し去る
		if (state.BuffersQueued == 0)
		{
			(*it)->DestroyVoice();
			it = m_vecSour.erase(it);
		}
		else {
			it++;
		}
	}
}

//-------------------------------------------------------------
// 開放
//-------------------------------------------------------------
void CSe::Release()
{
	for (int i = 0; i < bufLen; i++)
		SafeDeleteArray(m_pBuf);

	for (auto w : m_vecSour)
		w->DestroyVoice();

	m_vecSour.clear();
}

//-------------------------------------------------------------
// 再生
//-------------------------------------------------------------
void CSe::Play()
{
	this->AddSe();
}

//-------------------------------------------------------------
// ボリュームの設定
// 引　数：float	音の大きさ
//-------------------------------------------------------------
void CSe::SetVolume(float volume)
{
	m_volume = volume;

	for (auto w : m_vecSour)
		w->SetVolume(volume);
}

//-------------------------------------------------------------
// ボリュームの取得
// 戻り値：float	現在の大きさ
//-------------------------------------------------------------
float CSe::GetVolume()
{
	return m_volume;
}

//-------------------------------------------------------------
// Seの追加1
//-------------------------------------------------------------
void CSe::AddSe()
{
	IXAudio2SourceVoice* pWork;

	m_pXAudio2->CreateSourceVoice(&pWork, &m_fmt);

	pWork->SetVolume(m_volume);
	m_vecSour.push_back(pWork);
	pWork->FlushSourceBuffers();
	pWork->SubmitSourceBuffer(&m_pBuffer);
	pWork->Start();
}

//-------------------------------------------------------------
// ファイル読み込み
//-------------------------------------------------------------
void CSe::ReadFile(string name)
{
	MMIOINFO info = { 0 };
	MMRESULT mret;
	MMCKINFO riff_chunk;
	MMCKINFO chunk;

	m_mmio = mmioOpen((LPSTR)name.c_str(), &info, MMIO_READ);
	if (!m_mmio) MessageBox(NULL, "MMIOオープン失敗", "エラー", MB_OK); 

	riff_chunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mret = mmioDescend(m_mmio, &riff_chunk, NULL, MMIO_FINDRIFF);
	if (mret != MMSYSERR_NOERROR) MessageBox(NULL, "wave", "エラー", MB_OK); 

	chunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mret = mmioDescend(m_mmio, &chunk, &riff_chunk, MMIO_FINDCHUNK);
	if (mret != MMSYSERR_NOERROR) MessageBox(NULL, "fmt", "エラー", MB_OK); 

	memset(&m_fmt, 0, sizeof(WAVEFORMATEX));
	{
		DWORD size = mmioRead(m_mmio, (HPSTR)&m_fmt, chunk.cksize);
		if (size != chunk.cksize) MessageBox(NULL, "fmt", "エラー", MB_OK); 
	}

	chunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mret = mmioDescend(m_mmio, &chunk, &riff_chunk, MMIO_FINDCHUNK);
	if (mret != MMSYSERR_NOERROR) MessageBox(NULL, "data", "エラー", MB_OK);

	m_pData = chunk.cksize;
}

//-------------------------------------------------------------
// バッファの生成
// 引　数：const int バッファカウント
// 　　　：const int バッファの長さ
//-------------------------------------------------------------
void CSe::CreateBuffer(const int bufferLen)
{
	m_pBuffer = XAUDIO2_BUFFER();
	m_pBuf = new BYTE[bufferLen];
	int size = mmioRead(m_mmio, (HPSTR)m_pBuf, bufferLen);
	m_pBuffer.AudioBytes = size;
	m_pBuffer.pAudioData = m_pBuf;
}
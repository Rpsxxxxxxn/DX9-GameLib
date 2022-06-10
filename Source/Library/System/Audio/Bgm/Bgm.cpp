//*************************************************************
//
// Bgm.cpp
//
// Bgmファイル読み込み用クラス
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "Bgm.h"

//=============================================================
// 定数定義
//=============================================================
const int bufLen = 2;

//-------------------------------------------------------------
// コンストラクタ
// 引　数：IXAudio2*	オーディオインターフェース
// 　　　　string		ファイルパス
//-------------------------------------------------------------
CBgm::CBgm(IXAudio2* pXAudio, string name)
	: m_pXAudio2(pXAudio)
	, m_mmio(nullptr)
	, bufCnt(0)
	, m_loaded(0)
	, m_loopPoint(0)
{
	Initialize(name);
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CBgm::~CBgm()
{
	Release();
}

//-------------------------------------------------------------
// 初期化
// 引　数：string	ファイルディレクトリ
//-------------------------------------------------------------
void CBgm::Initialize(string name)
{
	ReadFile(name);
	m_pBuffer = { 0 };

	for (int i = 0; i < bufLen; i++)
		m_pBuf[i] = new BYTE[m_fmt.nAvgBytesPerSec];

	HRESULT hr = m_pXAudio2->CreateSourceVoice(&m_pSource, &m_fmt);
	if (FAILED(hr))	MessageBox(NULL, "CreateSourceVoice", "エラー", MB_OK);
}

//-------------------------------------------------------------
// 更新
//-------------------------------------------------------------
void CBgm::Update()
{
	XAUDIO2_VOICE_STATE state;
	m_pSource->GetState(&state);

	if (state.BuffersQueued < 2)
	{
		if (m_loaded > m_chunkSize)
		{
			Seek(m_loopPoint, SEEK_SET);
			m_loaded = m_loopPoint;
		}

		if (m_fmt.nAvgBytesPerSec <= 0) return;

		CreateBuffer(bufCnt, m_fmt.nAvgBytesPerSec);

		SetBuffer();

		if (bufLen <= ++bufCnt) bufCnt = 0;
	}
}

//-------------------------------------------------------------
// 開放
//-------------------------------------------------------------
void CBgm::Release()
{
	Stop();

	for (int i = 0; i < bufLen; i++)
		SafeDeleteArray(m_pBuf[i]);

	m_pSource->DestroyVoice();
}

//-------------------------------------------------------------
// シーク
//-------------------------------------------------------------
bool CBgm::Seek(int offset, int origin)
{
	if (mmioSeek(m_mmio, offset, origin))
	{
		MessageBox(nullptr, "mmioSeek", "エラー", MB_OK);
		return false;
	}

	m_chunkSize -= offset;

	return true;
}

//-------------------------------------------------------------
// 再生
//-------------------------------------------------------------
void CBgm::Play()
{
	m_pSource->Start();
}

//-------------------------------------------------------------
// 停止
//-------------------------------------------------------------
void CBgm::Stop()
{
	m_pSource->Stop();
}

//-------------------------------------------------------------
// 巻き戻し
//-------------------------------------------------------------
void CBgm::Rewind()
{
	Seek(m_loopPoint, SEEK_SET);
	m_loaded = m_loopPoint;
}

//-------------------------------------------------------------
// ボリュームの設定
// 引　数：float	音の大きさ
//-------------------------------------------------------------
void CBgm::SetVolume(float volume)
{
	m_pSource->SetVolume(volume);
}

//-------------------------------------------------------------
// ボリュームの取得
// 戻り値：float	現在の大きさ
//-------------------------------------------------------------
float CBgm::GetVolume()
{
	float volume;
	m_pSource->GetVolume(&volume);
	return volume;
}

//-------------------------------------------------------------
// ファイル読み込み
//-------------------------------------------------------------
void CBgm::ReadFile(string name)
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

	m_chunkSize = chunk.cksize;
}

//-------------------------------------------------------------
// バッファの設定
//-------------------------------------------------------------
void CBgm::SetBuffer()
{
	m_pSource->SubmitSourceBuffer(&m_pBuffer);
}

//-------------------------------------------------------------
// バッファの生成
// 引　数：const int バッファカウント
// 　　　：const int バッファの長さ
//-------------------------------------------------------------
void CBgm::CreateBuffer(const int bufferCnt, const int bufferLen)
{
	m_pBuffer = XAUDIO2_BUFFER();
	int size = mmioRead(m_mmio, (HPSTR)m_pBuf[bufferCnt], bufferLen);
	m_pBuffer.AudioBytes = size;
	m_pBuffer.pAudioData = m_pBuf[bufferCnt];
	m_loaded += size;
}

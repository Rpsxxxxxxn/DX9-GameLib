//*************************************************************
//
// System.cpp
//
// フレームワークのメインシステム
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "System.h"

//=============================================================
// クラス定義
//=============================================================

//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
CSystem::CSystem()
{
	//デバッグログクラスの生成
	m_pClsDebugLog = CDebugLog::Create();

	//レンダリングクラスの生成
	m_pClsRenderSystem = CRenderSystem::Create();

	//ファイル管理クラスの生成
	m_pClsFileManager = CFileManager::Create();

	//入力クラスの生成
	m_pClsInput = CInput::Create();

	//タイマークラスの生成
	m_pClsTimer = CTimer::Create();

	//オーディオクラスの生成
	m_pClsAudio = CAudio::Create();

	//画像描画の生成
	m_pClsSprite = CSprite::Create();

	//エフェクトクラスの生成
	m_pClsEffectSystem = CEffectSystem::Create();
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CSystem::~CSystem() {}

//-------------------------------------------------------------
// 初期化
// 引　数：CWindowSystem*	ウィンドウシステムクラスのポインタ
//-------------------------------------------------------------
void CSystem::Initialize(CWindowSystem* pWindowSystem)
{
	//ウィンドウハンドルの取得
	m_hWnd = pWindowSystem->GetHWND();

	//ウィンドウ設定のポインタ渡し
	m_pWindowSettings = pWindowSystem->GetWindowSettings();

	//レンダリングクラスの初期化
	m_pClsRenderSystem->Initialize(pWindowSystem);

	//ファイル管理クラスの初期化
	m_pClsFileManager->Initialize(m_pClsRenderSystem->GetDevice());

	//入力クラスの初期化
	m_pClsInput->Initialize(pWindowSystem);

	//エフェクトクラスの初期化
	m_pClsEffectSystem->Initialize(m_pClsRenderSystem->GetDevice(), 2000);

	//オーディオクラスの初期化
	m_pClsAudio->Initialize();

	//描画クラスの初期化
	m_pClsSprite->Initialize(m_pClsRenderSystem->GetDevice());

	//シェーダ管理の初期化
	CShaderManager::GetInstance().Initialize(m_pClsRenderSystem->GetDevice(), pWindowSystem);

	//ビューポートの初期化
	InitializeViewPort();
}

//-------------------------------------------------------------
// 更新
//-------------------------------------------------------------
void CSystem::Update()
{
	m_pClsInput->Update();
	m_pClsAudio->Update();
	m_pClsDebugLog->Update();
}

//-------------------------------------------------------------
// 解放
//-------------------------------------------------------------
void CSystem::ShutDown()
{
	m_pClsRenderSystem->Release();
	m_pClsFileManager->Release();
	m_pClsInput->Release();
}

//*************************************************************
//
// ファイル管理
//
//*************************************************************

//-------------------------------------------------------------
// XFileファイル読み込み
// 引　数：const int 	ファイルの固定番号
// 　　　　const string	ファイル名
//-------------------------------------------------------------
void CSystem::LoadXFile(const int index, const string strFileName)
{
	m_pClsFileManager->LoadXFile(index, strFileName);
}

//-------------------------------------------------------------
// Textureファイル読み込み
// 引　数：const int 	ファイルの固定番号
// 　　　　const string	ファイル名
//-------------------------------------------------------------
void CSystem::LoadTexture(const int index, const string strFileName)
{
	m_pClsFileManager->LoadTexture(index, strFileName);
}

//-------------------------------------------------------------
// ファイルの解放
//-------------------------------------------------------------
void CSystem::ReleaseAllFile()
{
	m_pClsFileManager->Release();
}

//-------------------------------------------------------------
// ロードしたファイルの解放(省メモリ)
// 引　数：const int 	ファイルの固定番号
//-------------------------------------------------------------
void CSystem::EraseData(const int index)
{
	m_pClsFileManager->EraseData(index);
}

//-------------------------------------------------------------
// テクスチャのポインタ取得
// 戻り値：LPDIRECT3DTEXTURE9	テクスチャ
//-------------------------------------------------------------
LPDIRECT3DTEXTURE9 CSystem::GetTexture(const int index)
{
	return m_pClsFileManager->GetTexture(index);
}

//-------------------------------------------------------------
// テクスチャ構造体の取得
// 戻り値：TEXTURE_DATA*		テクスチャ
//-------------------------------------------------------------
TEXTURE_DATA * CSystem::GetTextureData(const int index)
{
	return m_pClsFileManager->GetTextureData(index);
}

//-------------------------------------------------------------
// XFileクラスのポインタ取得
// 戻り値：CXFile*	XFileモデルクラス
//-------------------------------------------------------------
CXFile * CSystem::GetXFile(const int index)
{
	return m_pClsFileManager->GetXFile(index);
}

//-------------------------------------------------------------
// デバイスの取得
// 戻り値：const IDirect3DDevice9*	デバイスのポインタ
//-------------------------------------------------------------
LPDIRECT3DDEVICE9 CSystem::GetDevice() const
{
	return m_pClsRenderSystem->GetDevice();
}

//*************************************************************
//
// オーディオシステム
//
//*************************************************************

//-------------------------------------------------------------
// Bgmファイル読み込み
// 引　数：const int 	ファイルの固定番号
// 　　　　const string	ファイル名
//-------------------------------------------------------------
void CSystem::AddBgm(const int index, const string strFileName)
{
	m_pClsAudio->AddBgm(index, strFileName);
}

//-------------------------------------------------------------
// Seファイル読み込み
// 引　数：const int 	ファイルの固定番号
// 　　　　const string	ファイル名
//-------------------------------------------------------------
void CSystem::AddSe(const int index, const string strFileName)
{
	m_pClsAudio->AddSe(index, strFileName);
}

//-------------------------------------------------------------
// Bgmの再生
// 引　数：const string	追加した名前
//-------------------------------------------------------------
void CSystem::PlayBgm(const int index)
{
	m_pClsAudio->PlayBgm(index);
}

//-------------------------------------------------------------
// Seの再生
// 引　数：const string	追加した名前
//-------------------------------------------------------------
void CSystem::PlaySe(const int index)
{
	m_pClsAudio->PlaySe(index);
}

//-------------------------------------------------------------
// Bgmの停止
// 引　数：const string	追加した名前
//-------------------------------------------------------------
void CSystem::StopBgm(const int index)
{
	m_pClsAudio->StopBgm(index);
}

//-------------------------------------------------------------
// 巻き戻し
// 引　数：const string	追加したBgm名
//-------------------------------------------------------------
void CSystem::RewindBgm(const int index)
{
	m_pClsAudio->RewindBgm(index);
}

//-------------------------------------------------------------
// ファイルの解放
//-------------------------------------------------------------
void CSystem::ReleaseAllSound()
{
	m_pClsAudio->Release();
}

//-------------------------------------------------------------
// ボリュームの設定
// 引　数：float	ボリュームの設定
//-------------------------------------------------------------
void CSystem::SetVolume(float volume)
{
	m_pClsAudio->SetVolume(volume);
}

//*************************************************************
//
// 画像描画に関する
//
//*************************************************************

//-------------------------------------------------------------
// 描画
// 引　数：RECT*				ソース画像のマスク
// 　　　　LPDIRECT3DTEXTURE9	テクスチャのポインタ
// 　　　　RECT*				描画先の位置
// 　　　　DWORD				透明度
//-------------------------------------------------------------
void CSystem::DrawSprite(
	RECT * pSrcRect,
	LPDIRECT3DTEXTURE9 pTexture,
	RECT * pDstRect,
	DWORD dwColor,
	float angle
)
{
	m_pClsSprite->
		Draw(pSrcRect, pTexture, pDstRect, dwColor, angle);
}

//-------------------------------------------------------------
// スプライトレンダリング開始
// 備　考：描画前に呼び出しを必ず
//-------------------------------------------------------------
void CSystem::SpriteBegin()
{
	//レンダリング設定
	m_pClsRenderSystem->SpriteBegin();

	//描画開始
	m_pClsSprite->SpriteBegin();
}

//-------------------------------------------------------------
// スプライトレンダリング終了
// 備　考：描画後に呼び出しを必ず
//-------------------------------------------------------------
void CSystem::SpriteClose()
{
	//描画終了
	m_pClsSprite->SpriteClose();

	//レンダリング設定の初期化
	m_pClsRenderSystem->SpriteClose();
}

//*************************************************************
//
// エフェクトシステム
//
//*************************************************************

//-------------------------------------------------------------
// データの設定
// 引　数：const float 視野角（ラジアン角）
// 　　　　const float アスペクト比 (width / height)
// 　　　　const float 前方クリップ面
// 　　　　const float 後方クリップ面
//-------------------------------------------------------------
void CSystem::InitProjection(
	const float fov,
	const float aspect,
	const float nearClip, 
	const float farClip
)
{
	m_pClsEffectSystem->
		InitProjection(fov, aspect, nearClip, farClip);
}

//-------------------------------------------------------------
// エフェクトの追加
// 引　数：EFK_CHAR*	ファイル名
//-------------------------------------------------------------
void CSystem::AddEffect(const EFK_CHAR * strFileName)
{
	m_pClsEffectSystem->AddEffect(strFileName);
}

//-------------------------------------------------------------
// エフェクトの追加
// 引　数：const int	配列の番目
// 　　　　EFK_CHAR*	ファイル名
//-------------------------------------------------------------
void CSystem::AddEffect(const int index, const EFK_CHAR* strFileName)
{
	m_pClsEffectSystem->AddEffect(index, strFileName);
}

//-------------------------------------------------------------
// エフェクトの更新
// 引　数：CCamera*		カメラのポインタ
//-------------------------------------------------------------
void CSystem::UpdateEffect(CCamera * pCamera)
{
	m_pClsEffectSystem->Update(pCamera);
}

//-------------------------------------------------------------
// エフェクトの再生
// 引　数：const int	配列の番目
// 　　　　D3DXVECTOR3	3次元ベクトル
//-------------------------------------------------------------
void CSystem::PlayEffect(const int index, D3DXVECTOR3& VecPos)
{
	m_pClsEffectSystem->Play(index, VecPos);
}

//-------------------------------------------------------------
// エフェクトの再生
// 引　数：const int	配列の番目
// 　　　　D3DXVECTOR3*	3次元ベクトルポインタ
//-------------------------------------------------------------
void CSystem::PlayEffect(const int index, D3DXVECTOR3* pVecPos)
{
	m_pClsEffectSystem->Play(index, pVecPos);
}

//-------------------------------------------------------------
// カメラ座標の更新
// 引　数：Camera*	カメラクラスのポインタ
//-------------------------------------------------------------
void CSystem::SetCameraMatrix(CCamera * pCamera)
{
	m_pClsEffectSystem->Update(pCamera);
}

//-------------------------------------------------------------
// エフェクト描画
//-------------------------------------------------------------
void CSystem::DrawEffect()
{
	m_pClsEffectSystem->Draw();
}

//*************************************************************
//
// レンダリングシステム
//
//*************************************************************

//-------------------------------------------------------------
// バックバッファのクリア
// 引　数：const DWORD	色の指定
//-------------------------------------------------------------
void CSystem::Clear(const DWORD value)
{
	m_pClsRenderSystem->Clear(value);
}

//-------------------------------------------------------------
// レンダリング開始
//-------------------------------------------------------------
void CSystem::BeginRender()
{
	m_pClsRenderSystem->BeginRender();
}

//-------------------------------------------------------------
// レンダリング終了
//-------------------------------------------------------------
void CSystem::EndRender()
{
	m_pClsRenderSystem->EndRender();
}

//-------------------------------------------------------------
// 画面スワップ
//-------------------------------------------------------------
void CSystem::Present()
{
	m_pClsRenderSystem->Present();
}

//-------------------------------------------------------------
// シェーダの描画設定
// 引　数：ShaderType シェーダの種類
// 備　考：メッシュの描画前に呼び出しをしてください。
//-------------------------------------------------------------
void CSystem::SetShaderState(const DWORD value)
{
	CShaderManager::GetInstance().SetShaderState((Shader)value);
}

//*************************************************************
//
// ウィンドウ設定
//
//*************************************************************

//-------------------------------------------------------------
// ウィンドウ設定の取得
// 戻り値：const WindowSettings*	ウィンドウ構造体のポインタ
//-------------------------------------------------------------
WindowSettings CSystem::GetWindowSettings() const
{
	return m_pWindowSettings;
}

//*************************************************************
//
// インプット
//
//*************************************************************

//-------------------------------------------------------------
// キー入力情報の取得
// 引　数：BYTE	キーコード
// 備　考：キーのおしっぱなし
//-------------------------------------------------------------
bool CSystem::IsKeyDown(const BYTE keyCode)
{
	return m_pClsInput->IsKeyDown(keyCode);
}

//-------------------------------------------------------------
// キー入力情報の取得
// 引　数：BYTE	キーコード
// 備　考：キーの一度のみ
//-------------------------------------------------------------
bool CSystem::IsKeyPressed(const BYTE keyCode)
{
	return m_pClsInput->IsKeyPressed(keyCode);
}

//-------------------------------------------------------------
// キー入力情報の取得
// 引　数：BYTE	キーコード
// 備　考：キーのおしっぱなし
//-------------------------------------------------------------
bool CSystem::IsMouseDown(const BYTE mouseButton)
{
	return m_pClsInput->IsMouseDown(mouseButton);
}

//-------------------------------------------------------------
// キー入力情報の取得
// 引　数：BYTE	キーコード
// 備　考：キーの一度のみ
//-------------------------------------------------------------
bool CSystem::IsMousePressed(const BYTE mouseButton)
{
	return m_pClsInput->IsMousePressed(mouseButton);
}

//-------------------------------------------------------------
// マウスの位置
// 戻り値：POINT	X,Y
//-------------------------------------------------------------
POINT CSystem::GetMousePos() const
{
	POINT MousePos;

	GetCursorPos(&MousePos);

	ScreenToClient(m_hWnd, &MousePos);

	return MousePos;
}

//-------------------------------------------------------------
// マウスの移動量
// 戻り値：POINT	X,Y
//-------------------------------------------------------------
POINT CSystem::GetMouseMove() const
{
	return m_pClsInput->GetMouseMove();
}

//-------------------------------------------------------------
// マウスのホイール
// 戻り値：int		ホイール
//-------------------------------------------------------------
int CSystem::GetMouseWheel() const
{
	return m_pClsInput->GetMouseWheel();
}

//-------------------------------------------------------------
// パッドの取得
// 引　数：int	キーコード
// 備　考：キーのおしっぱなし
//-------------------------------------------------------------
bool CSystem::IsPadDown(const int padButton)
{
	return m_pClsInput->IsPadDown(padButton);
}

//-------------------------------------------------------------
// パッドの取得
// 引　数：int	キーコード
// 備　考：キーの一度のみ
//-------------------------------------------------------------
bool CSystem::IsPadPressed(const int padButton)
{
	return m_pClsInput->IsPadPressed(padButton);
}

//-------------------------------------------------------------
// ジョイスティックの取得
// 引　数：int	キーコード
// 備　考：キーの一度のみ
//-------------------------------------------------------------
float CSystem::GetJoyStickState(const int joyStick)
{
	return m_pClsInput->GetJoyStickState(joyStick);
}

//-------------------------------------------------------------
// コントローラーが接続されているか？
//-------------------------------------------------------------
bool CSystem::IsControllerConnect()
{
	return m_pClsInput->IsControllerConnect();
}

//*************************************************************
//
// 時間に関する
//
//*************************************************************

//-------------------------------------------------------------
// タイマーの更新
//-------------------------------------------------------------
void CSystem::UpdateTimer()
{
	m_pClsTimer->Update();
}

//-------------------------------------------------------------
// １ループ当たりの時間
//-------------------------------------------------------------
double CSystem::GetDeltaTime() const
{
	return m_pClsTimer->GetDeltaTime();
}

//*************************************************************
//
// デバッグ用に関する
//
//*************************************************************

//-------------------------------------------------------------
// INFO
// 引　数：string	テキスト
// 備　考：[Info]
//-------------------------------------------------------------
void CSystem::DebugInfo(string strInfo)
{
	m_pClsDebugLog->Info(strInfo);
}

//-------------------------------------------------------------
// WARNING
// 引　数：string	テキスト
// 備　考：[WARNING]
//-------------------------------------------------------------
void CSystem::DebugWorn(string strWorn)
{
	m_pClsDebugLog->Worn(strWorn);
}

//-------------------------------------------------------------
// ERROR
// 引　数：string	テキスト
// 備　考：[ERROR]
//-------------------------------------------------------------
void CSystem::DebugErr(string strError)
{
	m_pClsDebugLog->Err(strError);
}

//-------------------------------------------------------------
// DEBUG
// 引　数：string	テキスト
// 備　考：[DEBUG]
//-------------------------------------------------------------
void CSystem::DebugText(string strDebug)
{
	m_pClsDebugLog->Debug(strDebug);
}

//-------------------------------------------------------------
// ビューポートの初期化
//-------------------------------------------------------------
void CSystem::InitializeViewPort()
{
	m_viewport.X = 0;
	m_viewport.Y = 0;
	m_viewport.Width = m_pWindowSettings.Width;
	m_viewport.Height = m_pWindowSettings.Height;
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
	m_pClsRenderSystem->GetDevice()->SetViewport(&m_viewport);
}

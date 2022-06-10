//*************************************************************
//
// デプスレンダリング
//
// ShadowDepth.cpp
//
// Cascaded Shadow Maps
//
//*************************************************************

//=============================================================
// マクロ定義
//=============================================================
#define SHADOW_MAP_FORMAT D3DFMT_R32F
#define SHADOW_MAP_SIZE   4000
#define SHADOW_MAP_NEAR_CLIP 0.1f
#define SHADOW_MAP_FAR_CLIP 100.0f

#include "ShadowDepth.h"

//=============================================================
// クラス定義
//=============================================================

//-------------------------------------------------------------
// コンストラクタ
// 引　数：const UINT	横幅
// 　　　　const UINT	縦幅
//-------------------------------------------------------------
CShadowDepth::CShadowDepth(const UINT width, const UINT height)
	: m_width(width)
	, m_height(height)
{
	m_blendDistance = 0.0f;
	m_ppCascadePlanes = new D3DXPLANE*[CASCADE_SPLIT_COUNT];

	for (int i = 0; i < CASCADE_SPLIT_COUNT; i++)
	{
		this->m_pShadow[i] = nullptr;
		this->m_pShadow[i] = nullptr;
		this->m_pShadowZ[i] = nullptr;
		this->m_ppCascadePlanes[i] = new D3DXPLANE[6];

		m_previousMinValues[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
CShadowDepth::~CShadowDepth()
{
	for (int i = 0; i < CASCADE_SPLIT_COUNT; i++)
	{
		delete[] m_ppCascadePlanes[i];
		m_pShadow[i]->Release();
		m_pShadowZ[i]->Release();
		m_pShadowSurf[i]->Release();
	}
	delete[] m_ppCascadePlanes;
}

//-------------------------------------------------------------
// 初期化
// 引　数：LPDIRECT3DDEVICE9 デバイスのポインタ
//-------------------------------------------------------------
void CShadowDepth::Initialize(LPDIRECT3DDEVICE9 pDevice)
{
	CShaderBase::Initialize(pDevice, "Resource/Shaders/shadow_depth.fx");

	for (int i = 0; i < CASCADE_SPLIT_COUNT; i++) 
	{
		m_pDevice->CreateTexture(
			SHADOW_MAP_SIZE,
			SHADOW_MAP_SIZE,
			1,
			D3DUSAGE_RENDERTARGET,
			SHADOW_MAP_FORMAT, 
			D3DPOOL_DEFAULT, 
			&m_pShadow[i],
			NULL);
		
		m_pShadow[i]->GetSurfaceLevel(0, &m_pShadowSurf[i]);

		m_pDevice->CreateDepthStencilSurface(
			SHADOW_MAP_SIZE,
			SHADOW_MAP_SIZE, 
			D3DFMT_D16, 
			D3DMULTISAMPLE_NONE,
			0,
			TRUE,
			&m_pShadowZ[i],
			NULL);

		D3DXMatrixIdentity(&m_matViewProj[i]);
	}

	m_radii[0] = SHADOW_MAP_FAR_CLIP * 0.1f;
	m_radii[1] = SHADOW_MAP_FAR_CLIP * 0.5f;
	m_radii[2] = SHADOW_MAP_FAR_CLIP;

	CalculateShadowMapSplitDepths();
}

//-------------------------------------------------------------
// 更新
// 引　数：Engine*	エンジンクラスのポインタ
//-------------------------------------------------------------
void CShadowDepth::Update()
{
}

//-------------------------------------------------------------
// デプスレンダリング開始
//-------------------------------------------------------------
void CShadowDepth::StartDepthRender(const int index)
{
	m_pDevice->GetRenderTarget(0, &m_pOldBackBuffer);
	m_pDevice->GetDepthStencilSurface(&m_pOldZBuffer);
	m_pDevice->GetViewport(&m_oldViewport);

	m_pDevice->SetRenderTarget(0, m_pShadowSurf[index]);
	m_pDevice->SetDepthStencilSurface(m_pShadowZ[index]);

	D3DVIEWPORT9 view;

	view.Width = SHADOW_MAP_SIZE;
	view.Height = SHADOW_MAP_SIZE;
	view.MinZ = 0.0f;
	view.MaxZ = 1.0f;
	view.X = 0;
	view.Y = 0;

	m_pDevice->SetViewport(&view);

	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);

	int i = index;
	this->SetValue("g_iSliceNum", i);
}

//-------------------------------------------------------------
// デプスレンダリング終了
//-------------------------------------------------------------
void CShadowDepth::EndDepthRender()
{
	m_pDevice->SetRenderTarget(0, m_pOldBackBuffer);
	m_pDevice->SetDepthStencilSurface(m_pOldZBuffer);
	m_pDevice->SetViewport(&m_oldViewport);
	m_pOldBackBuffer->Release();
	m_pOldZBuffer->Release();

	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFF000000, 1.0f, 0);
}

//-------------------------------------------------------------
// シャドウテクスチャの取得
//-------------------------------------------------------------
LPDIRECT3DTEXTURE9 CShadowDepth::GetTexture(const int index) const
{
	return m_pShadow[index];
}

//-------------------------------------------------------------
// 視推台の計算
//-------------------------------------------------------------
void CShadowDepth::CalculateCascadePlanes()
{
	for (int i = 0; i < CASCADE_SPLIT_COUNT; i++)
	{
		//手前のプレート計算
		m_ppCascadePlanes[i][0].a = m_matViewProj[i]._14 + m_matViewProj[i]._13;
		m_ppCascadePlanes[i][0].b = m_matViewProj[i]._24 + m_matViewProj[i]._23;
		m_ppCascadePlanes[i][0].c = m_matViewProj[i]._34 + m_matViewProj[i]._33;
		m_ppCascadePlanes[i][0].d = m_matViewProj[i]._44 + m_matViewProj[i]._43;
		D3DXPlaneNormalize(&m_ppCascadePlanes[i][0], &m_ppCascadePlanes[i][0]);

		// 奥行きのプレートの計算
		m_ppCascadePlanes[i][1].a = m_matViewProj[i]._14 - m_matViewProj[i]._13;
		m_ppCascadePlanes[i][1].b = m_matViewProj[i]._24 - m_matViewProj[i]._23;
		m_ppCascadePlanes[i][1].c = m_matViewProj[i]._34 - m_matViewProj[i]._33;
		m_ppCascadePlanes[i][1].d = m_matViewProj[i]._44 - m_matViewProj[i]._43;
		D3DXPlaneNormalize(&m_ppCascadePlanes[i][1], &m_ppCascadePlanes[i][1]);

		// 左のプレート計算
		m_ppCascadePlanes[i][2].a = m_matViewProj[i]._14 + m_matViewProj[i]._11;
		m_ppCascadePlanes[i][2].b = m_matViewProj[i]._24 + m_matViewProj[i]._21;
		m_ppCascadePlanes[i][2].c = m_matViewProj[i]._34 + m_matViewProj[i]._31;
		m_ppCascadePlanes[i][2].d = m_matViewProj[i]._44 + m_matViewProj[i]._41;
		D3DXPlaneNormalize(&m_ppCascadePlanes[i][2], &m_ppCascadePlanes[i][2]);

		// 右のプレート計算
		m_ppCascadePlanes[i][3].a = m_matViewProj[i]._14 - m_matViewProj[i]._11;
		m_ppCascadePlanes[i][3].b = m_matViewProj[i]._24 - m_matViewProj[i]._21;
		m_ppCascadePlanes[i][3].c = m_matViewProj[i]._34 - m_matViewProj[i]._31;
		m_ppCascadePlanes[i][3].d = m_matViewProj[i]._44 - m_matViewProj[i]._41;
		D3DXPlaneNormalize(&m_ppCascadePlanes[i][3], &m_ppCascadePlanes[i][3]);

		// 上のプレート計算
		m_ppCascadePlanes[i][4].a = m_matViewProj[i]._14 - m_matViewProj[i]._12;
		m_ppCascadePlanes[i][4].b = m_matViewProj[i]._24 - m_matViewProj[i]._22;
		m_ppCascadePlanes[i][4].c = m_matViewProj[i]._34 - m_matViewProj[i]._32;
		m_ppCascadePlanes[i][4].d = m_matViewProj[i]._44 - m_matViewProj[i]._42;
		D3DXPlaneNormalize(&m_ppCascadePlanes[i][4], &m_ppCascadePlanes[i][4]);

		// 下のプレート計算
		m_ppCascadePlanes[i][5].a = m_matViewProj[i]._14 + m_matViewProj[i]._12;
		m_ppCascadePlanes[i][5].b = m_matViewProj[i]._24 + m_matViewProj[i]._22;
		m_ppCascadePlanes[i][5].c = m_matViewProj[i]._34 + m_matViewProj[i]._32;
		m_ppCascadePlanes[i][5].d = m_matViewProj[i]._44 + m_matViewProj[i]._42;
		D3DXPlaneNormalize(&m_ppCascadePlanes[i][5], &m_ppCascadePlanes[i][5]);
	}
}

//-------------------------------------------------------------
// 分割シャドウマップの深度計算
//-------------------------------------------------------------
void CShadowDepth::CalculateShadowMapSplitDepths()
{
	m_shadowSplitDepths[0] = SHADOW_MAP_NEAR_CLIP;
	m_shadowSplitDepths[1] = SHADOW_MAP_FAR_CLIP * 0.1f;
	m_shadowSplitDepths[2] = SHADOW_MAP_FAR_CLIP * 0.5f;
	m_shadowSplitDepths[3] = SHADOW_MAP_FAR_CLIP;
}

//-------------------------------------------------------------
// シャドウマップ行列の再計算
// 引　数：Camera			カメラクラス
// 　　　　D3DXVECTOR3*		ライト位置ベクトルのポインタ
//-------------------------------------------------------------
void CShadowDepth::RecalculateShadowMapMatrices(CCamera * camera)
{
	for (int i = 0; i < CASCADE_SPLIT_COUNT; i++)
	{
		CalculateShadowMatrices(camera, i, false);
	}
}

//-------------------------------------------------------------
// ライトビュー行列
// 引　数：D3DXMATRIX*	ビュー行列のポインタ
//-------------------------------------------------------------
void CShadowDepth::SetLightViewMatrix(D3DXMATRIX * pMatView)
{
	m_matLView = *pMatView;
}

//-------------------------------------------------------------
// ライトプロジェクション行列
// 引　数：D3DXMATRIX*	プロジェクション行列のポインタ
//-------------------------------------------------------------
void CShadowDepth::SetLightProjMatrix(D3DXMATRIX * pMatProj)
{
	m_matLProj = *pMatProj;
}

//-------------------------------------------------------------
// ライトプロジェクション行列
// 引　数：D3DXVECTOR3*		ライト位置ベクトルのポインタ
//-------------------------------------------------------------
void CShadowDepth::SetLightPosition(D3DXVECTOR3 * pVecLightPos)
{
	m_vecLightPosition = *pVecLightPos;
}

//-------------------------------------------------------------
// ビュープロジェクションの行列を取得
// 引　数：const int		分割番号
//-------------------------------------------------------------
D3DXMATRIX CShadowDepth::GetViewProjMatrix(const int index)
{
	return m_matViewProj[index];
}

//-------------------------------------------------------------
// 分割シャドウの行列計算
// 引　数：Camera カメラクラス
// 　　　　int	  分割番号
// 　　　　bool	  更新をするか
//-------------------------------------------------------------
void CShadowDepth::CalculateShadowMatrices(CCamera * camera, int i, bool forceUpdate)
{
	D3DXVECTOR4 cameraEye(camera->m_vecEye, 1.0f);
	D3DXVECTOR4 cameraFront(camera->m_vecForward, 1.0f);
	D3DXVECTOR4 cameraRight(camera->m_vecRight, 1.0f);
	D3DXVECTOR4 cameraUp(camera->m_vecUp, 1.0f);
	float aspect = (float)m_width / (float)m_height;
	float tmp = camera->GetFov();

	// 手前のプレート
	float halfNearHeight = tmp * m_shadowSplitDepths[i];
	float halfNearWidth = halfNearHeight * aspect;

	// 手前の4つの点計算
	D3DXVECTOR4	nearCenter = cameraEye + cameraFront * this->m_shadowSplitDepths[i];
	D3DXVECTOR4	nearTopLeft = nearCenter + (cameraUp * halfNearHeight) - (cameraRight * halfNearWidth);
	D3DXVECTOR4	nearTopRight = nearCenter + (cameraUp * halfNearHeight) + (cameraRight * halfNearWidth);
	D3DXVECTOR4	nearBottomLeft = nearCenter - (cameraUp * halfNearHeight) - (cameraRight * halfNearWidth);
	D3DXVECTOR4	nearBottomRight = nearCenter - (cameraUp * halfNearHeight) + (cameraRight * halfNearWidth);

	// 奥行きのプレート計算
	float halfFarHeight = tmp * this->m_shadowSplitDepths[i + 1];
	float halfFarWidth = halfFarHeight * aspect;

	// 奥行きの4つの点計算
	D3DXVECTOR4 farCenter = cameraEye + cameraFront * this->m_shadowSplitDepths[i + 1];
	D3DXVECTOR4 farTopLeft = farCenter + (cameraUp * halfFarHeight) - (cameraRight * halfFarWidth);
	D3DXVECTOR4	farTopRight = farCenter + (cameraUp * halfFarHeight) + (cameraRight * halfFarWidth);
	D3DXVECTOR4	farBottomLeft = farCenter - (cameraUp * halfFarHeight) - (cameraRight * halfFarWidth);
	D3DXVECTOR4	farBottomRight = farCenter - (cameraUp * halfFarHeight) + (cameraRight * halfFarWidth);

	// 視推台の8点を入れる
	D3DXVECTOR4 frustumPoints[8] =
	{
		nearTopLeft, nearTopRight, nearBottomLeft, nearBottomRight,
		farTopLeft, farTopRight, farBottomLeft, farBottomRight
	};

	// ポイントをワールド空間からライトの均一空間に変換します。
	for (int index = 0; index < 8; index++)
	{
		frustumPoints[index].w = 1.0f;
		D3DXVec4Transform(&frustumPoints[index], &frustumPoints[index], &m_matLView);
	}

	// 正投影のmin（near plane）およびmax（far plane）値を計算します。
	float infinity = std::numeric_limits<float>::infinity();
	D3DXVECTOR3 minValue(infinity, infinity, infinity);
	D3DXVECTOR3 maxValue(-infinity, -infinity, -infinity);
	for (int index = 0; index < 8; index++)
	{
		D3DXVECTOR3 vec3(frustumPoints[index].x, frustumPoints[index].y, frustumPoints[index].z);

		// 最小点と最大点を見つけます。
		D3DXVec3Minimize(&minValue, &minValue, &vec3);
		D3DXVec3Maximize(&maxValue, &maxValue, &vec3);
	}

	// 前の最小ポイント間の距離を計算します。
	D3DXVECTOR3 tmpVec = minValue - m_previousMinValues[i];
	float len = D3DXVec3Length(&tmpVec);
	if (len > m_radii[i])
	{
		m_previousMinValues[i] = minValue;
	}
	else
	{
		// ポイントがまだ半径内にあり、別のカスケードによって強制的に更新されない場合、
		// カスケードを再計算する必要はありません
		if (!forceUpdate)
		{
			//return;
		}
	}

	D3DXVECTOR3 tmpNearPlanePoint = minValue;
	D3DXVECTOR3 tmpFarPlanePoint = maxValue;
	// 潜在的なオクルーダーを含めるために、ニアプレーンをライトに近づけます
	if (m_vecLightPosition.z < 0.0f)
	{
		m_vecLightPosition *= -1.0f;
	}
	// すべてのカスケードに同じニアプレーンを設定
	tmpNearPlanePoint -= m_vecLightPosition * (float)(50 * (i + 1));
	tmpNearPlanePoint += m_vecLightPosition * m_radii[i]; //太陽に近い平面の近くに移動します
	tmpFarPlanePoint -= m_vecLightPosition * m_radii[i]; //太陽から遠い平面を移動します
	float nearPlane = tmpNearPlanePoint.z;
	float farPlane = maxValue.z;

	// このカスケード/錐台スライスの正投影を作成します。
	// ブレンド距離を追加して、シャドウマップ間でブレンドするために投影が互いにオーバーラップするようにします
	D3DXMatrixIdentity(&m_matLProj);

	// ブレンド距離により投影が重なります
	// このオーバーラップは、カスケードが交わるエッジを滑らか/ぼかすために使用されます
	D3DXMatrixOrthoOffCenterLH(&m_matLProj,
		minValue.x - m_blendDistance - m_radii[i], // 左
		maxValue.x + m_blendDistance + m_radii[i], // 右
		minValue.y - m_blendDistance - m_radii[i], // 下
		maxValue.y + m_blendDistance + m_radii[i], // 上
		nearPlane, farPlane);

	m_matViewProj[i] = m_matLView * m_matLProj;

	// 他のすべてのカスケード（下）を強制的に更新します（高いカスケードは下よりも頻繁に更新します）
	if (i > 0)
	{
		CalculateShadowMatrices(camera, i - 1, true);
	}
}

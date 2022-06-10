//*************************************************************
//
// Calculate.cpp
//
// 計算用クラス
//
// Date: 2019/07/30 更新
//
//*************************************************************

//=============================================================
// インクルード
//=============================================================
#include "Calculate.h"

//-------------------------------------------------------------
// ワールド座標からスクリーン座標に変換
// 引　数：LPDIRECT3DDEVICE9 デバイスのポインタ
// 　　　：Matrix			 ビュー行列
// 　　　：Matrix			 プロジェクション行列
// 　　　：Vector3			 変換元3次元ベクトル
// 戻り値：変換後の3次元ベクトル(X,Yに数値が入り、ZにはXが入ります。)
//-------------------------------------------------------------
Vector3 Calculate::WorldToScreen(
	LPDIRECT3DDEVICE9 pDevice,
	const D3DXMATRIX* mView,
	const D3DXMATRIX* mProj,
	const Vector3* vFrom
)
{
	D3DXMATRIX mWorld;
	Vector3 Out = { 0.0f,0.0f,0.0f };

	D3DXMatrixIdentity(&mWorld);

	D3DVIEWPORT9 view;

	pDevice->GetViewport(&view);

	D3DXVec3Project(&Out, vFrom, &view, mProj, mView, &mWorld);

	return Out;
}

//-------------------------------------------------------------
// イージング関数
// 引　数：double 現在の経過時間
// 　　　：double 最大時間
// 　　　：double 最終位置
// 　　　：double 初期位置
// 戻り値：経過時間の位置
//-------------------------------------------------------------
double Calculate::OutQuart(
	double time, 
	double totaltime,
	double max, 
	double min
)
{
	max -= min;
	time = time / totaltime - 1;
	return -max * (time * time * time * time - 1) + min;
}

//-------------------------------------------------------------
// イージング関数
// 引　数：double 現在の経過時間
// 　　　：double 最大時間
// 　　　：double 最終位置
// 　　　：double 初期位置
// 戻り値：経過時間の位置
//-------------------------------------------------------------
double Calculate::InQubic(
	double time,
	double totaltime, 
	double max, 
	double min
)
{
	max -= min;
	time /= totaltime;
	return max * time * time * time * min;
}

//-------------------------------------------------------------
// 範囲の中で現在の進行度を算出します。
// 引　数：const float	表示先の横幅
// 　　　：const float	計算元の最大数値
// 　　　：const float	現在の数値
//-------------------------------------------------------------
float Calculate::ProgressBar(
	const float width,
	const float maxValue, 
	const float nowValue
)
{
	return width / maxValue * nowValue;
}

//-------------------------------------------------------------
// 移動
//-------------------------------------------------------------
void Calculate::MoveAngle(
	Vector3* pVecPos,
	float fAngle, 
	float fSpeed
)
{
	pVecPos->x += cosf(fAngle) * fSpeed;
	pVecPos->z += sinf(fAngle) * fSpeed;
}

//-------------------------------------------------------------
// アングル調整180度
// 引　数：float 角度
// 戻り値：float 調整後の角度
//-------------------------------------------------------------
float Calculate::PI180(float fAngle)
{
	if (fAngle >= D3DX_PI) {
		fAngle -= D3DX_PI * 2.0f;
	}
	else if (fAngle < -D3DX_PI) {
		fAngle += D3DX_PI * 2.0f;
	}

	return fAngle;
}

//-------------------------------------------------------------
// アングル調整360度
// 引　数：float 角度
// 戻り値：float 調整後の角度
//-------------------------------------------------------------
float Calculate::PI360(float fAngle)
{
	if (fAngle >= D3DX_PI * 2) {
		fAngle -= D3DX_PI * 2.0f;
	}
	else if (fAngle <= 0.0f) {
		fAngle += D3DX_PI * 2.0f;
	}

	return fAngle;
}

//-------------------------------------------------------------
// 線形補間回転
//-------------------------------------------------------------
void Calculate::LerpRot(float from, float speed, float* angle)
{
	float work = Calculate::PI360(from);
	float diff = work - *angle;
	diff = Calculate::PI180(diff);
	*angle += diff * speed;
	*angle = Calculate::PI180(*angle);
}

//-------------------------------------------------------------
// ランダム 幅指定 (メルセンヌ・ツイスタ)
// 引　数：int 最小
// 　　　：int 最大
// 戻り値：int ランダム数値
//-------------------------------------------------------------
int Calculate::Random(int min, int max)
{
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution< int > rad(min, max);

	return rad(mt);
}

//-------------------------------------------------------------
// ランダム 幅指定 (メルセンヌ・ツイスタ)
// 引　数：float 最小
// 　　　：float 最大
// 戻り値：float ランダム数値
//-------------------------------------------------------------
float Calculate::Random(float min, float max)
{
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_real_distribution< float > rad(min, max);

	return rad(mt);
}
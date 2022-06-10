//*************************************************************
//
// Timer.cpp
//
// �t���[���Ǘ��Ǝ��Ԏ擾����
//
// Date: 2019/07/30 �X�V
//
//*************************************************************

//=============================================================
// �C���N���[�h
//=============================================================
#include "Timer.h"

const int CTimer::CONST_FPS = 60;

//-------------------------------------------------------------
// �V���O���g������
//-------------------------------------------------------------
CTimer * CTimer::Create()
{
	static CTimer instance;
	return &instance;
}

//-------------------------------------------------------------
// �X�V
//-------------------------------------------------------------
void CTimer::Update()
{
	CalcDeltaTime();

	if (m_isFixFpsRate)
	{
		FixFrameRate();
	}
}

//-------------------------------------------------------------
// �P���[�v������̎���
//-------------------------------------------------------------
double CTimer::GetDeltaTime() const
{
	return m_deltaTime;
}

//-------------------------------------------------------------
// 1���[�v������̎��Ԍv�Z
//-------------------------------------------------------------
void CTimer::CalcDeltaTime()
{
	m_endClock = chrono::system_clock::now();

	m_deltaTime =
		static_cast<double>(
			chrono::duration_cast<chrono::milliseconds>
			(m_endClock - m_startClock).count() * m_timeScale);

	m_startClock = chrono::system_clock::now();
}

//-------------------------------------------------------------
// �t���[����60�ɌŒ肵�܂�
//-------------------------------------------------------------
void CTimer::FixFrameRate()
{
	if (m_counter == 0)
	{
		m_startTime = timeGetTime();
	}

	if (m_counter == CONST_FPS)
	{
		int time = timeGetTime();
		m_fps = 1000.0f / ((time - m_startTime) / (float)CONST_FPS);
		m_counter = 0;
		m_startTime = time;
	}
	m_counter++;

	int tookTime = timeGetTime() - m_startTime;
	int waitTime = m_counter * 1000 / CONST_FPS - tookTime;
	if (waitTime > 0)
	{
		Sleep(waitTime);
	}
}

//-------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------
CTimer::CTimer()
	: m_startTime(0)
	, m_counter(0)
	, m_deltaTime(0.0)
	, m_timeScale(1.0)
	, m_isFixFpsRate(true)
{
	m_startClock = chrono::system_clock::now();
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
CTimer::~CTimer()
{
}

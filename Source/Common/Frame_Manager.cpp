/*===================================================================================
*    Date : 2022/12/18(日)
*        Author		: Gakuto.S
*        File		: Frame_Manager.cpp
*        Detail		:
===================================================================================*/
#include <iostream>
#include "Frame_Manager.h"
using namespace common;

// 初期化処理
FrameManager::FrameManager(long long frame)
	:m_criteria()
{
	if (frame <= 0.0)
		frame = 60ll;

	m_criteria = 1000ll / frame;
	m_oldTime = std::chrono::system_clock::now();
}

// FPSの制御処理
void FrameManager::Control()
{
	while (true)
	{
		m_currentTime = std::chrono::system_clock::now();
		long long milisec = std::chrono::duration_cast<std::chrono::milliseconds>(m_currentTime - m_oldTime).count(); //処理に要した時間をミリ秒に変換
		if (milisec >= m_criteria)
		{
#ifdef _DEBUG
			//std::cout << milisec << std::endl;
#endif // _DEBUG
			m_oldTime = m_currentTime;
			break;
		}
	}
}

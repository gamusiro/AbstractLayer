/*===================================================================================
*    Date : 2022/12/18(日)
*        Author		: Gakuto.S
*        File		: Frame_Manager.h
*        Detail		:
===================================================================================*/
#pragma once
#include <chrono>

namespace common
{
	class FrameManager
	{
	public:
		FrameManager() = delete;
		FrameManager(FrameManager&) = delete;
		FrameManager(FrameManager*) = delete;


		//**************************************************
		/// \brief コンストラクタ
		/// 
		/// \param[in] frame
		/// 
		/// \return なし
		//**************************************************
		FrameManager(long long frame);

		//**************************************************
		/// \brief 制御する
		/// 
		/// \return なし
		//**************************************************
		void Control();

	private:
		long long m_criteria;

		std::chrono::system_clock::time_point	m_currentTime;
		std::chrono::system_clock::time_point	m_oldTime;
	};
}


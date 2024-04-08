/*===================================================================================
*    Date : 2022/12/18(��)
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
		/// \brief �R���X�g���N�^
		/// 
		/// \param[in] frame
		/// 
		/// \return �Ȃ�
		//**************************************************
		FrameManager(long long frame);

		//**************************************************
		/// \brief ���䂷��
		/// 
		/// \return �Ȃ�
		//**************************************************
		void Control();

	private:
		long long m_criteria;

		std::chrono::system_clock::time_point	m_currentTime;
		std::chrono::system_clock::time_point	m_oldTime;
	};
}


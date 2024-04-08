/*===================================================================================
*    Date : 2022/10/28(Sat)
*        Author		: Gakuto.S
*        File		: Window_Win.h
*        Detail		: Create window for windows using WinAPI.
===================================================================================*/
#pragma once

#include <Windows.h>

#include "IF_Window.h"

#include "Common/Frame_Manager.h"
#include "Input/Keyboard.h"

class WindowWin : public IWindow
{
public:
	//**************************************************
	/// \brief Constructor
	/// 
	/// \param[in] width
	/// \param[in] height
	/// \param[in] caption
	/// \param[in] proc
	/// 
	/// \return none
	//**************************************************
	WindowWin(int width, int height, LPCWSTR caption, WNDPROC proc);

	//**************************************************
	/// \brief Destructor
	/// 
	/// \return none
	//**************************************************
	~WindowWin();

	//**************************************************
	/// \brief Show up a window
	/// 
	/// \return none
	//**************************************************
	void ShowUp();

	//**************************************************
	/// \brief Get window status
	/// 
	/// \return true then window closed.
	//**************************************************
	bool Close();

	//**************************************************
	/// \brief Get window handle
	/// 
	/// \return window handle of on windows -> HWND
	//**************************************************
	HWND GetHandle();

	//**************************************************
	/// \brief Get width of window size
	/// 
	/// \return width of window size
	//**************************************************
	unsigned int GetWidth();

	//**************************************************
	/// \brief Get height of window size
	/// 
	/// \return height of window size
	//**************************************************
	unsigned int GetHeight();

private:
	HWND					m_hWnd;
	HINSTANCE				m_hInstance;
	common::FrameManager	m_frameManager;
	const LPCWSTR			k_className;

public:
	static unsigned int s_FRAME_COUNT;
};
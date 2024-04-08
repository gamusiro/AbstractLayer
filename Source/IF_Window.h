/*===================================================================================
*    Date : 2023/02/01(êÖ)
*        Author		: Gakuto.S
*        File		: IF_Window.h
*        Detail		:
===================================================================================*/
#pragma once 

class IWindow
{
public:
	//**************************************************
	/// \brief Show up a window
	/// 
	/// \return none
	//**************************************************
	virtual void ShowUp() = 0;

	//**************************************************
	/// \brief Get window status
	/// 
	/// \return true then window closed.
	//**************************************************
	virtual bool Close() = 0;

	//**************************************************
	/// \brief Get width of window size
	/// 
	/// \return width of window size
	//**************************************************
	virtual unsigned int GetWidth() = 0;

	//**************************************************
	/// \brief Get height of window size
	/// 
	/// \return height of window size
	//**************************************************
	virtual unsigned int GetHeight() = 0;
};


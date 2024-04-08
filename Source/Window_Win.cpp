/*===================================================================================
*    Date : 2022/10/28(Sat)
*        Author		: Gakuto.S
*        File		: Window_Win.h
*        Detail		: Create window for windows using WinAPI.
===================================================================================*/
#include "Window_Win.h"
using namespace common;

#include <thread>
#include "Application.h"

unsigned int WindowWin::s_FRAME_COUNT = 60;

/* Constructor */
WindowWin::WindowWin(int width, int height, LPCWSTR caption, WNDPROC proc)
	:m_hInstance(GetModuleHandle(nullptr)),
	m_frameManager(long long(s_FRAME_COUNT)),
	k_className(caption)
{
	HRESULT ret = CoInitializeEx(nullptr, COINIT::COINIT_MULTITHREADED);
	if (FAILED(ret))
		throw("ComÇÃèâä˙âªÇ…é∏îs");

	WNDCLASSEX wndEx{};
	wndEx.cbSize		= sizeof(WNDCLASSEX);
	wndEx.lpfnWndProc	= proc;
	wndEx.style			= CS_CLASSDC;
	wndEx.hInstance		= m_hInstance;
	wndEx.lpszClassName = k_className;
	RegisterClassEx(&wndEx);

	m_hWnd = CreateWindow(
		k_className,
		caption,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width, height,
		nullptr,
		nullptr,
		wndEx.hInstance,
		nullptr
	);
}

/* Destructor */
WindowWin::~WindowWin()
{
	UnregisterClass(k_className, m_hInstance);
	CoUninitialize();
}

/* Show up */
void WindowWin::ShowUp()
{
	// Show up a window
	ShowWindow(m_hWnd, true);
	UpdateWindow(m_hWnd);
}

/* Get window status */
bool WindowWin::Close()
{
	/*std::thread th(Application::SoundUpdate);
	th.detach();*/

	MSG msg{};
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return true;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	m_frameManager.Control();

	BYTE state[Keyboard::k_KEY_NUM];
	auto connect = GetKeyboardState(state);
	Keyboard::SetKeyState(state);

	return false;
}

/* Get window handle */
HWND WindowWin::GetHandle()
{
	return m_hWnd;
}

/* Get width of window size */
unsigned int WindowWin::GetWidth()
{
	RECT rect{};
	GetWindowRect(m_hWnd, &rect);
	return rect.right - rect.left;
}

/* Get height of window size */
unsigned int WindowWin::GetHeight()
{
	RECT rect{};
	GetWindowRect(m_hWnd, &rect);
	return rect.bottom - rect.top;
}

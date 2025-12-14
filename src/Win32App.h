#pragma once

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class Win32Application
{
public:
	Win32Application(const HINSTANCE hInstance, const int CmdShow);
	~Win32Application();

	//! @brief Sets if the app should passthrough or accept inputs to the layer
	static void SetInputPassthrough(const bool enable);

	//! @brief Returns the HWND
	static HWND GetHWND() { return m_hwnd; }

	//! @brief Returns if the app is running
	static bool IsRunning() { return m_running; }

	//! @brief Sets if the window should run or stop
	static void SetRunning(const bool running) { m_running = running; }

	//! @brief Flush message queue
	static void MessagePump();

private:
	//! @brief Windows procedure 
	static LRESULT WinProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static bool m_running;
	static HINSTANCE m_instance;
	static HWND m_hwnd;
};
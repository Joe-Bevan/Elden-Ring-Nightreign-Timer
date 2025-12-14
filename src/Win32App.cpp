#include "Win32App.h"

#include <dwmapi.h> // For DwmExtendFrameIntoClientArea
#include <cstdint>

#include "../ImGui/backends/imgui_impl_win32.h"

/* static */ bool Win32Application::m_running = true;
/* static */ HWND Win32Application::m_hwnd = nullptr;
/* static */ HINSTANCE Win32Application::m_instance = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK Win32Application::WinProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return 0;
	}

	switch (msg)
	{
	case WM_QUIT:
	{
		m_running = false;
		return 0;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0u);
		m_running = false;
		return 0;
	}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Win32Application::Win32Application(const HINSTANCE hInstance, const int CmdShow)
{
	m_instance = hInstance;

	// Create the window class
	WNDCLASSEXW windowClass{};
	ZeroMemory(&windowClass, sizeof(windowClass));
	windowClass.cbSize = sizeof(WNDCLASSEXW);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WinProcHandler;
	windowClass.hInstance = m_instance;
	windowClass.lpszClassName = L"Elden Ring Nightreign Timer";
	RegisterClassExW(&windowClass);

	// Create the window
	m_hwnd = CreateWindowExW(
		WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,	/* EX styles (WS_EX_TRANSPARENT ignores inputs) */
		windowClass.lpszClassName,							/* Class name */
		L"Elden Ring Nightreign Timer",						/* Window title */
		WS_POPUP,											/* Window styles */
		0u,													/* X Position */
		0u,													/* Y Position */
		GetSystemMetrics(SM_CXSCREEN),						/* Width */
		GetSystemMetrics(SM_CYSCREEN),						/* Height */
		nullptr,											/* Parent */
		nullptr,											/* Menu */
		windowClass.hInstance,								/* Instance */
		nullptr												/* Custom arg */
	);

	SetLayeredWindowAttributes(m_hwnd, RGB(0, 0, 0), 255u, LWA_ALPHA);

	{
		RECT clientArea{};
		GetClientRect(m_hwnd, &clientArea);

		RECT windowArea{};
		GetWindowRect(m_hwnd, &windowArea);

		POINT diff{};
		ClientToScreen(m_hwnd, &diff);

		const MARGINS margins = {
			windowArea.left + (diff.x - windowArea.left),
			windowArea.top + (diff.y - windowArea.top),
			clientArea.right,
			clientArea.bottom,
		};

		DwmExtendFrameIntoClientArea(m_hwnd, &margins);
	}

	ShowWindow(m_hwnd, CmdShow);
	UpdateWindow(m_hwnd);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(m_hwnd);
}

Win32Application::~Win32Application()
{
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	DestroyWindow(m_hwnd);
	UnregisterClassW(L"Elden Ring Nightreign Timer", m_instance);
}

void Win32Application::MessagePump()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}

void Win32Application::SetInputPassthrough(const bool enable)
{
	// Update window style EX
	const uint32_t windowStyle = (enable) ? (WS_EX_TOPMOST | WS_EX_LAYERED) : (WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED);
	SetWindowLongPtrW(m_hwnd, GWL_EXSTYLE, windowStyle);
}

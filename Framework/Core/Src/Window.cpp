#include "Precompiled.h"	
#include "Window.h"

using namespace MoonEngine;
using namespace MoonEngine::Core;

LRESULT CALLBACK WindowMessageHandler(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;

	default:
		break;
	}

	// if we don't use the message, continue sending the message to anything else that is going to use it
	return DefWindowProc(handle, msg, wParam, lParam);
}

void Window::Initialize(HINSTANCE instance, const std::wstring& appName, uint32_t width, uint32_t height)
{
	m_instance = instance;
	m_appName = appName;

	// This is a struct that the windowsAPI uses. You need to fill it out before winAPI can make a window.
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowMessageHandler;	// when input from  the device or keyboard or anything on the device sends message, what do?
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = m_appName.c_str();

	RegisterClassEx(&wcex);

	m_screenRect = { 0, 0, (LONG)width, (LONG)height };
	AdjustWindowRect(&m_screenRect, WS_OVERLAPPEDWINDOW, FALSE);

	const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	const int winWidth = std::min(static_cast<int>(m_screenRect.right - m_screenRect.left), screenWidth);
	const int winHeight = std::min(static_cast<int>(m_screenRect.bottom - m_screenRect.top), screenHeight);
	const int left = (screenWidth - winWidth) / 2;
	const int top = (screenHeight - winHeight) / 2;

	m_screenRect.left = left;
	m_screenRect.top = top;

	m_window = CreateWindow(
		m_appName.c_str(),
		m_appName.c_str(),
		WS_OVERLAPPEDWINDOW,
		left, top,
		winWidth, winHeight,
		nullptr, nullptr,
		instance, nullptr
	);

	ShowWindow(m_window, SW_SHOWNORMAL);
	SetCursorPos(screenWidth / 2, screenHeight / 2);
	m_isActive = (m_window != nullptr);

}
void Window::Terminate()
{
	DestroyWindow(m_window);
	UnregisterClass(m_appName.c_str(), m_instance);
	m_window = nullptr;
	m_instance = nullptr;
	m_isActive = false;
}

void Window::ProcessMessaege()
{
	MSG msg{};
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (WM_QUIT == msg.message)
		{
			m_isActive = false;
		}
	}
}

HWND Window::GetWindowHandle() const
{
	return  m_window;
}
bool Window::IsActive() const
{
	return m_isActive;
}

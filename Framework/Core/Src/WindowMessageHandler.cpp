#include "Precompiled.h"

#include "WindowMessageHandler.h"
#include "DebugUtil.h"


using namespace MoonEngine;
using namespace MoonEngine::Core;

void WindowMessageHandler::Hook(HWND window, Callback cb)
{
	m_window = window;
	m_previousCallback = (Callback)GetWindowLongPtrA(window, GWLP_WNDPROC);
	SetWindowLongPtrA(window, GWLP_WNDPROC, (LONG_PTR)cb);
}

void WindowMessageHandler::Unhook()
{
	SetWindowLongPtrA(m_window, GWLP_WNDPROC, (LONG_PTR)m_previousCallback);
	m_window = nullptr;
}

LRESULT WindowMessageHandler::ForwardMessage(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ASSERT(m_previousCallback != nullptr, "WindowMessageHandler: no callback is hooked!");
	return CallWindowProcA((WNDPROC)m_previousCallback, window, msg, wParam, lParam);
}


#pragma once

namespace MoonEngine::Core
{
	class WindowMessageHandler
	{
	public:
		using Callback = LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM);

		void Hook(HWND window, Callback cb);
		void Unhook();

		LRESULT ForwardMessage(HWND window, UINT msg, WPARAM wParam, LPARAM lparam);
	private:
		HWND m_window = nullptr;
		Callback m_previousCallback = nullptr;
	};
}
#pragma once

namespace MoonEngine::Core
{
	class Window
	{
	public:
		void Initialize(HINSTANCE instance, const std::wstring& appName, uint32_t width, uint32_t height);
		void Terminate();

		void ProcessMessaege();

		HWND GetWindowHandle() const;
		bool IsActive() const;

	private:
		HINSTANCE m_instance = nullptr;
		HWND m_window = nullptr;
		RECT m_screenRect{};
		std::wstring m_appName;
		bool m_isActive = false;
	};
}
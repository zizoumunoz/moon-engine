#pragma once

namespace MoonEngine
{
	struct AppConfig
	{
		std::wstring appName = L"AppName";
		uint32_t winWidth = 1200;
		uint32_t winHeight = 720;
	};

	// final means you can't inherit from it
	class App final
	{
	public:
		void Run(const AppConfig& config);
		void Quit();

	private:
		bool m_isRunning = false;
	};


}
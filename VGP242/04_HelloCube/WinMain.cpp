#include <MoonEngine/Inc/MoonEngine.h>

#include "DoColorState.h"

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	MoonEngine::AppConfig config;
	config.appName = L"HelloCube";

	MoonEngine::App& myApp = MoonEngine::MainApp();
	myApp.AddState<DoColorState>("DoColorState");
	myApp.Run(config);
	return 0;
}
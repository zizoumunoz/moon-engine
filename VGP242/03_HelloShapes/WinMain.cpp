#include <MoonEngine/Inc/MoonEngine.h>
#include "DoSomethingState.h"
#include "DoColorState.h"

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	MoonEngine::AppConfig config;
	config.appName = L"HelloShapes";

	MoonEngine::App& myApp = MoonEngine::MainApp();
	myApp.AddState<DoSomethingState>("DoSomethingState");
	myApp.AddState<DoColorState>("DoColorState");
	myApp.Run(config);

	return 0;
}
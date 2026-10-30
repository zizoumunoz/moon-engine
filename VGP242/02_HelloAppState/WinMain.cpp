#include <MoonEngine/Inc/MoonEngine.h>
#include "MainState.h"
#include "GameState.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	MoonEngine::AppConfig config;
	config.appName = L"Hello App State";

	MoonEngine::App& myApp = MoonEngine::MainApp();
	myApp.AddState<MainState>("MainState");
	myApp.AddState<GameState>("GameState");
	myApp.Run(config);

	return 0;
}
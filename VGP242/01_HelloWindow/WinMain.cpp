
// #include </Users/zmuno/Documents/Local/moon-engine/Engine/MoonEngine/Inc/MoonEngine.h>
#include <MoonEngine/Inc/MoonEngine.h>

using namespace MoonEngine;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	AppConfig config;
	config.appName = L"Hello Window";

	App myApp;
	myApp.Run(config);

	return 0;
}
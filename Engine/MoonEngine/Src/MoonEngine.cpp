#include "Precompiled.h"
#include "MoonEngine.h"

MoonEngine::App& MoonEngine::MainApp()
{
	static App s_App;
	return s_App;
}
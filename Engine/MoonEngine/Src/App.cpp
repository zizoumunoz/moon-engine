#include "Precompiled.h"
#include "App.h"

using namespace MoonEngine;
using namespace MoonEngine::Core;

void App::Run(const AppConfig& config)
{
	LOG("App Started");

	// initialize all singletons
	Window myWindow;
	myWindow.Initialize(
		GetModuleHandle(nullptr),
		config.appName,
		config.winWidth,
		config.winHeight
	);


	// initialize starting state

	// updates
	m_isRunning = true;
	while (m_isRunning)
	{
		myWindow.ProcessMessaege();
		// updating all that needs updates
		if (!myWindow.IsActive())
		{
			Quit();
			continue;
		}
	}

	LOG("App Is Terminating");
	// terminate last state

	// terminate all singletons

	myWindow.Terminate();
}

void App::Quit()
{
	m_isRunning = false;
}
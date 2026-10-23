#include "Precompiled.h"
#include "App.h"

using namespace MoonEngine;
using namespace MoonEngine::Core;
using namespace MoonEngine::Graphics;

void App::Run(const AppConfig& config)
{
	LOG("App Started");

	// initialize all singletons and window(staticInitialize)
	Window myWindow;
	myWindow.Initialize(
		GetModuleHandle(nullptr),
		config.appName,
		config.winWidth,
		config.winHeight
	);


	auto handle = myWindow.GetWindowHandle();
	GraphicsSystem::StaticInitialize(handle, false);

	// initialize starting state

	// updates

	m_isRunning = true;
	while (m_isRunning)
	{
		myWindow.ProcessMessage();
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

	GraphicsSystem::StaticTerminate();
	myWindow.Terminate();
}

void App::Quit()
{
	m_isRunning = false;
}

void MoonEngine::App::ChangeState(const std::string& stateName)
{
	auto iter = m_appStates.find(stateName);
	if (iter != m_appStates.end())
	{
		m_nextState = iter->second.get();
	}
}

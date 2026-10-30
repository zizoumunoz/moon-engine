#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

using namespace MoonEngine;
using namespace MoonEngine::Core;
using namespace MoonEngine::Graphics;
using namespace MoonEngine::Input;

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
	InputSystem::StaticInitialize(handle);

	// initialize starting state
	ASSERT(m_currentState != nullptr, "App: no state is set");
	m_currentState->Initialize();

	InputSystem* input = InputSystem::Get();
	// updates

	m_isRunning = true;
	while (m_isRunning)
	{
		myWindow.ProcessMessage();
		input->Update();
		// updating all that needs updates
		if (!myWindow.IsActive())
		{
			Quit();
			continue;
		}

		if (m_nextState != nullptr)
		{
			m_currentState->Terminate();
			m_currentState = std::exchange(m_nextState, nullptr);
			m_currentState->Initialize();
		}

		float deltaTime = TimeUtil::GetDeltaTime();

#if defined(_DEBUG)
		if (deltaTime > 0.5f)
		{
			continue;
		}
#endif

		m_currentState->Update(deltaTime);

		GraphicsSystem* gs = GraphicsSystem::Get();
		gs->BeginRender();
		m_currentState->Render();
		m_currentState->DebugUI();
		gs->EndRender();
	}

	LOG("App Is Terminating");
	// terminate last state
	m_currentState->Terminate();

	// terminate all singletons
	InputSystem::StaticTerminate();
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

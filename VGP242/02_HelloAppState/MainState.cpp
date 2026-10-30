#include "MainState.h"

using namespace MoonEngine::Input;

void MainState::Initialize()
{
	LOG("MAIN STATE INITIALIZED");
}

void MainState::Update(float deltaTime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))
	{
		MoonEngine::MainApp().ChangeState("GameState");
	}
}

void MainState::Terminate()
{
	LOG("MAIN STATE IS TERMINATED");
}

#include "GameState.h"
using namespace MoonEngine::Input;
void GameState::Initialize()
{
	LOG("GAME STATE INITIALIZE");
}

void GameState::Update(float deltaTime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))
	{
		MoonEngine::MainApp().ChangeState("MainState");
	}
}

void GameState::Terminate()
{
	
}

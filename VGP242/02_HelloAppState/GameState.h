#pragma once

#include <MoonEngine/Inc/MoonEngine.h>

class GameState : public MoonEngine::AppState
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;
	void Terminate() override;
};
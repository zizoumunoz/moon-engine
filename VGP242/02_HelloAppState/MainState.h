#pragma once
#include <MoonEngine/Inc/MoonEngine.h>

class MainState : public MoonEngine::AppState
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;
	void Terminate() override;
};
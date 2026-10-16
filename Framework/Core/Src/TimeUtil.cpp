#include "Precompiled.h"
#include "TimeUtil.h"

using namespace MoonEngine;
using namespace MoonEngine::Core;

float TimeUtil::GetTime()
{
	static const auto startTime = std::chrono::high_resolution_clock::now();	// since this is static, it only happens once. So this will only get a value at the start
	const auto currentTime = std::chrono::high_resolution_clock::now();
	const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
	return milliseconds / 1000.0f;
}

float TimeUtil::GetDeltaTime()
{
	static auto lastCallTime = std::chrono::high_resolution_clock::now();
	const auto currentTime = std::chrono::high_resolution_clock::now();
	const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(lastCallTime - currentTime).count();
	lastCallTime = currentTime;
	return milliseconds / 1000.0f;
}
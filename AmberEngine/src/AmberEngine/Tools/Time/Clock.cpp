#include "Amberpch.h"

#include "AmberEngine\Tools\Time\Clock.h"

void Utils::Clock::Initialize()
{
	deltaTime = 0.0f;

	lastTime = currentTime;
	currentTime = std::chrono::steady_clock::now();


	isInitialized = true;
}

void Utils::Clock::Update()
{
	currentTime = std::chrono::steady_clock::now();
	elapsedTime = currentTime - lastTime;

	lastTime = currentTime;

	if (isInitialized)
	{
		deltaTime = elapsedTime.count() > 0.1 ? 0.1f : static_cast<float>(elapsedTime.count());

		timeSinceStart += deltaTime * timeScale;
	}
	else
	{
		Initialize();
	}
}

float Utils::Clock::GetFrameRate()
{
	return 1.0f / (deltaTime);
}

float Utils::Clock::GetDeltaTime()
{
	return deltaTime * timeScale;
}

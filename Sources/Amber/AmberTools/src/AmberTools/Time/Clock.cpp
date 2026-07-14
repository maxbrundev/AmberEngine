#include "Amberpch.h"

#include "AmberTools/Time/Clock.h"

void AmberTools::Time::Clock::Initialize()
{
	deltaTime = 0.0f;

	currentTime = std::chrono::steady_clock::now();
	lastTime = currentTime;

	isInitialized = true;
}

void AmberTools::Time::Clock::Update()
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

float AmberTools::Time::Clock::GetFrameRate() const
{
	return 1.0f / (deltaTime);
}

float AmberTools::Time::Clock::GetDeltaTime() const
{
	return deltaTime * timeScale;
}

#include "pch.h"

#include "Utils/Time.h"

std::chrono::steady_clock::time_point	RenderEngine::Utils::Time::__START_TIME;
std::chrono::steady_clock::time_point	RenderEngine::Utils::Time::__LAST_TIME;
std::chrono::steady_clock::time_point	RenderEngine::Utils::Time::__CURRENT_TIME;
std::chrono::duration<double>			RenderEngine::Utils::Time::__ELAPSED;

bool	RenderEngine::Utils::Time::__INITIALIZED = false;

float	RenderEngine::Utils::Time::__TIME_SCALE = 1.0f;
float	RenderEngine::Utils::Time::__DELTA_TIME = 0.0f;
float	RenderEngine::Utils::Time::__TIME_SINCE_START = 0.0f;

void RenderEngine::Utils::Time::Initialize()
{
	__DELTA_TIME = 0.0f;
	__START_TIME = std::chrono::steady_clock::now();
	__CURRENT_TIME = __START_TIME;
	__LAST_TIME = __START_TIME;

	__INITIALIZED = true;
}

void RenderEngine::Utils::Time::Update()
{
	__LAST_TIME = __CURRENT_TIME;
	__CURRENT_TIME = std::chrono::steady_clock::now();
	__ELAPSED = __CURRENT_TIME - __LAST_TIME;

	if (__INITIALIZED)
	{
		__DELTA_TIME = __ELAPSED.count() > 0.1 ? 0.1f : static_cast<float>(__ELAPSED.count());
		__TIME_SINCE_START += __DELTA_TIME * __TIME_SCALE;
	}
	else
		Initialize();
}

float RenderEngine::Utils::Time::GetFramerate()
{
	return 1.0f / (__DELTA_TIME);
}

float RenderEngine::Utils::Time::GetDeltaTime()
{
	return __DELTA_TIME * __TIME_SCALE;
}

float RenderEngine::Utils::Time::GetDeltaTimeUnscaled()
{
	return __DELTA_TIME;
}

float RenderEngine::Utils::Time::GetTimeSinceStart()
{
	return __TIME_SINCE_START;
}

float RenderEngine::Utils::Time::GetTimeScale()
{
	return __TIME_SCALE;
}

void RenderEngine::Utils::Time::Scale(float p_coeff)
{
	__TIME_SCALE *= p_coeff;
}

void RenderEngine::Utils::Time::SetTimeScale(float p_timeScale)
{
	__TIME_SCALE = p_timeScale;
}
#include "AmberEngine/Utils/Time.h"

std::chrono::steady_clock::time_point	AmberEngine::Utils::Time::__START_TIME;
std::chrono::steady_clock::time_point	AmberEngine::Utils::Time::__LAST_TIME;
std::chrono::steady_clock::time_point	AmberEngine::Utils::Time::__CURRENT_TIME;
std::chrono::duration<double>			AmberEngine::Utils::Time::__ELAPSED;

bool	AmberEngine::Utils::Time::__INITIALIZED = false;

float	AmberEngine::Utils::Time::__TIME_SCALE = 1.0f;
float	AmberEngine::Utils::Time::__DELTA_TIME = 0.0f;
float	AmberEngine::Utils::Time::__TIME_SINCE_START = 0.0f;

void AmberEngine::Utils::Time::Initialize()
{
	__DELTA_TIME = 0.0f;
	__START_TIME = std::chrono::steady_clock::now();
	__CURRENT_TIME = __START_TIME;
	__LAST_TIME = __START_TIME;

	__INITIALIZED = true;
}

void AmberEngine::Utils::Time::Update()
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

float AmberEngine::Utils::Time::GetFramerate()
{
	return 1.0f / (__DELTA_TIME);
}

float AmberEngine::Utils::Time::GetDeltaTime()
{
	return __DELTA_TIME * __TIME_SCALE;
}

float AmberEngine::Utils::Time::GetDeltaTimeUnscaled()
{
	return __DELTA_TIME;
}

float AmberEngine::Utils::Time::GetTimeSinceStart()
{
	return __TIME_SINCE_START;
}

float AmberEngine::Utils::Time::GetTimeScale()
{
	return __TIME_SCALE;
}

void AmberEngine::Utils::Time::Scale(float p_coeff)
{
	__TIME_SCALE *= p_coeff;
}

void AmberEngine::Utils::Time::SetTimeScale(float p_timeScale)
{
	__TIME_SCALE = p_timeScale;
}
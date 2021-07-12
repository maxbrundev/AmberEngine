#pragma once

#include <chrono>

namespace Utils
{
	class Clock
	{
	public:
		void Update();

		float GetFrameRate();
		float GetDeltaTime();

	private:
		void Initialize();

	private:
		bool isInitialized = false;

		std::chrono::steady_clock::time_point currentTime;
		std::chrono::steady_clock::time_point lastTime;
		std::chrono::duration<double> elapsedTime;

		float timeScale = 1.0f;
		float deltaTime = 0.0f;
		float timeSinceStart = 0.0f;

	};
}
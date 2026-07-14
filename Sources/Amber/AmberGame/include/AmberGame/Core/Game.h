#pragma once

#include "AmberUI/Canvas.h"

#include "AmberGame/Core/Context.h"
#include "AmberGame/Core/GameRenderer.h"

#include "AmberGame/Utils/FPSCounter.h"

#ifdef _DEBUG
#include "AmberGame/Debug/DriverInfo.h"
#include "AmberGame/Debug/FrameInfo.h"
#include "AmberGame/Debug/GameProfiler.h"
#endif

namespace AmberGame::Core
{
	class Game
	{
	public:
		Game(Context& p_context);
		~Game();

		void PreUpdate();
		void Update(float p_deltaTime);
		void PostUpdate();

	private:
		float m_elapsed = 0.0f;

		Context& m_context;
		AmberUI::Canvas m_canvas;

		GameRenderer m_gameRenderer;

		Utils::FPSCounter m_fpsCounter;

		#ifdef _DEBUG
		Debug::DriverInfo m_driverInfo;
		Debug::GameProfiler m_gameProfiler;
		Debug::FrameInfo m_frameInfo;
		#endif

		#ifdef _DEBUG
		bool m_showDebugInformation = true;
		#else
		bool m_showDebugInformation = false;
		#endif
	};
}

#pragma once

#include "AmberGame/Core/Context.h"
#include "AmberGame/Core/Game.h"

namespace AmberGame::Core
{
	class Application
	{
	public:
		Application();
		~Application();

		void Run();

		bool IsRunning() const;

	private:
		Context m_context;
		Game m_game;
	};
}

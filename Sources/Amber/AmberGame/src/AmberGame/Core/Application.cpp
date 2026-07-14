#include "Amberpch.h"

#include "AmberGame/Core/Application.h"

#include "AmberTools/Time/Clock.h"

AmberGame::Core::Application::Application() :
	m_game(m_context)
{
}

AmberGame::Core::Application::~Application()
{
}

void AmberGame::Core::Application::Run()
{
	AmberTools::Time::Clock clock;

	while (IsRunning())
	{
		m_game.PreUpdate();
		m_game.Update(clock.GetDeltaTime());
		m_game.PostUpdate();

		clock.Update();
	}
}

bool AmberGame::Core::Application::IsRunning() const
{
	return !m_context.window->ShouldClose();
}

#pragma once

#include <AmberEngine/Settings/RenderingSettings.h>
#include <AmberEngine/Managers/RenderingManager.h>

namespace Example
{
	class Application
	{
	private:
		AmberEngine::Managers::RenderingManager m_renderer;
	public:
		Application(const AmberEngine::Settings::RenderingSettings& p_settings);
		~Application() = default;

		void Setup();
		void Run();
	};
}
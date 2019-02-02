#pragma once

#include <AmberEngine/Managers/RenderingManager.h>

namespace Example
{
	class Application
	{
	private:
		AmberEngine::Managers::RenderingManager m_renderingManager;
	public:
		Application(const AmberEngine::Settings::RenderingSettings& p_settings);
		~Application() = default;

		void Setup();
		void Run();
	};
}
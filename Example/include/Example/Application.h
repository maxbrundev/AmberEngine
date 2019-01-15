#pragma once

#include "AmberEngine/Managers/RenderingManager.h"

namespace Example
{
	class Application
	{
	private:
		std::unique_ptr<AmberEngine::Managers::RenderingManager> m_renderingManager;
	public:
		Application();
		~Application() = default;
		void Setup();
		void Run();
	};
}
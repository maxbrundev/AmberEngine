#pragma once

#include "Managers/RenderingManager.h"

namespace RenderEngine
{
	namespace Systems
	{
		class Application
		{
		private:
			std::unique_ptr<Managers::RenderingManager> m_renderingManager;
		public:
			Application();
			~Application() =default;
			void Setup();
			void Run();
		};
	}
}
#pragma once

#include "Managers/RenderingManager.h"
#include "LowRenderer/Camera.h"

#include "Managers/ResourcesManager.h"

namespace RenderEngine
{
	namespace Systems
	{
		class RenderSystem
		{
		private:
			std::unique_ptr<Managers::RenderingManager> m_renderingManager;
			std::unique_ptr<Managers::ResourcesManager> m_resourcesManager;

		public:
			RenderSystem();
			~RenderSystem() =default;

			void Run();
		};
	}
}

#pragma once

#include "Managers/RenderingManager.h"
#include "LowRenderer/Camera.h"

#include "Managers/ResourcesManager.h"

namespace RenderEngine
{
	namespace Systems
	{
		class AmberRenderer
		{
		private:
			std::unique_ptr<Managers::RenderingManager> m_renderingManager;
			std::unique_ptr<Managers::ResourcesManager> m_resourcesManager;

			glm::vec3 m_lightPos;

			GLuint m_vao;
			GLuint m_vbo;
		public:
			AmberRenderer();
			~AmberRenderer() =default;
			void Setup();
			void Run();
		};
	}
}

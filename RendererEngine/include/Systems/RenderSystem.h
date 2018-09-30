#pragma once

#include "Context/Context.h"
#include "LowRenderer/Camera.h"
#include "Resources/Shader.h"

#include "Managers/ResourcesManager.h"
#include "UI/UISystem.h"

namespace RenderEngine
{
	namespace Systems
	{
		class RenderSystem
		{
		private:
			Core::Context m_context;

			std::unique_ptr <UISystem> m_uiSystem;
			std::unique_ptr<LowRenderer::Camera> m_camera;

			Managers::ResourcesManager m_resourcesManager;
			
			GLdouble m_deltaTime = 0.0f;
			GLdouble m_lastTime = 0.0f;

			unsigned int m_drawCallCount{0};

		public:
			RenderSystem();
			~RenderSystem();

			void InitOpenGL();
			void Run();

			void PreUpdate();
			void Update();
			void PostUpdate();
		};
	}
}
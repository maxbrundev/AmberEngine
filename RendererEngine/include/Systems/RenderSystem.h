#pragma once

#include "Context/Context.h"
#include "LowRenderer/Camera.h"
#include "Resources/Shader.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw_gl3.h"

#include "Managers/ResourcesManager.h"

namespace RenderEngine
{
	namespace Systems
	{
		class RenderSystem
		{
		private:
			Core::Context m_context;
			std::unique_ptr<LowRenderer::Camera> m_camera;
			
			GLdouble m_deltaTime = 0.0f;
			GLdouble m_lastTime = 0.0f;
			Managers::ResourcesManager m_resourcesManager;
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

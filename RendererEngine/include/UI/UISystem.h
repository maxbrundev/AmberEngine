#pragma once

#include "Context/Context.h"
#include "LowRenderer/Camera.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw_gl3.h"

namespace  RenderEngine
{
	namespace Systems
	{
		class UISystem
		{
		private:
			Core::Context& m_context;

		public:
			UISystem(Core::Context& p_context);
			~UISystem() = default;

			void PreUpdate();
			void Update(unsigned int p_drawcalls, LowRenderer::Camera& p_camera);
			void PostUpdate();
			void Close();
		};
	}
}

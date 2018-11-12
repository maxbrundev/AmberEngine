#pragma once

#include "Context/Device.h"
#include "LowRenderer/Camera.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw_gl3.h"
#include "Resources/Texture.h"

namespace  RenderEngine
{
	namespace Managers
	{
		class UIManager
		{
		private:
			Core::Device& m_device;
			float m_clearColor[4];

		public:
			UIManager(Core::Device& p_context);
			~UIManager() = default;

			void PreUpdate();
			void Update(LowRenderer::Camera& p_camera);
			void PostUpdate();
			void Close();

			void DisplayTexture(Resources::Texture p_texture);
		};
	}
}
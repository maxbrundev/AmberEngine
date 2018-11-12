#pragma once

#include "Managers/WindowManager.h"
#include "LowRenderer/Camera.h"
#include "UIManager.h"

namespace RenderEngine
{
	namespace Managers
	{
		class RenderingManager
		{
		private:
			std::unique_ptr<WindowManager> m_windowManager;
			std::unique_ptr<LowRenderer::Camera> m_camera;
			std::unique_ptr<UIManager> m_uiManager;

			GLdouble m_deltaTime = 0.0f;
			GLdouble m_lastTime = 0.0f;

			bool isRunning;

		public:
			RenderingManager();
			~RenderingManager();

			void PreUpdate();
			void Update();
			void PostUpdate();

			bool IsRunning();
			void UpdateRenderMode();

			glm::mat4 CalculateProjectionMatrix() const;
			glm::mat4 CalculateViewMatrix() const;

			const std::unique_ptr<LowRenderer::Camera>& GetCamera() const;
			const std::unique_ptr<WindowManager>& GetWindowManager() const;
		};
	}
}
#pragma once

#include "Managers/WindowManager.h"
#include "LowRenderer/Camera.h"
#include "UIManager.h"
#include "ResourcesManager.h"
#include "InputManager.h"

namespace RenderEngine
{
	namespace Managers
	{
		class RenderingManager
		{
		private:
			std::unique_ptr<WindowManager> m_windowManager;
			std::unique_ptr<LowRenderer::Camera> m_camera;
			std::unique_ptr<ResourcesManager> m_resourcesManager;
			std::unique_ptr<UIManager> m_uiManager;
			std::unique_ptr<InputManager> m_inputManager;

			GLdouble m_deltaTime = 0.0f;
			GLdouble m_lastTime = 0.0f;

			bool isRunning;
			bool isWireFrame;
			bool isCameraFree;

		public:
			RenderingManager();
			~RenderingManager() = default;

			void SetWindow(uint16_t p_width, uint16_t p_height);
			void SetCamera(const glm::vec3& p_position = glm::vec3(0.0f, 0.0f, 3.0f));

			void Clear();
			void Update();
			void SwapBuffers();

			bool IsRunning();

			void UpdateRenderMode();
			void UpdateDeltaTime();

			void PolygonModeLine();
			void PolygonModeFill();

			glm::mat4 CalculateProjectionMatrix() const;
			glm::mat4 CalculateViewMatrix() const;
			glm::mat4 GetModelMatrix() const;

			const std::unique_ptr<LowRenderer::Camera>& GetCamera() const;
			const std::unique_ptr<WindowManager>& GetWindowManager() const;
			const std::unique_ptr<ResourcesManager>& GetResourcesManager() const;
		};
	}
}

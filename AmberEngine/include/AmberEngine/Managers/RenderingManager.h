#pragma once

#include "AmberEngine/Managers/WindowManager.h"
#include "AmberEngine/LowRenderer/Camera.h"
#include "AmberEngine/Managers/UIManager.h"
#include "AmberEngine/Managers/ResourcesManager.h"
#include "AmberEngine/Managers/InputManager.h"

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Managers
{
	class API_AMBERENGINE RenderingManager
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
		void UpdateInput();

		void PolygonModeLine();
		void PolygonModeFill();

		void ToggleWireFrame();

		void FreeCamera();
		void LockCamera();
		void ToggleCamera();

		glm::mat4 CalculateProjectionMatrix() const;
		glm::mat4 CalculateViewMatrix() const;
		glm::mat4 GetModelMatrix() const;

		const std::unique_ptr<LowRenderer::Camera>& GetCamera() const;
		const std::unique_ptr<WindowManager>& GetWindowManager() const;
		const std::unique_ptr<ResourcesManager>& GetResourcesManager() const;
		const std::unique_ptr<InputManager>& GetInputManager() const;
	};
}

#pragma once

#include "AmberEngine/Managers/WindowManager.h"
#include "AmberEngine/Managers/ResourcesManager.h"
#include "AmberEngine/Managers/InputManager.h"
#include "AmberEngine/LowRenderer/Camera.h"
#include "AmberEngine/Settings/RenderingSettings.h"
#include "AmberEngine/API/Export.h"

namespace AmberEngine::Managers
{
	class API_AMBERENGINE RenderingManager
	{
	private:
		WindowManager m_windowManager;
		ResourcesManager m_resourcesManager;
		InputManager m_inputManager;

		LowRenderer::Camera m_camera;
		
		GLdouble m_deltaTime = 0.0f;
		GLdouble m_lastTime = 0.0f;

		bool isRunning;
		bool isWireFrame;
		bool isCameraFree;

	public:
		RenderingManager(const Settings::RenderingSettings& p_settings);
		~RenderingManager() = default;

		void SetWindow(uint16_t p_width, uint16_t p_height);
		void SetCamera(const glm::vec3& p_position = glm::vec3(0.0f, 0.0f, 3.0f));

		void Clear(float p_red, float p_green, float p_blue, float p_alpha = 1.0f);
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
		glm::mat4 GetUnitModelMatrix() const;

		WindowManager& GetWindowManager();
		ResourcesManager& GetResourcesManager();
		InputManager& GetInputManager();
		LowRenderer::Camera& GetCamera();
	};
}

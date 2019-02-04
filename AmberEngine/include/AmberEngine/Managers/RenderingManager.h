#pragma once

#include "AmberEngine/Managers/WindowManager.h"
#include "AmberEngine/LowRenderer/CameraController.h"
#include "AmberEngine/Managers/ResourcesManager.h"

#include "AmberEngine/Settings/RenderingSettings.h"

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Managers
{
	class API_AMBERENGINE RenderingManager
	{
	private:
		WindowManager m_windowManager;
		LowRenderer::CameraController m_cameraController;
		ResourcesManager m_resourcesManager;
		
		GLdouble m_deltaTime = 0.0f;
		GLdouble m_lastTime = 0.0f;

		bool isRunning;
		bool isWireFrame;
		bool isCameraFree;

	public:
		RenderingManager(const Settings::RenderingSettings& p_settings);
		~RenderingManager() = default;

		void SetWindow(uint16_t p_width, uint16_t p_height);

		void SetCameraPosition(const glm::vec3& p_position);

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

		glm::mat4 CalculateProjectionMatrix();
		glm::mat4 CalculateViewMatrix();
		glm::mat4 GetUnitModelMatrix();

		WindowManager& GetWindowManager();
		LowRenderer::CameraController& GetCameraController();
		ResourcesManager& GetResourcesManager();
	};
}

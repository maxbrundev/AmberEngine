#pragma once

#include "AmberEngine/Managers/WindowManager.h"
#include "AmberEngine/LowRenderer/CameraController.h"
#include "AmberEngine/Managers/ResourcesManager.h"

#include "AmberEngine/Settings/RenderingSettings.h"

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Managers
{
	/**
	* Temporary class
	*/
	
	class API_AMBERENGINE RenderingManager
	{
	private:
		WindowManager m_windowManager;
		LowRenderer::CameraController m_cameraController;
		ResourcesManager m_resourcesManager;
		
		float m_deltaTime = 0.0f;
		float m_lastTime = 0.0f;

		bool isRunning;
		bool isWireFrame;
		bool isCameraFree;

	public:
		RenderingManager(const Settings::RenderingSettings& p_settings);
		~RenderingManager() = default;

		void SetCameraPosition(const glm::vec3& p_position);

		void SetClearColor(float p_red, float p_green, float p_blue, float p_alpha = 1.0f);
		void Clear(bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer);
		void Clear(LowRenderer::Camera& p_camera, bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer);
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

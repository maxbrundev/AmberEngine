#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Context/Driver.h"
#include "AmberEngine/Context/Device.h"
#include "AmberEngine/Context/Window.h"

#include "AmberEngine/Inputs/InputManager.h"

#include "AmberEngine/Managers/ResourcesManager.h"

#include "AmberEngine/LowRenderer/CameraController.h"

namespace AmberEngine::Managers
{
	/**
	* Temporary / WIP class
	*/
	
	class API_AMBERENGINE RenderingManager
	{
	public:
		std::unique_ptr<Context::Device> m_device;
		std::unique_ptr<Context::Window> m_window;
		std::unique_ptr<Context::Driver> m_driver;
		
		std::unique_ptr<Inputs::InputManager> m_inputManager;
		
		std::shared_ptr<LowRenderer::CameraController> m_cameraController;
		
		ResourcesManager m_resourcesManager;
		
	private:
		float m_deltaTime = 0.0f;
		float m_lastTime  = 0.0f;

		bool isRunning;
		bool isWireFrame;
		bool isCameraFree;

	public:
		RenderingManager(const Settings::DeviceSettings& p_deviceSettings, const Settings::WindowSettings& p_windowSettings, const Settings::DriverSettings& p_driverSettings);
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

		Context::Window& GetWindow();
		LowRenderer::CameraController& GetCameraController();
		ResourcesManager& GetResourcesManager();
	};
}

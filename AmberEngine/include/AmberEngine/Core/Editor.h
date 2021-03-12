#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Core/Context.h"

#include "AmberEngine/LowRenderer/CameraController.h"

namespace AmberEngine::Core
{
	class API_AMBERENGINE Editor
	{
	public:
		Editor(Context& p_context);
		~Editor();

		void PreUpdate();
		void Update(float p_deltaTime);
		void PostUpdate();
		
		
		void UpdateInput();

		void FreeCamera();
		void LockCamera();
		void ToggleCamera();
		void SetCameraPosition(const glm::vec3& p_position);

		LowRenderer::CameraController& GetCameraController();
		
	private:
		AmberEngine::Core::Context&	m_context;

		LowRenderer::CameraController m_cameraController;
		
		bool isCameraFree;
	};
}

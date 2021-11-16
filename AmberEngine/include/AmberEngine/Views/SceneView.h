#pragma once

#include "AmberEngine/API/Export.h"

#include "AView.h"

#include "AmberEngine/Buffers/FrameBuffer.h"

#include "AmberEngine/LowRenderer/CameraController.h"

namespace AmberEngine::Views
{
	class API_AMBERENGINE SceneView : public AView
	{
	public:
		SceneView(Context::Window& p_window, Inputs::InputManager& p_inputManager);
		virtual ~SceneView() override;

		void Update(float p_deltaTime);
		void Render();

		void PrepareCamera();

		void BindFBO();
		void UnbindFBO();

		LowRenderer::CameraController& GetCameraController();
		
	public:
		AmberEngine::Context::Window& m_window;
		LowRenderer::CameraController m_cameraController;
		AmberEngine::Buffers::FrameBuffer m_frameBuffer;
		//glm::vec2 viewportSize;
		bool isFirstFrame = true;
	};
}

#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/UI/Views/AView.h"

#include "AmberEngine/Buffers/FrameBuffer.h"

#include "AmberEngine/LowRenderer/CameraController.h"

// TODO: Render current scene from the scene manager.
namespace AmberEngine::UI
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
		
	private:
		void ResizeFrameBuffer(uint16_t p_width, uint16_t p_height);

	public:
		AmberEngine::Context::Window& m_window;
		LowRenderer::CameraController m_cameraController;
		AmberEngine::Buffers::FrameBuffer m_frameBuffer;
		//glm::vec2 viewportSize;
		bool isFirstFrame = true;
	};
}

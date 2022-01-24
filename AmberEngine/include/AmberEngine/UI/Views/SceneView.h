#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/UI/Views/AView.h"

#include "AmberEngine/Buffers/FrameBuffer.h"

#include "AmberEngine/LowRenderer/CameraController.h"
#include "AmberEngine/Core/Context.h"

// TODO: Render current scene from the scene manager.
namespace AmberEngine::UI
{
	class API_AMBERENGINE SceneView : public AView
	{
	public:
		SceneView(Core::Context& p_context);
		virtual ~SceneView() override;

		void Update(float p_deltaTime);
		void Render();
		void Draw();
		void PrepareCamera();
		void FillEngineUBO();

		LowRenderer::CameraController& GetCameraController();
		
	private:
		void ResizeFrameBuffer(uint16_t p_width, uint16_t p_height);

	private:
		Core::Context& m_context;

		LowRenderer::CameraController m_cameraController;

		AmberEngine::Buffers::FrameBuffer m_frameBuffer;

		glm::vec2 viewportSize;

		bool isFirstFrame = true;
		bool m_isFocused  = false;
		bool m_isHovered  = false;
	};
}

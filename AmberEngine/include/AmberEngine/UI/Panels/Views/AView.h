#pragma once

#include "AmberEngine/UI/Panels/APanel.h"

#include "AmberEngine/LowRenderer/Camera.h"

#include "AmberEngine/Buffers/FrameBuffer.h"

namespace AmberEngine::UI
{
	class AView : public APanel
	{
	public:
		AView(const std::string& p_title, bool p_opened);
		~AView() override;

		void UpdateSize();
		void PrepareCamera();
		void FillEngineUBO();

		void ResizeFrameBuffer(uint16_t p_width, uint16_t p_height);

		void Render();
		void Draw();

	protected:
		virtual void Update(float p_deltaTime) override;
		virtual void DrawImplementation() override;

		virtual void RenderImplementation() = 0;

	protected:
		glm::vec3 m_cameraPosition;
		LowRenderer::Camera m_camera;
	
		Buffers::FrameBuffer m_frameBuffer;
	};
}

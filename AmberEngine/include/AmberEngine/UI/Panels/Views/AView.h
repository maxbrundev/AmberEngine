#pragma once


#include "AmberEngine/UI/Panels/APanelWindow.h"

#include "AmberEngine/Buffers/FrameBuffer.h"
#include "AmberEngine/Core/EditorRenderer.h"

#include "AmberEngine/LowRenderer/Camera.h"
#include "AmberEngine/UI/Widgets/Image.h"

namespace AmberEngine::UI
{
	class AView : public Panels::APanelWindow
	{
	public:
		AView(const std::string& p_title, bool p_opened, Panels::PanelSettings p_panelSettings);
		~AView() override;

		virtual void Update(float p_deltaTime);

		void PrepareCamera();
		void FillEngineUBO();
		void ResizeFrameBuffer(uint16_t p_width, uint16_t p_height);
		void Render();

	protected:
		virtual void DrawImplementation() override;
		virtual void RenderImplementation() = 0;

	protected:
		glm::vec3 m_cameraPosition;
		LowRenderer::Camera m_camera;
		Widgets::Image* m_image;
		Buffers::FrameBuffer m_frameBuffer;
		AmberEngine::Core::EditorRenderer& m_editorRenderer;
	};
}

#pragma once


#include "AmberUI/Panels/APanelWindow.h"

#include "AmberRendering/Buffers/FrameBuffer.h"
#include "AmberEditor/Core/EditorRenderer.h"

#include "AmberRendering/Entities/Camera.h"
#include "AmberUI/Widgets/Image.h"

namespace AmberEditor::Panels
{
	class AView : public AmberUI::Panels::APanelWindow
	{
	public:
		AView(const std::string& p_title, bool p_opened, AmberUI::Panels::PanelSettings p_panelSettings);
		virtual ~AView() override;

		virtual void Update(float p_deltaTime);

		void PrepareCamera();
		void FillEngineUBO();
		void ResizeFrameBuffer(uint16_t p_width, uint16_t p_height);
		void Render();

		void SetCameraPosition(const glm::vec3& p_position);
		void SetCameraRotation(const glm::quat& p_rotation);
		void SetGridColor(const glm::vec3& p_color);
		const glm::vec3& GetCameraPosition() const;
		const glm::quat& GetCameraRotation() const;
		AmberRendering::Entities::Camera& GetCamera();

	protected:
		virtual void DrawImplementation() override;
		virtual void RenderImplementation() = 0;

	protected:
		glm::vec3 m_cameraPosition;
		glm::quat m_cameraRotation;
		AmberRendering::Entities::Camera m_camera;
		AmberUI::Widgets::Image* m_image;
		AmberRendering::Buffers::FrameBuffer m_frameBuffer;
		AmberEditor::Core::EditorRenderer& m_editorRenderer;

		glm::vec3 m_gridColor = glm::vec3{ 0.176f, 0.176f, 0.176f };
		float m_div = 10.0f;
		float m_bias = 0.5f;
		float m_lwidht = 1.0f;
		float m_mwidth = 2.0f;
	};
}

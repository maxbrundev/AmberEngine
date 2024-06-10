#pragma once


#include "AmberEngine/UI/Panels/APanelWindow.h"

#include "AmberEngine/Buffers/FrameBuffer.h"
#include "AmberEngine/Core/EditorRenderer.h"

#include "AmberEngine/Rendering/Entities/Camera.h"
#include "AmberEngine/UI/Widgets/Image.h"

namespace AmberEngine::UI::Panels
{
	class AView : public APanelWindow
	{
	public:
		AView(const std::string& p_title, bool p_opened, PanelSettings p_panelSettings);
		virtual ~AView() override;

		virtual void Update(float p_deltaTime);

		void PrepareCamera();
		void FillEngineUBO();
		void ResizeFrameBuffer(uint16_t p_width, uint16_t p_height);
		void Render();

		void SetCameraPosition(const glm::vec3& p_position);
		void SetCameraRotation(const glm::quat& p_rotation);
		const glm::vec3& GetCameraPosition() const;
		const glm::quat& GetCameraRotation() const;
		Rendering::Entities::Camera& GetCamera();

	protected:
		virtual void DrawImplementation() override;
		virtual void RenderImplementation() = 0;

	protected:
		glm::vec3 m_cameraPosition;
		glm::quat m_cameraRotation;
		Rendering::Entities::Camera m_camera;
		Widgets::Image* m_image;
		Buffers::FrameBuffer m_frameBuffer;
		AmberEngine::Core::EditorRenderer& m_editorRenderer;
	};
}

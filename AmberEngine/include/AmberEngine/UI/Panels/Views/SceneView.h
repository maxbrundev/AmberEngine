#pragma once

#include "AmberEngine/UI/Panels/Views/AView.h"

#include "AmberEngine/LowRenderer/CameraController.h"

namespace AmberEngine::UI
{
	class SceneView : public AView
	{
	public:
		SceneView(const std::string& p_title, bool p_opened, PanelSettings p_panelSettings);
		~SceneView() override = default;

		void Update(float p_deltaTime) override;

		LowRenderer::CameraController& GetCameraController();

	protected:
		void RenderImplementation() override;
		void DrawContent() override;

	private:
		LowRenderer::CameraController m_cameraController;
	};
}

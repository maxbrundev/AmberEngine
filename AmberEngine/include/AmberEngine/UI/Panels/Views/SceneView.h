#pragma once

#include "AmberEngine/Core/SceneSystem/SceneManager.h"
#include "AmberEngine/UI/Panels/Views/AView.h"

#include "AmberEngine/LowRenderer/CameraController.h"

namespace AmberEngine::UI::Panels
{
	class SceneView : public AView
	{
	public:
		SceneView(const std::string& p_title, bool p_opened, PanelSettings p_panelSettings);

		void Update(float p_deltaTime) override;
		void RenderScene();

		LowRenderer::CameraController& GetCameraController();

	protected:
		void RenderImplementation() override;
		
	private:
		AmberEngine::Core::SceneSystem::SceneManager& m_sceneManager;
		LowRenderer::CameraController m_cameraController;
	};
}

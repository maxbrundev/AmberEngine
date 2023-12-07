#pragma once

#include "AmberEngine/Core/SceneSystem/SceneManager.h"
#include "AmberEngine/UI/Panels/Views/AView.h"

#include "AmberEngine/LowRenderer/CameraController.h"

namespace AmberEngine::UI
{
	class SceneView : public AView
	{
	public:
		SceneView(const std::string& p_title, bool p_opened, Panels::PanelSettings p_panelSettings);
		~SceneView() override = default;

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

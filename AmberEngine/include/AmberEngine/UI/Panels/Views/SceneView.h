#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/UI/Panels/Views/AView.h"

#include "AmberEngine/LowRenderer/CameraController.h"

namespace AmberEngine
{
	namespace Core
	{
		class Editor;
	}
}

namespace AmberEngine::UI
{
	class API_AMBERENGINE SceneView : public AView
	{
	public:
		SceneView(const std::string& p_title, bool p_opened);
		~SceneView() override = default;

		void Update(float p_deltaTime) override;

		LowRenderer::CameraController& GetCameraController();

	protected:
		void RenderImplementation() override;
		void DrawContentImplementation() override;

	private:
		LowRenderer::CameraController m_cameraController;
	};
}

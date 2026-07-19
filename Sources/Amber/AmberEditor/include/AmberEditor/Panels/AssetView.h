#pragma once
#include <variant>

#include "AView.h"
#include "AmberEditor/Core/CameraController.h"

namespace AmberEditor::Panels
{
	class AssetView : public AView
	{
	public:
		using ViewableResource = std::variant<AmberRendering::Resources::Model*, AmberRendering::Resources::Texture*, AmberCore::Resources::Material*>;

		AssetView
		(
			const std::string& p_title,
			bool p_opened,
			const AmberUI::Panels::PanelSettings& p_windowSettings
		);

		void Update(float p_deltaTime) override;

		virtual void RenderImplementation() override;

		void SetResource(ViewableResource p_resource);

		ViewableResource GetResource() const;

		AmberEditor::Core::CameraController& GetCameraController();

	private:
		ViewableResource m_resource;
		AmberEditor::Core::CameraController m_cameraController;
	};
}

#pragma once
#include <variant>

#include "AView.h"
#include "AmberEngine/Core/CameraController.h"

namespace AmberEngine::UI::Panels
{
	class AssetView : public AView
	{
	public:
		using ViewableResource = std::variant<Resources::Model*, Resources::Texture*, Resources::Material*>;

		AssetView
		(
			const std::string& p_title,
			bool p_opened,
			const PanelSettings& p_windowSettings
		);

		void Update(float p_deltaTime) override;

		virtual void RenderImplementation() override;

		void SetResource(ViewableResource p_resource);

		ViewableResource GetResource() const;

	private:
		ViewableResource m_resource;
		AmberEngine::Core::CameraController m_cameraController;
	};
}

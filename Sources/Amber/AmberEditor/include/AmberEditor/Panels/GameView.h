#pragma once

#include "AmberEditor/Panels/AView.h"

namespace AmberEditor::Panels
{
	class GameView : public AView
	{
	public:
		GameView(const std::string& p_title, bool p_opened, AmberUI::Panels::PanelSettings p_panelSettings);

		void Update(float p_deltaTime) override;
		bool HasCamera() const;
		const AmberRendering::Data::Frustum* GetActiveFrustum() const;

	protected:
		void RenderImplementation() override;

	private:
		AmberCore::SceneSystem::SceneManager& m_sceneManager;
		bool m_hasCamera = false;
	};
}

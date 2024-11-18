#pragma once

#include "AmberEngine/UI/Panels/Views/AView.h"

namespace AmberEngine::UI::Panels
{
	class GameView : public AView
	{
	public:
		GameView(const std::string& p_title, bool p_opened, PanelSettings p_panelSettings);

		void Update(float p_deltaTime) override;
		bool HasCamera() const;

	protected:
		void RenderImplementation() override;

	private:
		AmberEngine::Core::SceneSystem::SceneManager& m_sceneManager;
		bool m_hasCamera = false;
	};
}

#pragma once

#include "AmberUI/Panels/APanelWindow.h"
#include "AmberUI/Widgets/Group.h"

#include "AmberCore/ECS/Actor.h"
#include "AmberUI/Widgets/ComboBox.h"

namespace AmberEditor::Panels
{
	class Inspector : public AmberUI::Panels::APanelWindow
	{
	public:
		Inspector(const std::string& p_title, bool p_opened, AmberUI::Panels::PanelSettings p_panelSettings);
		virtual ~Inspector() override;

		void FocusActor(AmberCore::ECS::Actor& p_actor);
		void UnFocus();

		void CreateActorInspector(AmberCore::ECS::Actor& p_target);
		AmberCore::ECS::Actor* GetTargetActor() const;

		void Refresh();

	private:
		void DrawComponent(AmberCore::ECS::Components::AComponent& p_component) const;
		void DrawBehaviour(AmberCore::ECS::Components::Behaviour& p_behaviour) const;

	private:
		AmberUI::Widgets::Group* m_actorInfo;
		AmberUI::Widgets::Group* m_inspectorHeader;
		AmberUI::Widgets::ComboBox* m_componentSelectorWidget;
		AmberCore::ECS::Actor* m_targetActor;

		uint64_t m_componentAddedListener = 0;
		uint64_t m_componentRemovedListener = 0;
		uint64_t m_behaviourAddedListener = 0;
		uint64_t m_behaviourRemovedListener = 0;
		uint64_t m_destroyedListener = 0;
	};
}

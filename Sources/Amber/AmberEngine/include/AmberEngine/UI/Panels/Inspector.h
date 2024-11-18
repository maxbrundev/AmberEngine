#pragma once

#include "AmberEngine/UI/Panels/APanelWindow.h"
#include "AmberEngine/UI/Widgets/Group.h"

#include "AmberEngine/Core/ECS/Actor.h"
#include "AmberEngine/UI/Widgets/ComboBox.h"

namespace AmberEngine::UI::Panels
{
	class Inspector : public APanelWindow
	{
	public:
		Inspector(const std::string& p_title, bool p_opened, PanelSettings p_panelSettings);
		virtual ~Inspector() override;

		void FocusActor(AmberEngine::Core::ECS::Actor& p_actor);
		void UnFocus();

		void CreateActorInspector(AmberEngine::Core::ECS::Actor& p_target);
		AmberEngine::Core::ECS::Actor* GetTargetActor() const;

		void Refresh();

	private:
		void DrawComponent(AmberEngine::Core::ECS::Components::AComponent& p_component) const;

	private:
		Widgets::Group* m_actorInfo;
		Widgets::Group* m_inspectorHeader;
		Widgets::ComboBox* m_componentSelectorWidget;
		AmberEngine::Core::ECS::Actor* m_targetActor;

		uint64_t m_componentAddedListener = 0;
		uint64_t m_componentRemovedListener = 0;
		uint64_t m_destroyedListener = 0;
	};
}

#pragma once

#include "AmberEngine/UI/Panels/APanelWindow.h"
#include "AmberEngine/Core/ECS/Actor.h"
#include "AmberEngine/UI/Widgets/Group.h"

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

	private:
		void DrawComponent(AmberEngine::Core::ECS::Components::AComponent& p_component);

	private:
		Widgets::Group* m_actorInfo;
		Widgets::Group* m_inspectorHeader;
		AmberEngine::Core::ECS::Actor* m_targetActor;
	};
}
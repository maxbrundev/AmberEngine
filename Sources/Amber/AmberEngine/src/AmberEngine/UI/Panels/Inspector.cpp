#include "Amberpch.h"

#include "AmberEngine/UI/Panels/Inspector.h"

#include "AmberEngine/Core/API/IInspectorItem.h"

#include "AmberEngine/UI/Widgets/Column.h"
#include "AmberEngine/UI/Widgets/GroupCollapsable.h"
#include "AmberEngine/UI/GUIDrawer.h"
#include "AmberEngine/UI/WidgetContainer.h"

AmberEngine::UI::Panels::Inspector::Inspector(const std::string& p_title, bool p_opened, PanelSettings p_panelSettings) :
	APanelWindow(p_title, p_opened, p_panelSettings), m_targetActor(nullptr)
{
	m_inspectorHeader = &CreateWidget<Widgets::Group>();
	m_inspectorHeader->enabled = false;

	m_actorInfo = &CreateWidget<Widgets::Group>();

	auto& headerColumns = m_inspectorHeader->CreateWidget<Widgets::Columns<2>>();

	auto nameGatherer = [this] { return m_targetActor ? m_targetActor->GetName() : "%undef%"; };
	auto nameProvider = [this](const std::string& p_newName) { if (m_targetActor) m_targetActor->SetName(p_newName); };

	GUIDrawer::DrawString(headerColumns, "Name", nameGatherer, nameProvider);
}

AmberEngine::UI::Panels::Inspector::~Inspector()
{
	UnFocus();
}

void AmberEngine::UI::Panels::Inspector::FocusActor(AmberEngine::Core::ECS::Actor& p_actor)
{
	if (m_targetActor)
		UnFocus();

	m_actorInfo->RemoveAllWidgets();

	m_targetActor = &p_actor;

	m_inspectorHeader->enabled = true;

	CreateActorInspector(p_actor);
}

void AmberEngine::UI::Panels::Inspector::UnFocus()
{
	if(m_targetActor)
	{
		m_inspectorHeader->enabled = false;

		m_targetActor = nullptr;
		m_actorInfo->RemoveAllWidgets();
	}
}

void AmberEngine::UI::Panels::Inspector::CreateActorInspector(AmberEngine::Core::ECS::Actor& p_target)
{
	std::map<std::string, AmberEngine::Core::ECS::Components::AComponent*> components;

	for (auto component : p_target.GetComponents())
		if (component->GetName() != "Transform")
			components[component->GetName()] = component.get();

	auto transform = p_target.GetComponent<AmberEngine::Core::ECS::Components::CTransform>();
	if (transform)
		DrawComponent(*transform);

	for (auto&[name, instance] : components)
		DrawComponent(*instance);
}

AmberEngine::Core::ECS::Actor* AmberEngine::UI::Panels::Inspector::GetTargetActor() const
{
	return m_targetActor;
}

void AmberEngine::UI::Panels::Inspector::DrawComponent(AmberEngine::Core::ECS::Components::AComponent& p_component)
{
	if (auto inspectorItem = dynamic_cast<AmberEngine::Core::API::IInspectorItem*>(&p_component); inspectorItem)
	{
		auto& header = m_actorInfo->CreateWidget<Widgets::GroupCollapsable>(p_component.GetName());
		header.closable = !dynamic_cast<AmberEngine::Core::ECS::Components::CTransform*>(&p_component);
		header.CloseEvent += [this, &header, &p_component]
		{
		};
		auto& columns = header.CreateWidget<Widgets::Columns<2>>();
		columns.widths[0] = 200;
		inspectorItem->OnInspector(columns);
	}
}

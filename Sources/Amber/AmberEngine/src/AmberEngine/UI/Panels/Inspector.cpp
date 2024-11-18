#include "Amberpch.h"

#include "AmberEngine/UI/Panels/Inspector.h"

#include "AmberEngine/Core/API/IInspectorItem.h"
#include "AmberEngine/Core/ECS/Components/CDirectionalLight.h"
#include "AmberEngine/Core/ECS/Components/CMaterialRenderer.h"
#include "AmberEngine/Core/ECS/Components/CModelRenderer.h"
#include "AmberEngine/Core/ECS/Components/CPointLight.h"

#include "AmberEngine/UI/GUIDrawer.h"
#include "AmberEngine/UI/WidgetContainer.h"
#include "AmberEngine/UI/Widgets/Button.h"
#include "AmberEngine/UI/Widgets/Column.h"
#include "AmberEngine/UI/Widgets/ComboBox.h"
#include "AmberEngine/UI/Widgets/GroupCollapsable.h"
#include "AmberEngine/UI/Widgets/Separator.h"

#include "AmberEngine/Core/EditorAction.h"
#include "AmberEngine/Core/ECS/Components/CCamera.h"
#include "AmberEngine/Core/ECS/Components/CPhysicalObject.h"
#include "AmberEngine/Core/ECS/Components/CPhysicalBox.h"
#include "AmberEngine/Core/ECS/Components/CPhysicalCapsule.h"
#include "AmberEngine/Core/ECS/Components/CPhysicalSphere.h"
#include "AmberEngine/Core/ECS/Components/CSpotLight.h"

AmberEngine::UI::Panels::Inspector::Inspector(const std::string& p_title, bool p_opened, PanelSettings p_panelSettings) :
	APanelWindow(p_title, p_opened, p_panelSettings), m_targetActor(nullptr)
{
	m_inspectorHeader = &CreateWidget<Widgets::Group>();
	m_inspectorHeader->enabled = false;
	m_actorInfo = &CreateWidget<Widgets::Group>();

	auto& headerColumns = m_inspectorHeader->CreateWidget<Widgets::Columns<2>>();

	auto nameGatherer = [this] { return m_targetActor ? m_targetActor->GetName() : "%undef%"; };
	auto nameProvider = [this](const std::string& p_newName)
	{
		if (m_targetActor && !p_newName.empty())
		{
			m_targetActor->SetName(p_newName);
		}
	};
	GUIDrawer::DrawString(headerColumns, "Name", nameGatherer, nameProvider);

	/* Tag field */
	auto tagGatherer = [this] { return m_targetActor ? m_targetActor->GetTag() : "%undef%"; };
	auto tagProvider = [this](const std::string & p_newName) { if (m_targetActor) m_targetActor->SetTag(p_newName); };
	GUIDrawer::DrawString(headerColumns, "Tag", tagGatherer, tagProvider);


	auto activeGatherer = [this] { return m_targetActor ? m_targetActor->IsSelfActive() : false; };
	auto activeProvider = [this](bool p_active) { if (m_targetActor) m_targetActor->SetActive(p_active); };
	GUIDrawer::DrawBoolean(headerColumns, "Active", activeGatherer, activeProvider);

	/* Component select + button */
	{
		auto& componentSelectorWidget = m_inspectorHeader->CreateWidget<Widgets::ComboBox>(0);
		componentSelectorWidget.lineBreak = false;
		componentSelectorWidget.choices.emplace(0, "Model Renderer");
		componentSelectorWidget.choices.emplace(1, "Camera");
		componentSelectorWidget.choices.emplace(2, "Physical Box");
		componentSelectorWidget.choices.emplace(3, "Physical Sphere");
		componentSelectorWidget.choices.emplace(4, "Physical Capsule");
		componentSelectorWidget.choices.emplace(5, "Point Light");
		componentSelectorWidget.choices.emplace(6, "Directional Light");
		componentSelectorWidget.choices.emplace(7, "Spot Light");
		componentSelectorWidget.choices.emplace(8, "Ambient Box Light");
		componentSelectorWidget.choices.emplace(9, "Ambient Sphere Light");
		componentSelectorWidget.choices.emplace(10, "Material Renderer");
		//componentSelectorWidget.choices.emplace(11, "Audio Source");
		//componentSelectorWidget.choices.emplace(12, "Audio Listener");

		auto& addComponentButton = m_inspectorHeader->CreateWidget<Widgets::Button>("Add Component", glm::vec2{ 100.f, 0 });
		addComponentButton.idleBackgroundColor = Data::Color{ 0.7f, 0.5f, 0.f };
		addComponentButton.textColor = Data::Color::White;
		addComponentButton.ClickedEvent += [&componentSelectorWidget, this]
		{
			switch (componentSelectorWidget.currentChoice)
			{
			case 0: GetTargetActor()->AddComponent<AmberEngine::Core::ECS::Components::CModelRenderer>(); GetTargetActor()->AddComponent<AmberEngine::Core::ECS::Components::CMaterialRenderer>(); break;
			case 1: GetTargetActor()->AddComponent<AmberEngine::Core::ECS::Components::CCamera>();				break;
			case 2: GetTargetActor()->AddComponent<AmberEngine::Core::ECS::Components::CPhysicalBox>();			break;
			case 3: GetTargetActor()->AddComponent<AmberEngine::Core::ECS::Components::CPhysicalSphere>();		break;
			case 4: GetTargetActor()->AddComponent<AmberEngine::Core::ECS::Components::CPhysicalCapsule>();		break;
			case 5: GetTargetActor()->AddComponent<AmberEngine::Core::ECS::Components::CPointLight>();			break;
			case 6: GetTargetActor()->AddComponent<AmberEngine::Core::ECS::Components::CDirectionalLight>();	break;
			case 7: GetTargetActor()->AddComponent<AmberEngine::Core::ECS::Components::CSpotLight>();			break;
			case 8: GetTargetActor()->AddComponent<AmberEngine::Core::ECS::Components::CAmbientBoxLight>();		break;
			case 9: GetTargetActor()->AddComponent<AmberEngine::Core::ECS::Components::CAmbientSphereLight>();	break;
			case 10: GetTargetActor()->AddComponent<AmberEngine::Core::ECS::Components::CMaterialRenderer>();	break;
			//case 11: GetTargetActor()->AddComponent<CAudioSource>();		break;
			//case 12: GetTargetActor()->AddComponent<CAudioListener>();		break;
			}

			componentSelectorWidget.ValueChangedEvent.Invoke(componentSelectorWidget.currentChoice);
		};

		componentSelectorWidget.ValueChangedEvent += [this, &addComponentButton](int p_value)
		{
			auto defineButtonsStates = [&addComponentButton](bool p_componentExists)
			{
				addComponentButton.disabled = p_componentExists;
				addComponentButton.idleBackgroundColor = !p_componentExists ? Data::Color{ 0.7f, 0.5f, 0.f } : Data::Color{ 0.1f, 0.1f, 0.1f };
			};

			switch (p_value)
			{
			case 0: defineButtonsStates(GetTargetActor()->GetComponent<AmberEngine::Core::ECS::Components::CModelRenderer>());		return;
			case 1: defineButtonsStates(GetTargetActor()->GetComponent<AmberEngine::Core::ECS::Components::CCamera>());				return;
			case 2: defineButtonsStates(GetTargetActor()->GetComponent<AmberEngine::Core::ECS::Components::CPhysicalObject>());		return;
			case 3: defineButtonsStates(GetTargetActor()->GetComponent<AmberEngine::Core::ECS::Components::CPhysicalObject>());		return;
			case 4: defineButtonsStates(GetTargetActor()->GetComponent<AmberEngine::Core::ECS::Components::CPhysicalObject>());		return;
			case 5: defineButtonsStates(GetTargetActor()->GetComponent<AmberEngine::Core::ECS::Components::CPointLight>());			return;
			case 6: defineButtonsStates(GetTargetActor()->GetComponent<AmberEngine::Core::ECS::Components::CDirectionalLight>());	return;
			case 7: defineButtonsStates(GetTargetActor()->GetComponent<AmberEngine::Core::ECS::Components::CSpotLight>());			return;
			case 8: defineButtonsStates(GetTargetActor()->GetComponent<AmberEngine::Core::ECS::Components::CAmbientBoxLight>());	return;
			case 9: defineButtonsStates(GetTargetActor()->GetComponent<AmberEngine::Core::ECS::Components::CAmbientSphereLight>());	return;
			case 10: defineButtonsStates(GetTargetActor()->GetComponent<AmberEngine::Core::ECS::Components::CMaterialRenderer>());	return;
			//case 11: defineButtonsStates(GetTargetActor()->GetComponent<CAudioSource>());		return;
			//case 12: defineButtonsStates(GetTargetActor()->GetComponent<CAudioListener>());		return;
			}
		};

		m_componentSelectorWidget = &componentSelectorWidget;
	}

	m_inspectorHeader->CreateWidget<Widgets::Separator>();

	AmberEngine::Core::ECS::Actor::DestroyEvent += [this](AmberEngine::Core::ECS::Actor& p_destroyed)
	{
		if (&p_destroyed == m_targetActor)
			UnFocus();
	};
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

	m_componentAddedListener = m_targetActor->ComponentAddedEvent += [this](auto& useless) { EDITOR_EXEC(DelayAction([this] { Refresh(); })); };
	m_componentRemovedListener = m_targetActor->ComponentRemovedEvent += [this](auto& useless) { EDITOR_EXEC(DelayAction([this] { Refresh(); })); };

	m_inspectorHeader->enabled = true;

	CreateActorInspector(p_actor);

	m_componentSelectorWidget->ValueChangedEvent.Invoke(m_componentSelectorWidget->currentChoice);

	EDITOR_EVENT(ActorSelectedEvent).Invoke(*m_targetActor);
}

void AmberEngine::UI::Panels::Inspector::UnFocus()
{
	if(m_targetActor)
	{
		m_targetActor->ComponentAddedEvent -= m_componentAddedListener;
		m_targetActor->ComponentRemovedEvent -= m_componentRemovedListener;

		EDITOR_EVENT(ActorUnselectedEvent).Invoke(*m_targetActor);

		m_inspectorHeader->enabled = false;

		m_targetActor = nullptr;
		m_actorInfo->RemoveAllWidgets();
	}
}

void AmberEngine::UI::Panels::Inspector::CreateActorInspector(AmberEngine::Core::ECS::Actor& p_target)
{
	std::map<std::string, AmberEngine::Core::ECS::Components::AComponent*> components;

	for (auto component : p_target.GetComponents())
	{
		if (component->GetName() != "Transform")
			components[component->GetName()] = component.get();
	}

	if (auto transform = p_target.GetComponent<AmberEngine::Core::ECS::Components::CTransform>())
		DrawComponent(*transform);

	for (auto&[name, instance] : components)
		DrawComponent(*instance);
}

AmberEngine::Core::ECS::Actor* AmberEngine::UI::Panels::Inspector::GetTargetActor() const
{
	return m_targetActor;
}

void AmberEngine::UI::Panels::Inspector::Refresh()
{
	if (m_targetActor)
	{
		m_actorInfo->RemoveAllWidgets();
		CreateActorInspector(*m_targetActor);
	}
}

void AmberEngine::UI::Panels::Inspector::DrawComponent(AmberEngine::Core::ECS::Components::AComponent& p_component) const
{
	if (auto inspectorItem = dynamic_cast<AmberEngine::Core::API::IInspectorItem*>(&p_component); inspectorItem != nullptr)
	{
		auto& header = m_actorInfo->CreateWidget<Widgets::GroupCollapsable>(p_component.GetName());
		header.closable = !dynamic_cast<AmberEngine::Core::ECS::Components::CTransform*>(&p_component);
		header.CloseEvent += [this, &header, &p_component]
		{
			if (p_component.Owner.RemoveComponent(p_component))
				m_componentSelectorWidget->ValueChangedEvent.Invoke(m_componentSelectorWidget->currentChoice);
		};
		auto& columns = header.CreateWidget<Widgets::Columns<2>>();
		columns.widths[0] = 200;
		inspectorItem->OnInspector(columns);
	}
}

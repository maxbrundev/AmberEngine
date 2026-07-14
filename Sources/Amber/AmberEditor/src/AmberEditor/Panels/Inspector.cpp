#include "Amberpch.h"

#include "AmberEditor/Panels/Inspector.h"

#include "AmberCore/API/IInspectorItem.h"
#include "AmberCore/ECS/Components/CDirectionalLight.h"
#include "AmberCore/ECS/Components/CMaterialRenderer.h"
#include "AmberCore/ECS/Components/CAudioSource.h"
#include "AmberCore/ECS/Components/CAudioListener.h"
#include "AmberCore/ECS/Components/CModelRenderer.h"
#include "AmberCore/ECS/Components/CPointLight.h"

#include "AmberCore/Helpers/GUIDrawer.h"
#include "AmberUI/WidgetContainer.h"
#include "AmberUI/Widgets/Button.h"
#include "AmberUI/Widgets/Column.h"
#include "AmberUI/Widgets/ComboBox.h"
#include "AmberUI/Widgets/GroupCollapsable.h"
#include "AmberUI/Widgets/InputText.h"
#include "AmberUI/Widgets/Separator.h"

#include "AmberEditor/Core/EditorAction.h"
#include "AmberCore/ECS/Components/CCamera.h"
#include "AmberCore/ECS/Components/CPhysicalObject.h"
#include "AmberCore/ECS/Components/CPhysicalBox.h"
#include "AmberCore/ECS/Components/CPhysicalCapsule.h"
#include "AmberCore/ECS/Components/CPhysicalSphere.h"
#include "AmberCore/ECS/Components/CSpotLight.h"

AmberEditor::Panels::Inspector::Inspector(const std::string& p_title, bool p_opened, AmberUI::Panels::PanelSettings p_panelSettings) :
	AmberUI::Panels::APanelWindow(p_title, p_opened, p_panelSettings), m_targetActor(nullptr)
{
	m_inspectorHeader = &CreateWidget<AmberUI::Widgets::Group>();
	m_inspectorHeader->Enabled = false;
	m_actorInfo = &CreateWidget<AmberUI::Widgets::Group>();

	auto& headerColumns = m_inspectorHeader->CreateWidget<AmberUI::Widgets::Columns<2>>();

	auto nameGatherer = [this] { return m_targetActor ? m_targetActor->GetName() : "%undef%"; };
	auto nameProvider = [this](const std::string& p_newName)
	{
		if (m_targetActor && !p_newName.empty())
		{
			m_targetActor->SetName(p_newName);
		}
	};
	AmberCore::Helpers::GUIDrawer::DrawString(headerColumns, "Name", nameGatherer, nameProvider);

	/* Tag field */
	auto tagGatherer = [this] { return m_targetActor ? m_targetActor->GetTag() : "%undef%"; };
	auto tagProvider = [this](const std::string & p_newName) { if (m_targetActor) m_targetActor->SetTag(p_newName); };
	AmberCore::Helpers::GUIDrawer::DrawString(headerColumns, "Tag", tagGatherer, tagProvider);


	auto activeGatherer = [this] { return m_targetActor ? m_targetActor->IsSelfActive() : false; };
	auto activeProvider = [this](bool p_active) { if (m_targetActor) m_targetActor->SetActive(p_active); };
	AmberCore::Helpers::GUIDrawer::DrawBoolean(headerColumns, "Active", activeGatherer, activeProvider);

	/* Component select + button */
	{
		auto& componentSelectorWidget = m_inspectorHeader->CreateWidget<AmberUI::Widgets::ComboBox>(0);
		componentSelectorWidget.LineBreak = false;
		componentSelectorWidget.Choices.emplace(0, "Model Renderer");
		componentSelectorWidget.Choices.emplace(1, "Camera");
		componentSelectorWidget.Choices.emplace(2, "Physical Box");
		componentSelectorWidget.Choices.emplace(3, "Physical Sphere");
		componentSelectorWidget.Choices.emplace(4, "Physical Capsule");
		componentSelectorWidget.Choices.emplace(5, "Point Light");
		componentSelectorWidget.Choices.emplace(6, "Directional Light");
		componentSelectorWidget.Choices.emplace(7, "Spot Light");
		componentSelectorWidget.Choices.emplace(8, "Ambient Box Light");
		componentSelectorWidget.Choices.emplace(9, "Ambient Sphere Light");
		componentSelectorWidget.Choices.emplace(10, "Material Renderer");
		componentSelectorWidget.Choices.emplace(11, "Audio Source");
		componentSelectorWidget.Choices.emplace(12, "Audio Listener");

		auto& addComponentButton = m_inspectorHeader->CreateWidget<AmberUI::Widgets::Button>("Add Component", glm::vec2{ 100.f, 0 });
		addComponentButton.IdleBackgroundColor = AmberRendering::Data::Color{ 0.7f, 0.5f, 0.f };
		addComponentButton.TextColor = AmberRendering::Data::Color::White;
		addComponentButton.ClickedEvent += [&componentSelectorWidget, this]
		{
			switch (componentSelectorWidget.CurrentChoice)
			{
			case 0: GetTargetActor()->AddComponent<AmberCore::ECS::Components::CModelRenderer>(); GetTargetActor()->AddComponent<AmberCore::ECS::Components::CMaterialRenderer>(); break;
			case 1: GetTargetActor()->AddComponent<AmberCore::ECS::Components::CCamera>();				break;
			case 2: GetTargetActor()->AddComponent<AmberCore::ECS::Components::CPhysicalBox>();			break;
			case 3: GetTargetActor()->AddComponent<AmberCore::ECS::Components::CPhysicalSphere>();		break;
			case 4: GetTargetActor()->AddComponent<AmberCore::ECS::Components::CPhysicalCapsule>();		break;
			case 5: GetTargetActor()->AddComponent<AmberCore::ECS::Components::CPointLight>();			break;
			case 6: GetTargetActor()->AddComponent<AmberCore::ECS::Components::CDirectionalLight>();	break;
			case 7: GetTargetActor()->AddComponent<AmberCore::ECS::Components::CSpotLight>();			break;
			case 8: GetTargetActor()->AddComponent<AmberCore::ECS::Components::CAmbientBoxLight>();		break;
			case 9: GetTargetActor()->AddComponent<AmberCore::ECS::Components::CAmbientSphereLight>();	break;
			case 10: GetTargetActor()->AddComponent<AmberCore::ECS::Components::CMaterialRenderer>();	break;
			case 11: GetTargetActor()->AddComponent<AmberCore::ECS::Components::CAudioSource>();		break;
			case 12: GetTargetActor()->AddComponent<AmberCore::ECS::Components::CAudioListener>();		break;
			}

			componentSelectorWidget.ValueChangedEvent.Invoke(componentSelectorWidget.CurrentChoice);
		};

		componentSelectorWidget.ValueChangedEvent += [this, &addComponentButton](int p_value)
		{
			auto defineButtonsStates = [&addComponentButton](bool p_componentExists)
			{
				addComponentButton.Disabled = p_componentExists;
				addComponentButton.IdleBackgroundColor = !p_componentExists ? AmberRendering::Data::Color{ 0.7f, 0.5f, 0.f } : AmberRendering::Data::Color{ 0.1f, 0.1f, 0.1f };
			};

			switch (p_value)
			{
			case 0: defineButtonsStates(GetTargetActor()->GetComponent<AmberCore::ECS::Components::CModelRenderer>());		return;
			case 1: defineButtonsStates(GetTargetActor()->GetComponent<AmberCore::ECS::Components::CCamera>());				return;
			case 2: defineButtonsStates(GetTargetActor()->GetComponent<AmberCore::ECS::Components::CPhysicalObject>());		return;
			case 3: defineButtonsStates(GetTargetActor()->GetComponent<AmberCore::ECS::Components::CPhysicalObject>());		return;
			case 4: defineButtonsStates(GetTargetActor()->GetComponent<AmberCore::ECS::Components::CPhysicalObject>());		return;
			case 5: defineButtonsStates(GetTargetActor()->GetComponent<AmberCore::ECS::Components::CPointLight>());			return;
			case 6: defineButtonsStates(GetTargetActor()->GetComponent<AmberCore::ECS::Components::CDirectionalLight>());	return;
			case 7: defineButtonsStates(GetTargetActor()->GetComponent<AmberCore::ECS::Components::CSpotLight>());			return;
			case 8: defineButtonsStates(GetTargetActor()->GetComponent<AmberCore::ECS::Components::CAmbientBoxLight>());	return;
			case 9: defineButtonsStates(GetTargetActor()->GetComponent<AmberCore::ECS::Components::CAmbientSphereLight>());	return;
			case 10: defineButtonsStates(GetTargetActor()->GetComponent<AmberCore::ECS::Components::CMaterialRenderer>());	return;
			case 11: defineButtonsStates(GetTargetActor()->GetComponent<AmberCore::ECS::Components::CAudioSource>());		return;
			case 12: defineButtonsStates(GetTargetActor()->GetComponent<AmberCore::ECS::Components::CAudioListener>());		return;
			}
		};

		m_componentSelectorWidget = &componentSelectorWidget;
	}

	{
		auto& scriptSelectorWidget = m_inspectorHeader->CreateWidget<AmberUI::Widgets::InputText>("");
		scriptSelectorWidget.LineBreak = false;

		scriptSelectorWidget.AddDragDropTarget("File").DataReceivedEvent += [&scriptSelectorWidget](std::pair<std::string, AmberUI::Widgets::Group*> p_data)
		{
			scriptSelectorWidget.Content = EDITOR_EXEC(GetScriptPath(p_data.first));
		};

		auto& addScriptButton = m_inspectorHeader->CreateWidget<AmberUI::Widgets::Button>("Add Script", glm::vec2 { 100.0f, 0.0f });
		addScriptButton.IdleBackgroundColor = AmberRendering::Data::Color { 0.7f, 0.5f, 0.0f };
		addScriptButton.TextColor = AmberRendering::Data::Color::White;
		addScriptButton.ClickedEvent += [&scriptSelectorWidget, this]
		{
			GetTargetActor()->AddBehaviour(scriptSelectorWidget.Content);
		};
	}

	m_inspectorHeader->CreateWidget<AmberUI::Widgets::Separator>();

	AmberCore::ECS::Actor::DestroyEvent += [this](AmberCore::ECS::Actor& p_destroyed)
	{
		if (&p_destroyed == m_targetActor)
			UnFocus();
	};
}

AmberEditor::Panels::Inspector::~Inspector()
{
	UnFocus();
}

void AmberEditor::Panels::Inspector::FocusActor(AmberCore::ECS::Actor& p_actor)
{
	if (m_targetActor)
		UnFocus();

	m_actorInfo->RemoveAllWidgets();

	m_targetActor = &p_actor;

	m_componentAddedListener = m_targetActor->ComponentAddedEvent += [this](auto& useless) { EDITOR_EXEC(DelayAction([this] { Refresh(); })); };
	m_componentRemovedListener = m_targetActor->ComponentRemovedEvent += [this](auto& useless) { EDITOR_EXEC(DelayAction([this] { Refresh(); })); };
	m_behaviourAddedListener = m_targetActor->BehaviourAddedEvent += [this](auto& useless) { EDITOR_EXEC(DelayAction([this] { Refresh(); })); };
	m_behaviourRemovedListener = m_targetActor->BehaviourRemovedEvent += [this](auto& useless) { EDITOR_EXEC(DelayAction([this] { Refresh(); })); };

	m_inspectorHeader->Enabled = true;

	CreateActorInspector(p_actor);

	m_componentSelectorWidget->ValueChangedEvent.Invoke(m_componentSelectorWidget->CurrentChoice);

	EDITOR_EVENT(ActorSelectedEvent).Invoke(*m_targetActor);
}

void AmberEditor::Panels::Inspector::UnFocus()
{
	if(m_targetActor)
	{
		m_targetActor->ComponentAddedEvent -= m_componentAddedListener;
		m_targetActor->ComponentRemovedEvent -= m_componentRemovedListener;
		m_targetActor->BehaviourAddedEvent -= m_behaviourAddedListener;
		m_targetActor->BehaviourRemovedEvent -= m_behaviourRemovedListener;

		EDITOR_EVENT(ActorUnselectedEvent).Invoke(*m_targetActor);

		m_inspectorHeader->Enabled = false;

		m_targetActor = nullptr;
		m_actorInfo->RemoveAllWidgets();
	}
}

void AmberEditor::Panels::Inspector::CreateActorInspector(AmberCore::ECS::Actor& p_target)
{
	std::map<std::string, AmberCore::ECS::Components::AComponent*> components;

	for (auto component : p_target.GetComponents())
	{
		if (component->GetName() != "Transform")
			components[component->GetName()] = component.get();
	}

	if (auto transform = p_target.GetComponent<AmberCore::ECS::Components::CTransform>())
		DrawComponent(*transform);

	for (auto&[name, instance] : components)
		DrawComponent(*instance);

	for (auto&[name, behaviour] : p_target.GetBehaviours())
		DrawBehaviour(behaviour);
}

AmberCore::ECS::Actor* AmberEditor::Panels::Inspector::GetTargetActor() const
{
	return m_targetActor;
}

void AmberEditor::Panels::Inspector::Refresh()
{
	if (m_targetActor)
	{
		m_actorInfo->RemoveAllWidgets();
		CreateActorInspector(*m_targetActor);
	}
}

void AmberEditor::Panels::Inspector::DrawComponent(AmberCore::ECS::Components::AComponent& p_component) const
{
	if (auto inspectorItem = dynamic_cast<AmberCore::API::IInspectorItem*>(&p_component); inspectorItem != nullptr)
	{
		auto& header = m_actorInfo->CreateWidget<AmberUI::Widgets::GroupCollapsable>(p_component.GetName());
		header.Closable = !dynamic_cast<AmberCore::ECS::Components::CTransform*>(&p_component);
		header.CloseEvent += [this, &header, &p_component]
		{
			if (p_component.Owner.RemoveComponent(p_component))
				m_componentSelectorWidget->ValueChangedEvent.Invoke(m_componentSelectorWidget->CurrentChoice);
		};
		auto& columns = header.CreateWidget<AmberUI::Widgets::Columns<2>>();
		columns.Widths[0] = 200;
		inspectorItem->OnInspector(columns);
	}
}

void AmberEditor::Panels::Inspector::DrawBehaviour(AmberCore::ECS::Components::Behaviour& p_behaviour) const
{
	if (auto inspectorItem = dynamic_cast<AmberCore::API::IInspectorItem*>(&p_behaviour); inspectorItem != nullptr)
	{
		auto& header = m_actorInfo->CreateWidget<AmberUI::Widgets::GroupCollapsable>(p_behaviour.Name);
		header.Closable = true;
		header.CloseEvent += [this, &header, &p_behaviour]
		{
			p_behaviour.Owner.RemoveBehaviour(p_behaviour);
		};
		auto& columns = header.CreateWidget<AmberUI::Widgets::Columns<2>>();
		columns.Widths[0] = 200;
		inspectorItem->OnInspector(columns);
	}
}

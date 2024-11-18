#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Components/CCamera.h"

#include "AmberEngine/Core/Helpers/Serializer.h"

#include "AmberEngine/UI/GUIDrawer.h"
#include "AmberEngine/UI/Widgets/ComboBox.h"

AmberEngine::Core::ECS::Components::CCamera::CCamera(Actor& p_owner) : AComponent(p_owner)
{
	SetClearColor({ 0.1921569f, 0.3019608f, 0.4745098f });
}

std::string AmberEngine::Core::ECS::Components::CCamera::GetName()
{
	return "Camera";
}

void AmberEngine::Core::ECS::Components::CCamera::SetFov(float p_value)
{
	m_camera.SetFov(p_value);
}

void AmberEngine::Core::ECS::Components::CCamera::SetSize(float p_value)
{
	m_camera.SetSize(p_value);
}

void AmberEngine::Core::ECS::Components::CCamera::SetNear(float p_value)
{
	m_camera.SetNear(p_value);
}

void AmberEngine::Core::ECS::Components::CCamera::SetFar(float p_value)
{
	m_camera.SetFar(p_value);
}

void AmberEngine::Core::ECS::Components::CCamera::SetClearColor(const glm::vec3& p_clearColor)
{
	m_camera.SetClearColor(p_clearColor);
}

void AmberEngine::Core::ECS::Components::CCamera::SetProjectionMode(Rendering::Settings::EProjectionMode p_projectionMode)
{
	m_camera.SetProjectionMode(p_projectionMode);
}

float AmberEngine::Core::ECS::Components::CCamera::GetFov() const
{
	return m_camera.GetFov();
}

float AmberEngine::Core::ECS::Components::CCamera::GetSize() const
{
	return m_camera.GetSize();
}

float AmberEngine::Core::ECS::Components::CCamera::GetNear() const
{
	return m_camera.GetNear();
}

float AmberEngine::Core::ECS::Components::CCamera::GetFar() const
{
	return m_camera.GetFar();
}

const glm::vec3& AmberEngine::Core::ECS::Components::CCamera::GetClearColor() const
{
	return m_camera.GetClearColor();
}

AmberEngine::Rendering::Settings::EProjectionMode AmberEngine::Core::ECS::Components::CCamera::GetProjectionMode() const
{
	return m_camera.GetProjectionMode();
}

AmberEngine::Rendering::Entities::Camera& AmberEngine::Core::ECS::Components::CCamera::GetCamera()
{
	return m_camera;
}

void AmberEngine::Core::ECS::Components::CCamera::OnInspector(UI::WidgetContainer& p_root)
{
	auto currentProjectionMode = GetProjectionMode();

	UI::GUIDrawer::DrawScalar<float>(p_root, "Field of view", std::bind(&CCamera::GetFov, this), std::bind(&CCamera::SetFov, this, std::placeholders::_1));
	auto& fovWidget = *p_root.GetWidgets()[p_root.GetWidgets().size() - 1];
	auto& fovWidgetLabel = *p_root.GetWidgets()[p_root.GetWidgets().size() - 2];
	fovWidget.enabled = fovWidgetLabel.enabled = currentProjectionMode == Rendering::Settings::EProjectionMode::PERSPECTIVE;

	UI::GUIDrawer::DrawScalar<float>(p_root, "Size", std::bind(&CCamera::GetSize, this), std::bind(&CCamera::SetSize, this, std::placeholders::_1));
	auto& sizeWidget = *p_root.GetWidgets()[p_root.GetWidgets().size() - 1];
	auto& sizeWidgetLabel = *p_root.GetWidgets()[p_root.GetWidgets().size() - 2];
	sizeWidget.enabled = sizeWidgetLabel.enabled = currentProjectionMode == Rendering::Settings::EProjectionMode::ORTHOGRAPHIC;

	UI::GUIDrawer::DrawScalar<float>(p_root, "Near", std::bind(&CCamera::GetNear, this), std::bind(&CCamera::SetNear, this, std::placeholders::_1));
	UI::GUIDrawer::DrawScalar<float>(p_root, "Far", std::bind(&CCamera::GetFar, this), std::bind(&CCamera::SetFar, this, std::placeholders::_1));
	UI::GUIDrawer::DrawColor(p_root, "Clear color", [this]() {return reinterpret_cast<const Data::Color&>(GetClearColor()); }, [this](Data::Color p_color) { SetClearColor({ p_color.r, p_color.g, p_color.b }); }, false);

	UI::GUIDrawer::CreateTitle(p_root, "Projection Mode");
	auto& projectionMode = p_root.CreateWidget<UI::Widgets::ComboBox>(static_cast<int>(GetProjectionMode()));
	projectionMode.choices.emplace(0, "Perspective");
	projectionMode.choices.emplace(1, "Orthographic");

	projectionMode.ValueChangedEvent += [this, &fovWidget, &fovWidgetLabel, &sizeWidget, &sizeWidgetLabel](int p_choice)
	{
		const auto newProjectionMode = static_cast<Rendering::Settings::EProjectionMode>(p_choice);
		SetProjectionMode(newProjectionMode);
		fovWidget.enabled = fovWidgetLabel.enabled = newProjectionMode == Rendering::Settings::EProjectionMode::PERSPECTIVE;
		sizeWidget.enabled = sizeWidgetLabel.enabled = newProjectionMode == Rendering::Settings::EProjectionMode::ORTHOGRAPHIC;
	};
}

void AmberEngine::Core::ECS::Components::CCamera::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "fov", m_camera.GetFov());
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "size", m_camera.GetSize());
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "near", m_camera.GetNear());
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "far", m_camera.GetFar());
	Helpers::Serializer::SerializeVec3(p_doc, p_node, "clear_color", m_camera.GetClearColor());
	Helpers::Serializer::SerializeInt(p_doc, p_node, "projection_mode", static_cast<int>(m_camera.GetProjectionMode()));
}

void AmberEngine::Core::ECS::Components::CCamera::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	m_camera.SetFov(Helpers::Serializer::DeserializeFloat(p_doc, p_node, "fov"));
	m_camera.SetSize(Helpers::Serializer::DeserializeFloat(p_doc, p_node, "size"));
	m_camera.SetNear(Helpers::Serializer::DeserializeFloat(p_doc, p_node, "near"));
	m_camera.SetFar(Helpers::Serializer::DeserializeFloat(p_doc, p_node, "far"));
	m_camera.SetClearColor(Helpers::Serializer::DeserializeVec3(p_doc, p_node, "clear_color"));

	// We have to make sure the "projection_mode" exists in the serialized component, otherwise we do not want to modify the default setting (Perspective).
	// This is a bad practice to have each components calling setters in `OnDeserialize` even if no XML node hasn't been found for a given property.
	// We should rework this system later. As it is out of the scope of the orthographic projection scope, this will be left as is for now.
	if (p_node->FirstChildElement("projection_mode"))
	{
		m_camera.SetProjectionMode(static_cast<Rendering::Settings::EProjectionMode>(Helpers::Serializer::DeserializeInt(p_doc, p_node, "projection_mode")));
	}
}

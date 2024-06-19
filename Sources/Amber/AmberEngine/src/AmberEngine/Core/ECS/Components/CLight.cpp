#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Components/CLight.h"

#include "AmberEngine/Core/ECS/Actor.h"
#include "AmberEngine/Core/Helpers/Serializer.h"
#include "AmberEngine/UI/GUIDrawer.h"

AmberEngine::Core::ECS::Components::CLight::CLight(Actor& p_owner, Rendering::Settings::ELightType p_type): AComponent(p_owner), m_data(p_owner.transform.GetTransform(), p_type)
{
}

void AmberEngine::Core::ECS::Components::CLight::SetColor(const Data::Color& p_color)
{
	m_data.Color = p_color;
}

void AmberEngine::Core::ECS::Components::CLight::SetIntensity(float p_intensity)
{
	m_data.Intensity = p_intensity;
}

const AmberEngine::Data::Color& AmberEngine::Core::ECS::Components::CLight::GetColor() const
{
	return m_data.Color;
}

float AmberEngine::Core::ECS::Components::CLight::GetIntensity() const
{
	return m_data.Intensity;
}

AmberEngine::Rendering::Entities::Light& AmberEngine::Core::ECS::Components::CLight::GetData()
{
	return m_data;
}

void AmberEngine::Core::ECS::Components::CLight::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Helpers::Serializer::SerializeColor(p_doc, p_node, "color", m_data.Color);
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "intensity", m_data.Intensity);
}

void AmberEngine::Core::ECS::Components::CLight::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Helpers::Serializer::DeserializeColor(p_doc, p_node, "color", m_data.Color);
	Helpers::Serializer::DeserializeFloat(p_doc, p_node, "intensity", m_data.Intensity);
}

void AmberEngine::Core::ECS::Components::CLight::OnInspector(UI::WidgetContainer& p_root)
{
	UI::GUIDrawer::DrawColor(p_root, "Color", m_data.Color);
	UI::GUIDrawer::DrawScalar<float>(p_root, "Intensity", m_data.Intensity, 0.005f, UI::GUIDrawer::MIN_FLOAT, UI::GUIDrawer::MAX_FLOAT);
}
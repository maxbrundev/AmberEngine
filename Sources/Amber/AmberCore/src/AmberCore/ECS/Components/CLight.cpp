#include "Amberpch.h"

#include "AmberCore/ECS/Components/CLight.h"

#include "AmberCore/ECS/Actor.h"
#include "AmberCore/Helpers/Serializer.h"

#include "AmberCore/Helpers/GUIDrawer.h"

AmberCore::ECS::Components::CLight::CLight(Actor& p_owner, AmberRendering::Settings::ELightType p_type) :
AComponent(p_owner),
m_data(p_owner.transform.GetTransform(), p_type)
{
}

void AmberCore::ECS::Components::CLight::SetColor(const AmberRendering::Data::Color& p_color)
{
	m_data.Color = p_color;
}

void AmberCore::ECS::Components::CLight::SetIntensity(float p_intensity)
{
	m_data.Intensity = p_intensity;
}

const AmberRendering::Data::Color& AmberCore::ECS::Components::CLight::GetColor() const
{
	return m_data.Color;
}

float AmberCore::ECS::Components::CLight::GetIntensity() const
{
	return m_data.Intensity;
}

AmberRendering::Entities::Light& AmberCore::ECS::Components::CLight::GetData()
{
	return m_data;
}

void AmberCore::ECS::Components::CLight::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Helpers::Serializer::SerializeColor(p_doc, p_node, "color", m_data.Color);
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "intensity", m_data.Intensity);
}

void AmberCore::ECS::Components::CLight::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Helpers::Serializer::DeserializeColor(p_doc, p_node, "color", m_data.Color);
	Helpers::Serializer::DeserializeFloat(p_doc, p_node, "intensity", m_data.Intensity);
}

void AmberCore::ECS::Components::CLight::OnInspector(AmberUI::WidgetContainer& p_root)
{
	AmberCore::Helpers::GUIDrawer::DrawColor(p_root, "Color", m_data.Color);
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Intensity", m_data.Intensity, 0.005f, AmberCore::Helpers::GUIDrawer::MIN_FLOAT, AmberCore::Helpers::GUIDrawer::MAX_FLOAT);
}
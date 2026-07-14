#include "Amberpch.h"

#include "AmberCore/ECS/Components/CAmbientSphereLight.h"

#include "AmberCore/Helpers/Serializer.h"

#include "AmberCore/Helpers/GUIDrawer.h"

AmberCore::ECS::Components::CAmbientSphereLight::CAmbientSphereLight(Actor& p_owner) :
CLight(p_owner, AmberRendering::Settings::ELightType::AMBIENT_SPHERE)
{
	m_data.Intensity = 0.1f;
	m_data.Constant  = 1.0f;

	m_data.Type = static_cast<float>(AmberRendering::Settings::ELightType::AMBIENT_SPHERE);
}

std::string AmberCore::ECS::Components::CAmbientSphereLight::GetName()
{
	return "Ambient Sphere Light";
}

float AmberCore::ECS::Components::CAmbientSphereLight::GetRadius() const
{
	return m_data.Constant;
}

void AmberCore::ECS::Components::CAmbientSphereLight::SetRadius(float p_radius)
{
	m_data.Constant = p_radius;
}

void AmberCore::ECS::Components::CAmbientSphereLight::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	CLight::OnSerialize(p_doc, p_node);

	Helpers::Serializer::SerializeFloat(p_doc, p_node, "radius", m_data.Constant);
}

void AmberCore::ECS::Components::CAmbientSphereLight::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	CLight::OnDeserialize(p_doc, p_node);

	Helpers::Serializer::DeserializeFloat(p_doc, p_node, "radius", m_data.Constant);
}

void AmberCore::ECS::Components::CAmbientSphereLight::OnInspector(AmberUI::WidgetContainer& p_root)
{
	CLight::OnInspector(p_root);

	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Radius", m_data.Constant, 0.1f, 0.0f);
}

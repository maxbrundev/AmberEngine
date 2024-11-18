#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Components/CAmbientSphereLight.h"

#include "AmberEngine/Core/Helpers/Serializer.h"

#include "AmberEngine/UI/GUIDrawer.h"

AmberEngine::Core::ECS::Components::CAmbientSphereLight::CAmbientSphereLight(Actor& p_owner) :
CLight(p_owner, Rendering::Settings::ELightType::AMBIENT_SPHERE)
{
	m_data.Intensity = 0.1f;
	m_data.Constant  = 1.0f;

	m_data.Type = static_cast<float>(Rendering::Settings::ELightType::AMBIENT_SPHERE);
}

std::string AmberEngine::Core::ECS::Components::CAmbientSphereLight::GetName()
{
	return "Ambient Sphere Light";
}

float AmberEngine::Core::ECS::Components::CAmbientSphereLight::GetRadius() const
{
	return m_data.Constant;
}

void AmberEngine::Core::ECS::Components::CAmbientSphereLight::SetRadius(float p_radius)
{
	m_data.Constant = p_radius;
}

void AmberEngine::Core::ECS::Components::CAmbientSphereLight::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	CLight::OnSerialize(p_doc, p_node);

	Helpers::Serializer::SerializeFloat(p_doc, p_node, "radius", m_data.Constant);
}

void AmberEngine::Core::ECS::Components::CAmbientSphereLight::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	CLight::OnDeserialize(p_doc, p_node);

	Helpers::Serializer::DeserializeFloat(p_doc, p_node, "radius", m_data.Constant);
}

void AmberEngine::Core::ECS::Components::CAmbientSphereLight::OnInspector(UI::WidgetContainer& p_root)
{
	CLight::OnInspector(p_root);

	UI::GUIDrawer::DrawScalar<float>(p_root, "Radius", m_data.Constant, 0.1f, 0.0f);
}

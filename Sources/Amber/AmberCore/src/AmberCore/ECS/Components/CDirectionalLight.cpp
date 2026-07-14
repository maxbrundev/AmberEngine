#include "Amberpch.h"

#include "AmberCore/ECS/Components/CDirectionalLight.h"

AmberCore::ECS::Components::CDirectionalLight::CDirectionalLight(Actor& p_owner) :
CLight(p_owner, AmberRendering::Settings::ELightType::DIRECTIONAL)
{
}

std::string AmberCore::ECS::Components::CDirectionalLight::GetName()
{
	return "Directional Light";
}

void AmberCore::ECS::Components::CDirectionalLight::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	CLight::OnSerialize(p_doc, p_node);
}

void AmberCore::ECS::Components::CDirectionalLight::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	CLight::OnDeserialize(p_doc, p_node);
}

void AmberCore::ECS::Components::CDirectionalLight::OnInspector(AmberUI::WidgetContainer& p_root)
{
	CLight::OnInspector(p_root);
}

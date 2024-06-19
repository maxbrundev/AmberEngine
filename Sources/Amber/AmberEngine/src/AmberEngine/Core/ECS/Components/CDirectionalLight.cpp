#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Components/CDirectionalLight.h"

AmberEngine::Core::ECS::Components::CDirectionalLight::CDirectionalLight(Actor& p_owner) : CLight(p_owner, Rendering::Settings::ELightType::DIRECTIONAL)
{
}

std::string AmberEngine::Core::ECS::Components::CDirectionalLight::GetName()
{
	return "Directional Light";
}

void AmberEngine::Core::ECS::Components::CDirectionalLight::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	CLight::OnSerialize(p_doc, p_node);
}

void AmberEngine::Core::ECS::Components::CDirectionalLight::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	CLight::OnDeserialize(p_doc, p_node);
}

void AmberEngine::Core::ECS::Components::CDirectionalLight::OnInspector(UI::WidgetContainer& p_root)
{
	CLight::OnInspector(p_root);
}

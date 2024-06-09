#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Components/CDirectionalLight.h"

AmberEngine::Core::ECS::Components::CDirectionalLight::CDirectionalLight(Actor& p_owner) : CLight(p_owner, Rendering::Settings::ELightType::DIRECTIONAL)
{
}

std::string AmberEngine::Core::ECS::Components::CDirectionalLight::GetName()
{
	return "Directional Light";
}

void AmberEngine::Core::ECS::Components::CDirectionalLight::OnInspector(UI::WidgetContainer& p_root)
{
	CLight::OnInspector(p_root);
}

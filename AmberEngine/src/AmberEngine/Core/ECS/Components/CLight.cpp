#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Components/CLight.h"

#include "AmberEngine/Core/ECS/Actor.h"
#include "AmberEngine/UI/GUIDrawer.h"

AmberEngine::Core::ECS::Components::CLight::CLight(Actor& p_owner, Rendering::Entities::ELightType p_type): AComponent(p_owner), m_data(p_owner.transform.GetTransform(), p_type)
{
}

void AmberEngine::Core::ECS::Components::CLight::SetColor(const Data::Color& p_color)
{
	m_data.color = p_color;
}

void AmberEngine::Core::ECS::Components::CLight::SetIntensity(float p_intensity)
{
	m_data.intensity = p_intensity;
}

const AmberEngine::Data::Color& AmberEngine::Core::ECS::Components::CLight::GetColor() const
{
	return m_data.color;
}

float AmberEngine::Core::ECS::Components::CLight::GetIntensity() const
{
	return m_data.intensity;
}

AmberEngine::Rendering::Entities::Light& AmberEngine::Core::ECS::Components::CLight::GetData()
{
	return m_data;
}

void AmberEngine::Core::ECS::Components::CLight::OnInspector(UI::WidgetContainer& p_root)
{
	UI::GUIDrawer::DrawColor(p_root, "Color", m_data.color);
	UI::GUIDrawer::DrawScalar<float>(p_root, "Intensity", m_data.intensity, 0.005f, UI::GUIDrawer::MIN_FLOAT, UI::GUIDrawer::MAX_FLOAT);
}

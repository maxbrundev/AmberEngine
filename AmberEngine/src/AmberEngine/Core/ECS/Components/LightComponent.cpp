#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Components/LightComponent.h"

#include "AmberEngine/Core/ECS/Actor.h"

AmberEngine::ECS::Components::LightComponent::LightComponent(Actor& p_owner, Rendering::Entities::ELightType p_type): AComponent(p_owner), m_data(p_owner.GetTransform(), p_type)
{

}

void AmberEngine::ECS::Components::LightComponent::Update(float p_deltaTime)
{

}

void AmberEngine::ECS::Components::LightComponent::SetColor(const glm::vec3& p_color)
{
	m_data.color = p_color;
}

void AmberEngine::ECS::Components::LightComponent::SetIntensity(float p_intensity)
{
	m_data.intensity = p_intensity;
}

void AmberEngine::ECS::Components::LightComponent::SetConstant(float p_constant)
{
	m_data.constant = p_constant;
}

void AmberEngine::ECS::Components::LightComponent::SetLinear(float p_linear)
{
	m_data.linear = p_linear;
}

void AmberEngine::ECS::Components::LightComponent::SetQuadratic(float p_quadratic)
{
	m_data.quadratic = p_quadratic;
}

void AmberEngine::ECS::Components::LightComponent::SetCutoff(float p_cutoff)
{
	m_data.cutoff = p_cutoff;
}

void AmberEngine::ECS::Components::LightComponent::SetOuterCutoff(float p_outerCutoff)
{
	m_data.outerCutoff = p_outerCutoff;
}

AmberEngine::Rendering::Entities::Light& AmberEngine::ECS::Components::LightComponent::GetLightData()
{
	return m_data;
}

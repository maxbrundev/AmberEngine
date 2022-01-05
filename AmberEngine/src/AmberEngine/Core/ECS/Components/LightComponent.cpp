#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Components/LightComponent.h"

#include "AmberEngine/Core/ECS/Actor.h"

AmberEngine::ECS::Components::LightComponent::LightComponent(Actor& p_owner, Rendering::Entities::ELightType p_type): AComponent(p_owner), m_data(p_owner.GetTransform(), p_type)
{

}

AmberEngine::ECS::Components::LightComponent::~LightComponent()
{

}

void AmberEngine::ECS::Components::LightComponent::Update(float p_deltaTime)
{

}

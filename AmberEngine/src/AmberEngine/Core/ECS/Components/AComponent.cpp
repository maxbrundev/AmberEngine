#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Components/AComponent.h"

#include "AmberEngine/Core/ECS/Actor.h"

AmberEngine::ECS::Components::AComponent::AComponent(Actor& p_owner) : owner(p_owner)
{
}

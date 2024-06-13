#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Components/AComponent.h"

#include "AmberEngine/Core/ECS/Actor.h"

AmberEngine::Core::ECS::Components::AComponent::AComponent(AmberEngine::Core::ECS::Actor& p_owner) : owner(p_owner)
{
}

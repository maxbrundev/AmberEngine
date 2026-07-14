#include "Amberpch.h"

#include "AmberCore/ECS/Components/AComponent.h"

#include "AmberCore/ECS/Actor.h"

AmberCore::ECS::Components::AComponent::AComponent(AmberCore::ECS::Actor& p_owner) : Owner(p_owner)
{
}

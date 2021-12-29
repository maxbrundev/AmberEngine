#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Actor.h"

AmberEngine::ECS::Actor::Actor()
{
}

AmberEngine::ECS::Actor::~Actor()
{
}

void AmberEngine::ECS::Actor::Update(float p_deltaTime)
{

}

AmberEngine::Maths::Transform& AmberEngine::ECS::Actor::GetTransform()
{
	return m_transform;
}

#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Actor.h"

AmberEngine::ECS::Actor::Actor()
{
	std::cout << "Actor created" << std::endl;

}

AmberEngine::ECS::Actor::~Actor()
{
	std::cout << "Actor Destrcuted" << std::endl;
	for (auto& component : m_components)
	{
		delete component;
		component = nullptr;
	}
	m_components.clear();
}

void AmberEngine::ECS::Actor::Update(float p_deltaTime)
{
	for (const auto component : m_components)
	{
		component->Update(p_deltaTime);
	}
}

void AmberEngine::ECS::Actor::SetName(std::string p_name)
{
	m_name = std::move(p_name);
}

AmberEngine::Maths::Transform& AmberEngine::ECS::Actor::GetTransform()
{
	return m_transform;
}

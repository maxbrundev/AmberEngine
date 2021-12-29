#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Components/ModelComponent.h"

AmberEngine::ECS::Components::ModelComponent::ModelComponent(const std::string& p_filePath) : m_model(nullptr)
{
}

AmberEngine::ECS::Components::ModelComponent::~ModelComponent()
{
	if(m_model != nullptr)
	{
		m_model = nullptr;
	}
}

void AmberEngine::ECS::Components::ModelComponent::Update(float p_deltaTime)
{
}

AmberEngine::Resources::Model* AmberEngine::ECS::Components::ModelComponent::GetModel() const
{
	return m_model;
}

std::vector<AmberEngine::Resources::Mesh*>& AmberEngine::ECS::Components::ModelComponent::GetMeshes() const
{
	return m_model->GetMeshes();
}

#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Components/ModelComponent.h"

#include "AmberEngine/Core/ECS/Actor.h"

#include "AmberEngine/Managers/ResourcesManager.h"
//#include "AmberEngine/Tools/Utils/String.h"

AmberEngine::ECS::Components::ModelComponent::ModelComponent(Actor& p_owner, std::string_view p_name, const std::string& p_filePath) : AComponent(p_owner),
	m_model(&Managers::ResourcesManager::Instance().LoadModel(p_name, p_filePath)), m_name(p_name)
{
	//std::string name = Utils::String::RemoveExtensionFromFileName(Utils::String::ExtractFileNameFromPath(p_filePath));
}

AmberEngine::ECS::Components::ModelComponent::~ModelComponent()
{
	if(m_model != nullptr)
	{
		m_model = nullptr;
	}

	// Require Model to be shared pointer in order to unload the resource if the reference count is <= 1
	//Managers::ResourcesManager::Instance().RemoveModel(m_name);
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

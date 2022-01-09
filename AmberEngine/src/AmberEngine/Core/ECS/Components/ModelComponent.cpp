#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Components/ModelComponent.h"

#include "AmberEngine/Core/ECS/Actor.h"

#include "AmberEngine/Managers/ResourcesManager.h"
//#include "AmberEngine/Tools/Utils/String.h"


AmberEngine::ECS::Components::ModelComponent::ModelComponent(Actor& p_owner, std::string_view p_name, const std::string& p_filePath) : AComponent(p_owner), m_model(nullptr)
{
	//std::string name = Utils::String::RemoveExtensionFromFileName(Utils::String::ExtractFileNameFromPath(p_filePath));
	
	m_model = &Managers::ResourcesManager::Instance().LoadModel(p_name, p_filePath);

	std::cout << (m_model == nullptr ? "model is null" : "model is not null") << std::endl;
	std::cout << "Model component created" << std::endl;
}

AmberEngine::ECS::Components::ModelComponent::~ModelComponent()
{
	std::cout << "Model component destructed" << std::endl;

	if(m_model != nullptr)
	{
		m_model = nullptr;
	}
}

void AmberEngine::ECS::Components::ModelComponent::Update(float p_deltaTime)
{
	const glm::mat4 modelMatrix = owner.GetTransform().GetWorldMatrix();

	if(m_model->GetShader() != nullptr)
	{
		m_model->GetShader()->Bind();
		//m_model->GetShader()->SetUniformMat4("u_ModelMatrix", modelMatrix);
		//m_model->GetShader()->SetUniformMat4("u_ProjectionMatrix", projection);
		//m_model->GetShader()->SetUniformMat4("u_ViewMatrix", view);
		m_model->GetShader()->Unbind();
	}
}

AmberEngine::Resources::Model* AmberEngine::ECS::Components::ModelComponent::GetModel() const
{
	return m_model;
}

std::vector<AmberEngine::Resources::Mesh*>& AmberEngine::ECS::Components::ModelComponent::GetMeshes() const
{
	return m_model->GetMeshes();
}

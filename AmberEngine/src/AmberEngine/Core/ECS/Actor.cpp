#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Actor.h"
#include "AmberEngine/Core/ECS/Components/ModelComponent.h"
#include "AmberEngine/Resources/Shader.h"
#include "AmberEngine/Core/SceneSystem/Scene.h"

AmberEngine::ECS::Actor::Actor()
{
}

AmberEngine::ECS::Actor::~Actor()
{
	for (auto& component : m_components)
	{
		delete component;
		component = nullptr;
	}

	m_components.clear();
}

void AmberEngine::ECS::Actor::Update(const std::vector<ECS::Components::LightComponent*>& p_lights, float p_deltaTime)
{
	for (const auto component : m_components)
	{
		component->Update(p_deltaTime);
	}

	if(const auto modelComponent = GetComponent<ECS::Components::ModelComponent>(); modelComponent != nullptr)
	{
		const auto shader = modelComponent->GetModel()->GetShader();
		shader->Bind();

		for(auto& light : p_lights)
		{
			auto lightData = light->GetLightData();
			shader->SetUniformVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
			shader->SetUniformVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
			shader->SetUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
			shader->SetUniformVec3("light.direction", light->owner.GetTransform().GetWorldForward());
		}

		shader->Unbind();
	}
}

void AmberEngine::ECS::Actor::SetName(std::string p_name)
{
	m_name = std::move(p_name);
}

std::string AmberEngine::ECS::Actor::GetName()
{
	return m_name;
}

AmberEngine::Maths::Transform& AmberEngine::ECS::Actor::GetTransform()
{
	return m_transform;
}

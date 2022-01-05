#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Core/ECS/Components/AComponent.h"

#include "AmberEngine/Resources/Model.h"

#include <iostream>

namespace AmberEngine
{
	namespace ECS
	{
		class Actor;
	}
}

namespace AmberEngine::ECS::Components
{
	class API_AMBERENGINE ModelComponent : public AComponent
	{
	public:
		ModelComponent(const ModelComponent& other)
			: AComponent(other),
			  m_model(other.m_model)
		{
			std::cout << "Model component copied" << std::endl;
		}

		ModelComponent(ModelComponent&& other)
			: AComponent(std::move(other)),
			  m_model(other.m_model)
		{
			std::cout << "Model component moved" << std::endl;
		}

		ModelComponent& operator=(const ModelComponent& other)
		{
			std::cout << "Model component copied operator" << std::endl;
			if (this == &other)
				return *this;
			
			return *this;
		}

		ModelComponent& operator=(ModelComponent&& other)
		{
			std::cout << "Model component moved operator" << std::endl;
			if (this == &other)
				return *this;
			
			return *this;
		}

		ModelComponent(Actor& p_owner, const std::string& p_filePath);
		virtual ~ModelComponent() override;

		void Update(float p_deltaTime) override;

		[[nodiscard]] Resources::Model* GetModel() const;
		[[nodiscard]] std::vector<Resources::Mesh*>& GetMeshes() const;

	private:
		Resources::Model* m_model;
	};
}

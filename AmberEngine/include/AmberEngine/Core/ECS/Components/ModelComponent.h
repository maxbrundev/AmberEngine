#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Core/ECS/Components/AComponent.h"

#include "AmberEngine/Resources/Model.h"

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
		ModelComponent(Actor& p_owner, std::string_view p_name, const std::string& p_filePath);
		virtual ~ModelComponent() override;

		void Update(float p_deltaTime) override;

		Resources::Model* GetModel() const;
		std::vector<Resources::Mesh*>& GetMeshes() const;

	private:
		Resources::Model* m_model;
		std::string_view m_name;
	};
}

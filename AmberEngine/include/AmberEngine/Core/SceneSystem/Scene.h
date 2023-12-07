#pragma once

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Core::ECS
{
	class Actor;
}

namespace AmberEngine::Core::ECS::Components
{
	class AComponent;
	class CLight;
	class CModelRenderer;

}

namespace AmberEngine::Core::SceneSystem
{
	class API_AMBERENGINE Scene
	{
	public:
		struct FastAccessComponents
		{
			std::vector<ECS::Components::CModelRenderer*> modelRenderers;
			std::vector<ECS::Components::CLight*>         lights;
		};

		Scene(std::string p_name);
		~Scene();

		void AddActor(ECS::Actor* p_actor);
		ECS::Actor& CreateActor();
		ECS::Actor& CreateActor(const std::string& p_name, const std::string& p_tag = "");
		bool DestroyActor(ECS::Actor& p_target);

		void OnComponentAdded(ECS::Components::AComponent& p_compononent);
		void OnComponentRemoved(ECS::Components::AComponent& p_compononent);

		std::vector<ECS::Actor*>& GetActors();

		const FastAccessComponents& GetFastAccessComponents() const;

	private:
		int64_t m_availableID = 1;
		std::string m_name;
		std::vector<ECS::Actor*> m_actors;
		FastAccessComponents m_fastAccessComponents;
	};
}

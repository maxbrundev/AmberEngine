#pragma once
#include "AmberEngine/API/Export.h"

#include "AmberEngine/Core/Renderer.h"
#include "AmberEngine/Core/ECS/Actor.h"
#include "AmberEngine/Core/ECS/Components/LightComponent.h"

namespace AmberEngine::Core::SceneSystem
{
	class API_AMBERENGINE Scene
	{
	public:
		Scene(std::string p_name);
		~Scene();

		void AddActor(ECS::Actor* p_actor);
		void DestroyActor(ECS::Actor*& p_actor);
		
		void DrawAll(Renderer& p_renderer) const;
		void SetDebugNormal(bool p_value);
		void Update(float p_deltaTime) const;

		std::unordered_map<std::string, ECS::Actor*>& GetActors();
		const std::vector<ECS::Components::LightComponent*>& GetLights();

	private:
		std::unordered_map<std::string, ECS::Actor*> m_actors;
		std::vector<ECS::Components::LightComponent*> m_lights;
		std::string m_name;
		bool m_isDebugingNormal = false;
	};
}

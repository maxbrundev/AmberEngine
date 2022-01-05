#pragma once

#include "AmberEngine/Core/Renderer.h"
#include "AmberEngine/Core/ECS/Actor.h"

namespace AmberEngine::Core
{
	class Scene
	{
	public:
		Scene(std::string p_name);
		~Scene();

		void AddActor(ECS::Actor* p_actor, const std::string& p_name);
		void RemoveGameObject(ECS::Actor*& p_actor);
		
		void DrawAll(Core::Renderer& p_renderer);

		void Update(float p_deltaTime);

	private:
		std::unordered_map<std::string_view, ECS::Actor*> m_actors;
		std::string m_name;
	};
}

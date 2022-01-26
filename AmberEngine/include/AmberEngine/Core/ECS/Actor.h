#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Eventing/Event.h"

#include "AmberEngine/Maths/Transform.h"

#include "AmberEngine/Core/ECS/Components/AComponent.h"
#include "AmberEngine/Core/ECS/Components/LightComponent.h"

namespace AmberEngine::ECS
{
	class API_AMBERENGINE Actor
	{
	public:
		static Eventing::Event<Actor&> CreatedEvent;
		static Eventing::Event<Actor&> DestroyEvent;

	public:
		Actor(std::string p_name);
		~Actor();

		void Update(const std::vector<ECS::Components::LightComponent*>& p_lights, float p_deltaTime);

		void RemoveParent();

		template<typename T>
		void AddComponent(T* p_component)
		{
			static_assert(std::is_base_of_v<Components::AComponent, T>, "T should inherit from AComponent");

			m_components.emplace_back(p_component);
		}

		template<typename T, typename ... args>
		void AddComponent(args ... p_args)
		{
			static_assert(std::is_base_of_v<Components::AComponent, T>, "T should inherit from AComponent");

			m_components.emplace_back(new T(*this, p_args ...));
		}

		template<typename T>
		T* GetComponent()
		{
			static_assert(std::is_base_of_v<Components::AComponent, T>, "T should inherit from AComponent");

			for (auto& component : m_components)
			{
				if(typeid(*component) == typeid(T))
				{
					return static_cast<T*>(component);
				}
			}

			return nullptr;
		}

		void SetName(std::string p_name);
		void SetParent(Actor& p_parent);

		std::string GetName();
		Actor* GetParent() const;

		std::vector<Actor*>& GetChildren();

		Maths::Transform& GetTransform();

	private:
		std::string m_name;
		Maths::Transform m_transform;
		Actor* m_parent = nullptr;
		std::vector<Actor*> m_children;
		std::vector<Components::AComponent*> m_components;
	};
}

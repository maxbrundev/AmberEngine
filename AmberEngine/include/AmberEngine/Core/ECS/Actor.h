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
		static Eventing::Event<Actor&>         CreatedEvent;
		static Eventing::Event<Actor&>         DestroyEvent;
		static Eventing::Event<Actor&, Actor&> AttachEvent;
		static Eventing::Event<Actor&>         DettachEvent;

	public:
		Actor(std::string p_name);
		~Actor();

		template<typename T>
		void AddComponent(T* p_component)
		{
			static_assert(std::is_base_of_v<Components::AComponent, T>, "T should inherit from AComponent");

			if (auto component = GetComponent<T>(); !component)
			{
				m_components.emplace_back(std::move(p_component));
			}
		}

		template<typename T, typename ... Args>
		void AddComponent(Args&& ... p_args)
		{
			static_assert(std::is_base_of_v<Components::AComponent, T>, "T should inherit from AComponent");

			if (auto component = GetComponent<T>(); !component)
			{
				m_components.emplace_back(std::move(new T(*this, std::forward<Args>(p_args) ...)));
			}
		}

		template<typename T>
		void RemoveComponent()
		{
			T* result = nullptr;

			for (auto it = m_components.begin(); it != m_components.end(); ++it)
			{
				if(it != m_components.end())
				{
					result = static_cast<T*>(*it);

					if (result)
					{
						delete result;
						result = nullptr;

						m_components.erase(it);
						return;
					}
				}
			}
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

		void Update(const std::vector<Components::LightComponent*>& p_lights, float p_deltaTime) const;

	
		void SetName(std::string p_name);
	
		void SetParent(Actor& p_parent);
		void RemoveParent();
		bool HasParent() const;
		bool HasChildren() const;
		bool IsDescendantOf(const Actor* p_actor) const;

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

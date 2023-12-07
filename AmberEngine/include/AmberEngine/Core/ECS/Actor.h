#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Eventing/Event.h"

#include "AmberEngine/Core/ECS/Components/AComponent.h"
#include "AmberEngine/Core/ECS/Components/CTransform.h"

namespace AmberEngine::Core::ECS
{
	class API_AMBERENGINE Actor
	{
	public:
		static Eventing::Event<Actor&>         CreatedEvent;
		static Eventing::Event<Actor&>         DestroyEvent;
		static Eventing::Event<Actor&, Actor&> AttachEvent;
		static Eventing::Event<Actor&>         DettachEvent;

		Eventing::Event<Components::AComponent&>	ComponentAddedEvent;
		Eventing::Event<Components::AComponent&>	ComponentRemovedEvent;

	public:
		Actor(int64_t p_actorID, const std::string & p_name, const std::string & p_tag);
		~Actor();

		template<typename T, typename ...Args>
		inline T& AddComponent(Args&& ...p_args)
		{
			static_assert(std::is_base_of<Components::AComponent, T>::value, "T should derive from AComponent");

			if (auto found = GetComponent<T>(); !found)
			{
				m_components.insert(m_components.begin(), std::make_shared<T>(*this, p_args...));
				T& instance = *dynamic_cast<T*>(m_components.front().get());

				ComponentAddedEvent.Invoke(instance);

				return instance;
			}
			else
			{
				return *found;
			}
		}

		//template<typename T, typename ... Args>
		//void AddComponent(Args&& ... p_args)
		//{
		//	static_assert(std::is_base_of<Components::AComponent, T>::value, "T should derive from AComponent");
		//
		//	if (auto found = GetComponent<T>(); !found)
		//	{
		//		m_components.insert(m_components.begin(), std::make_shared<T>(*this, p_args...));
		//		T& instance = *dynamic_cast<T*>(m_components.front().get());
		//	}
		//}

		template<typename T>
		void RemoveComponent()
		{
			static_assert(std::is_base_of<Components::AComponent, T>::value, "T should derive from AComponent");

			std::shared_ptr<T> result(nullptr);

			for (auto it = m_components.begin(); it != m_components.end(); ++it)
			{
				result = std::dynamic_pointer_cast<T>(*it);

				if (result)
				{
					ComponentRemovedEvent.Invoke(*result.get());

					m_components.erase(it);
				}
			}
		}

		template<typename T>
		inline T* GetComponent()
		{
			static_assert(std::is_base_of<Components::AComponent, T>::value, "T should derive from AComponent");

			std::shared_ptr<T> result(nullptr);

			for (auto it = m_components.begin(); it != m_components.end(); ++it)
			{
				result = std::dynamic_pointer_cast<T>(*it);
				if (result)
				{
					return result.get();
				}
			}

			return nullptr;
		}

		bool RemoveComponent(ECS::Components::AComponent& p_component);

		std::vector<std::shared_ptr<Components::AComponent>>& GetComponents();

		void SetName(std::string p_name);
	
		void SetParent(Actor& p_parent);
		void RemoveParent();
		bool HasParent() const;
		bool HasChildren() const;
		bool IsDescendantOf(const Actor* p_actor) const;

		const std::string& GetName() const;
		const std::string& GetTag() const;

		int64_t GetID() const;

		Actor* GetParent() const;

		void SetActive(bool p_active);
		bool IsSelfActive() const;
		bool IsActive() const;

		std::vector<Actor*>& GetChildren();

	private:
		std::string m_name;
		std::string m_tag;

		int64_t	m_actorID;

		bool m_active = true;
		Actor* m_parent = nullptr;
		std::vector<Actor*> m_children;
		std::vector<std::shared_ptr<Components::AComponent>> m_components;

	public:
		Components::CTransform& transform;
	};
}

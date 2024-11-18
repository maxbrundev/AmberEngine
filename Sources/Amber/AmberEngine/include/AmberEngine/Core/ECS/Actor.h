#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Eventing/Event.h"

#include "AmberEngine/Core/ECS/Components/AComponent.h"
#include "AmberEngine/Core/ECS/Components/CTransform.h"
#include "AmberEngine/Core/API/ISerializable.h"


namespace AmberEngine::Core::ECS
{
	class API_AMBERENGINE Actor : public API::ISerializable
	{
	public:
		static Eventing::Event<Actor&>         CreatedEvent;
		static Eventing::Event<Actor&>         DestroyEvent;
		static Eventing::Event<Actor&, Actor&> AttachEvent;
		static Eventing::Event<Actor&, Actor*> DettachEvent;

		Eventing::Event<Components::AComponent&> ComponentAddedEvent;
		Eventing::Event<Components::AComponent&> ComponentRemovedEvent;

	public:
		Actor(int64_t p_actorID, const std::string & p_name, const std::string & p_tag);
		virtual ~Actor() override;

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

		template<typename T>
		inline bool RemoveComponent()
		{
			static_assert(std::is_base_of<Components::AComponent, T>::value, "T should derive from AComponent");
			static_assert(!std::is_same<Components::CTransform, T>::value, "You can't remove a CTransform from an actor");

			std::shared_ptr<T> result(nullptr);

			for (auto it = m_components.begin(); it != m_components.end(); ++it)
			{
				result = std::dynamic_pointer_cast<T>(*it);
				if (result)
				{
					ComponentRemovedEvent.Invoke(*result.get());
					m_components.erase(it);

					return true;
				}
			}

			return false;
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
		void SetTag(const std::string& p_tag);
		void SetID(int64_t p_id);
	
		void SetParent(Actor& p_parent);
		void RemoveParent();
		bool HasParent() const;
		bool HasChildren() const;
		bool IsDescendantOf(const Actor* p_actor) const;

		void MarkAsDestroy();

		bool IsAlive() const;

		const std::string& GetName() const;
		const std::string& GetTag() const;

		int64_t GetID() const;

		Actor* GetParent() const;
		int64_t GetParentID() const;

		void SetActive(bool p_active);
		bool IsSelfActive() const;
		bool IsActive() const;

		void SetSleeping(bool p_sleeping);
		void OnAwake();
		void OnStart();
		void OnEnable();
		void OnDisable();
		void OnDestroy();
		void OnUpdate(float p_deltaTime);
		void OnFixedUpdate(float p_deltaTime);
		void OnLateUpdate(float p_deltaTime);

		std::vector<Actor*>& GetChildren();
		void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

	private:
		std::string m_name;
		std::string m_tag;

		int64_t m_actorID;
		bool m_destroyed = false;
		bool m_active = true;
		Actor* m_parent = nullptr;
		bool	m_sleeping = true;
		bool	m_awaked = false;
		bool	m_started = false;
		int64_t	m_parentID = 0;
		std::vector<Actor*> m_children;
		std::vector<std::shared_ptr<Components::AComponent>> m_components;

	public:
		Components::CTransform& transform;
	};
}

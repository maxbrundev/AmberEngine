#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Maths/Transform.h"
#include "AmberEngine/Core/ECS/Components/AComponent.h"

namespace AmberEngine::ECS
{
	class API_AMBERENGINE Actor
	{
	public:
		Actor();
		~Actor();

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

		void Update(float p_deltaTime);

		void SetName(std::string p_name);

		Maths::Transform& GetTransform();

	private:
		Maths::Transform m_transform;
		std::vector<Components::AComponent*> m_components;
		std::string m_name;
	};
}

#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Core/ECS/Components/IComponent.h"

#include "AmberEngine/Maths/Transform.h"

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
			m_components.emplace_back(p_component);
		}

		template<typename T, typename ... args>
		void AddComponent(args ... p_args)
		{
			m_components.emplace_back(p_args ...);
		}

		void Update(float p_deltaTime);

		Maths::Transform& GetTransform();

	private:
		Maths::Transform m_transform;
		std::vector<Components::IComponent*> m_components;
	};
}

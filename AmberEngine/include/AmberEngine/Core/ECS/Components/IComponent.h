#pragma once

namespace AmberEngine::ECS::Components
{
	class IComponent
	{
	public:
		IComponent() = default;
		virtual ~IComponent() = default;

		virtual void Update(float p_deltaTime) = 0;
	};
}
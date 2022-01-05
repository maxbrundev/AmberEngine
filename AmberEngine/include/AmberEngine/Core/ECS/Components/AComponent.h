#pragma once

namespace AmberEngine
{
	namespace ECS
	{
		class Actor;
	}
}

namespace AmberEngine::ECS::Components
{
	class AComponent
	{
	public:
		AComponent(Actor& p_owner);
		virtual ~AComponent() = default;

		virtual void Update(float p_deltaTime) = 0;

	public:
		Actor& owner;
	};
}

#pragma once

#include "AmberEngine/Core/ECS/Actor.h"

namespace AmberEngine::UI
{
	class Hierarchy
	{
	public:
		static uint64_t __TREENODE_ID;

	public:
		Hierarchy();
		~Hierarchy();

	public:
		void AddActorByInstance(ECS::Actor& p_actor);
		void DeleteActorByInstance(ECS::Actor& p_actor);
		void Draw() const;

	private:
		uint64_t m_destroyedListener;
		const std::string& m_name;
		std::string m_rootID;
		std::unordered_map<ECS::Actor*, std::string> m_actors;
	};
}

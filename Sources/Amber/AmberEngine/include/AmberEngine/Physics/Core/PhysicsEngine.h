#pragma once

#include <optional>

#include "AmberEngine/Physics/Entities/PhysicalObject.h"
#include "AmberEngine/Physics/Entities/RaycastHit.h"

#include "AmberEngine/Physics/Settings/PhysicsSettings.h"

namespace AmberEngine::Physics::Core
{
	class PhysicsEngine
	{
	public:
		PhysicsEngine(const Settings::PhysicsSettings& p_settings);
		~PhysicsEngine() = default;

		bool Update(float p_deltaTime);

		std::optional<Entities::RaycastHit> Raycast(glm::vec3 p_origin, glm::vec3 p_direction, float p_distance);

		void SetGravity(const glm::vec3& p_gravity);

		glm::vec3 GetGravity() const;

	private:
		void PreUpdate();
		void PostUpdate();

		void ListenToPhysicalObjects();

		void Consider(Entities::PhysicalObject& p_toConsider);
		void Unconsider(Entities::PhysicalObject& p_toUnconsider);

		void Consider(btRigidBody& p_toConsider);
		void Unconsider(btRigidBody& p_toUnconsider);

		void ResetCollisionEvents();
		void CheckCollisionStopEvents();

		static bool CollisionCallback(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1, const btCollisionObjectWrapper* obj2, int id2, int index2);
		void SetCollisionCallback();

	private:
		std::unique_ptr<btDynamicsWorld> m_world;
		std::unique_ptr<btDispatcher> m_dispatcher;
		std::unique_ptr<btCollisionConfiguration> m_collisionConfig;
		std::unique_ptr<btBroadphaseInterface> m_broadphase;
		std::unique_ptr<btConstraintSolver> m_solver;

		static std::map< std::pair<Entities::PhysicalObject*, Entities::PhysicalObject*>, bool> m_collisionEvents;
		std::vector<std::reference_wrapper<Entities::PhysicalObject>> m_physicalObjects;
	};
}

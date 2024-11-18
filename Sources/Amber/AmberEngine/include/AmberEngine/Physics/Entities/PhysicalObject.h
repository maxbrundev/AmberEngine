#pragma once

#include <any>
#include <memory>

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

#include "AmberEngine/Eventing/Event.h"
#include "AmberEngine/Maths/Transform.h"
#include "AmberEngine/Physics/Settings/BodySettings.h"

namespace AmberEngine::Physics::Core { class PhysicsEngine; }

namespace AmberEngine::Physics::Entities
{
	class PhysicalObject
	{
	public:
		friend class AmberEngine::Physics::Core::PhysicsEngine;

		enum class ECollisionDetectionMode : uint8_t
		{
			DISCRETE,
			CONTINUOUS
		};

		enum class EActivationState : uint8_t
		{
			ACTIVE            = 1,
			SLEEPING          = 2,
			LOOKING_FOR_SLEEP = 3,
			ALWAYS_ACTIVE     = 4,
			ALWAYS_SLEEPING   = 5
		};

		PhysicalObject();
		PhysicalObject(Maths::Transform& p_transform);

		virtual ~PhysicalObject();

		void AddForce(const glm::vec3& p_force);

		void AddImpulse(const glm::vec3& p_impulse);

		void ClearForces();

		void AddFlag(btCollisionObject::CollisionFlags p_flag);

		void RemoveFlag(btCollisionObject::CollisionFlags p_flag);

		float GetMass() const;
		const ECollisionDetectionMode& GetCollisionDetectionMode() const;
		float GetBounciness() const;
		float GetFriction()	const;
		glm::vec3 GetLinearVelocity() const;
		glm::vec3 GetAngularVelocity() const;
		glm::vec3 GetLinearFactor()	const;
		glm::vec3 GetAngularFactor() const;

		bool IsTrigger() const;
		bool IsKinematic() const;

		EActivationState GetActivationState() const;
		Maths::Transform& GetTransform();

		void SetMass(float p_mass);
		void SetCollisionDetectionMode(ECollisionDetectionMode p_mode);
		void SetBounciness(float p_bounciness);
		void SetFriction(float p_friction);
		void SetLinearVelocity(const glm::vec3& p_linearVelocity);
		void SetAngularVelocity(const glm::vec3& p_angularVelocity);
		void SetLinearFactor(const glm::vec3& p_linearFactor);
		void SetAngularFactor(const glm::vec3& p_angularFactor);
		void SetTrigger(bool p_trigger);
		void SetKinematic(bool p_kinematic);
		void SetActivationState(EActivationState p_activationState);
		void SetEnabled(bool p_enabled);

		bool IsEnabled() const;

		template <typename T> T GetUserData() const { return std::any_cast<T>(m_userData); }

		template <typename T> void SetUserData(T p_data) { m_userData = p_data; }

	protected:
		void Init();
		void RecreateBody();
		void ApplyInertia();
		virtual void SetLocalScaling(const glm::vec3& p_scaling) = 0;
		void Consider();
		void Unconsider();

	private:
		/* Internal */
		void								CreateBody(const Settings::BodySettings& p_bodySettings);
		Settings::BodySettings				DestroyBody();
		btVector3							CalculateInertia() const;

		/* Needed by the physics engine */
		btRigidBody&			GetBody();
		void					UpdateBtTransform();
		void					UpdateFTransform();

	public:
		Eventing::Event<PhysicalObject&>			CollisionStartEvent;
		Eventing::Event<PhysicalObject&>			CollisionStayEvent;
		Eventing::Event<PhysicalObject&>			CollisionStopEvent;
		
		Eventing::Event<PhysicalObject&>			TriggerStartEvent;
		Eventing::Event<PhysicalObject&>			TriggerStayEvent;
		Eventing::Event<PhysicalObject&>			TriggerStopEvent;

	private:
		/* Transform stuff */
		Maths::Transform* const		m_transform;
		const bool						m_internalTransform;

		/* Settings */
		float					m_mass = 1.0f;
		bool					m_kinematic = false;
		bool					m_trigger = false;
		bool					m_enabled = true;
		bool					m_considered = false;
		ECollisionDetectionMode m_collisionMode = ECollisionDetectionMode::DISCRETE;

		/* Other */
		std::any m_userData;
		glm::vec3 m_previousScale = { 0.0f, 0.0f, 0.0f };
		static Eventing::Event<PhysicalObject&>	CreatedEvent;
		static Eventing::Event<PhysicalObject&>	DestroyedEvent;
		static Eventing::Event<btRigidBody&>		ConsiderEvent;
		static Eventing::Event<btRigidBody&>		UnconsiderEvent;

		/* Bullet relatives */
		std::unique_ptr<btMotionState>		m_motion;
		std::unique_ptr<btRigidBody>		m_body;

	protected:
		std::unique_ptr<btCollisionShape>	m_shape;
	};
}
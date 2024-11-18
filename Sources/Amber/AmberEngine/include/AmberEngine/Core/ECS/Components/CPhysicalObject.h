#pragma once


#include "AmberEngine/Core/ECS/Components/AComponent.h"

#include "AmberEngine/Physics/Entities/PhysicalObject.h"

namespace AmberEngine::Core::ECS { class Actor; }

namespace AmberEngine::Core::ECS::Components
{
	class CPhysicalObject : public AComponent
	{
	public:
		CPhysicalObject(ECS::Actor& p_owner);

		void AddForce(const glm::vec3& p_force);

		void AddImpulse(const glm::vec3& p_impulse);

		void ClearForces();

		float GetMass() const;

		Physics::Entities::PhysicalObject::ECollisionDetectionMode GetCollisionDetectionMode() const;

		float GetBounciness() const;

		float GetFriction()	const;

		glm::vec3 GetLinearVelocity() const;

		glm::vec3 GetAngularVelocity() const;

		glm::vec3 GetLinearFactor()	const;

		glm::vec3 GetAngularFactor() const;

		bool IsTrigger() const;

		bool IsKinematic() const;

		Physics::Entities::PhysicalObject::EActivationState GetActivationState() const;

		void SetMass(float p_mass);

		void SetCollisionDetectionMode(Physics::Entities::PhysicalObject::ECollisionDetectionMode p_mode);

		void SetBounciness(float p_bounciness);

		void SetFriction(float p_friction);

		void SetLinearVelocity(const glm::vec3& p_linearVelocity);

		void SetAngularVelocity(const glm::vec3& p_angularVelocity);

		void SetLinearFactor(const glm::vec3& p_linearFactor);

		void SetAngularFactor(const glm::vec3& p_angularFactor);

		void SetTrigger(bool p_trigger);

		void SetKinematic(bool p_kinematic);

		void SetActivationState(Physics::Entities::PhysicalObject::EActivationState p_activationState);

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(UI::WidgetContainer& p_root) override;

	protected:
		template <typename T> T& GetPhysicalObjectAs() const { return *static_cast<T*>(m_physicalObject.get()); }
		void Init();
		void BindListener();

	private:
		virtual void OnEnable() override;
		virtual void OnDisable() override;

	public:
		Eventing::Event<CPhysicalObject&> CollisionEnterEvent;
		Eventing::Event<CPhysicalObject&> CollisionStayEvent;
		Eventing::Event<CPhysicalObject&> CollisionExitEvent;

		Eventing::Event<CPhysicalObject&> TriggerEnterEvent;
		Eventing::Event<CPhysicalObject&> TriggerStayEvent;
		Eventing::Event<CPhysicalObject&> TriggerExitEvent;

	protected:
		std::unique_ptr<Physics::Entities::PhysicalObject> m_physicalObject;
	};
}
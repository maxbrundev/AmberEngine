#include "Amberpch.h"

#include "AmberEngine/Physics/Entities/PhysicalObject.h"
#include "AmberEngine/Physics/Tools/Conversion.h"

AmberEngine::Eventing::Event<AmberEngine::Physics::Entities::PhysicalObject&>	AmberEngine::Physics::Entities::PhysicalObject::CreatedEvent;
AmberEngine::Eventing::Event<AmberEngine::Physics::Entities::PhysicalObject&>	AmberEngine::Physics::Entities::PhysicalObject::DestroyedEvent;
AmberEngine::Eventing::Event<btRigidBody&>							AmberEngine::Physics::Entities::PhysicalObject::ConsiderEvent;
AmberEngine::Eventing::Event<btRigidBody&>							AmberEngine::Physics::Entities::PhysicalObject::UnconsiderEvent;

AmberEngine::Physics::Entities::PhysicalObject::PhysicalObject() :
	m_transform(new Maths::Transform()),
	m_internalTransform(true)
{
	CollisionStartEvent += [this](AmberEngine::Physics::Entities::PhysicalObject& otherPhysicalObject)
	{
		UpdateBtTransform();
	};
}

AmberEngine::Physics::Entities::PhysicalObject::PhysicalObject(Maths::Transform& p_transform) :
	m_transform(&p_transform),
	m_internalTransform(false)
{
}

AmberEngine::Physics::Entities::PhysicalObject::~PhysicalObject()
{
	DestroyBody();
	DestroyedEvent.Invoke(*this);

	if (m_internalTransform)
		delete m_transform;
}

void AmberEngine::Physics::Entities::PhysicalObject::Init()
{
	CreatedEvent.Invoke(*this);
	CreateBody({});
}

void AmberEngine::Physics::Entities::PhysicalObject::AddForce(const glm::vec3& p_force)
{
	m_body->applyCentralForce(Tools::Conversion::ToBtVector3(p_force));
}

void AmberEngine::Physics::Entities::PhysicalObject::AddImpulse(const glm::vec3& p_impulse)
{
	m_body->applyCentralImpulse(Tools::Conversion::ToBtVector3(p_impulse));
}

void AmberEngine::Physics::Entities::PhysicalObject::ClearForces()
{
	m_body->clearForces();
}

void AmberEngine::Physics::Entities::PhysicalObject::AddFlag(btCollisionObject::CollisionFlags p_flag)
{
	m_body->setCollisionFlags(m_body->getCollisionFlags() | p_flag);
}

void AmberEngine::Physics::Entities::PhysicalObject::RemoveFlag(btCollisionObject::CollisionFlags p_flag)
{
	m_body->setCollisionFlags(m_body->getCollisionFlags() & ~p_flag);
}

float AmberEngine::Physics::Entities::PhysicalObject::GetMass() const
{
	return m_mass;
}

const AmberEngine::Physics::Entities::PhysicalObject::ECollisionDetectionMode& AmberEngine::Physics::Entities::PhysicalObject::GetCollisionDetectionMode() const
{
	return m_collisionMode;
}

float AmberEngine::Physics::Entities::PhysicalObject::GetBounciness() const
{
	return m_body->getRestitution();
}

float AmberEngine::Physics::Entities::PhysicalObject::GetFriction() const
{
	return m_body->getFriction();
}

glm::vec3 AmberEngine::Physics::Entities::PhysicalObject::GetLinearVelocity() const
{
	return Tools::Conversion::ToOvVector3(m_body->getLinearVelocity());
}

glm::vec3 AmberEngine::Physics::Entities::PhysicalObject::GetAngularVelocity() const
{
	return Tools::Conversion::ToOvVector3(m_body->getAngularVelocity());
}

glm::vec3 AmberEngine::Physics::Entities::PhysicalObject::GetLinearFactor() const
{
	return Tools::Conversion::ToOvVector3(m_body->getLinearFactor());
}

glm::vec3 AmberEngine::Physics::Entities::PhysicalObject::GetAngularFactor() const
{
	return Tools::Conversion::ToOvVector3(m_body->getAngularFactor());
}

bool AmberEngine::Physics::Entities::PhysicalObject::IsTrigger() const
{
	return m_trigger;
}

bool AmberEngine::Physics::Entities::PhysicalObject::IsKinematic() const
{
	return m_kinematic;
}

AmberEngine::Physics::Entities::PhysicalObject::EActivationState AmberEngine::Physics::Entities::PhysicalObject::GetActivationState() const
{
	return static_cast<EActivationState>(m_body->getActivationState());
}

AmberEngine::Maths::Transform& AmberEngine::Physics::Entities::PhysicalObject::GetTransform()
{
	return *m_transform;
}

void AmberEngine::Physics::Entities::PhysicalObject::SetMass(float p_mass)
{
	m_mass = p_mass;
	ApplyInertia();
}

void AmberEngine::Physics::Entities::PhysicalObject::SetCollisionDetectionMode(ECollisionDetectionMode p_mode)
{
	m_collisionMode = p_mode;

	switch (m_collisionMode)
	{
	case ECollisionDetectionMode::DISCRETE:
		m_body->setCcdMotionThreshold(std::numeric_limits<float>::max());
		m_body->setCcdSweptSphereRadius(0.0f);
		break;
	case ECollisionDetectionMode::CONTINUOUS:
		m_body->setCcdMotionThreshold(static_cast<btScalar>(1e-7));
		m_body->setCcdSweptSphereRadius(0.5f);
		break;
	}
}

void AmberEngine::Physics::Entities::PhysicalObject::SetBounciness(float p_bounciness)
{
	m_body->setRestitution(p_bounciness);
}

void AmberEngine::Physics::Entities::PhysicalObject::SetFriction(float p_friction)
{
	m_body->setFriction(p_friction);
}

void AmberEngine::Physics::Entities::PhysicalObject::SetLinearVelocity(const glm::vec3 & p_linearVelocity)
{
	m_body->setLinearVelocity(Tools::Conversion::ToBtVector3(p_linearVelocity));
}

void AmberEngine::Physics::Entities::PhysicalObject::SetAngularVelocity(const glm::vec3 & p_angularVelocity)
{
	m_body->setAngularVelocity(Tools::Conversion::ToBtVector3(p_angularVelocity));
}

void AmberEngine::Physics::Entities::PhysicalObject::SetLinearFactor(const glm::vec3 & p_linearFactor)
{
	m_body->setLinearFactor(Tools::Conversion::ToBtVector3(p_linearFactor));
}

void AmberEngine::Physics::Entities::PhysicalObject::SetAngularFactor(const glm::vec3 & p_angularFactor)
{
	m_body->setAngularFactor(Tools::Conversion::ToBtVector3(p_angularFactor));
}

void AmberEngine::Physics::Entities::PhysicalObject::SetTrigger(bool p_trigger)
{
	if (p_trigger)
		AddFlag(btCollisionObject::CF_NO_CONTACT_RESPONSE);
	else
		RemoveFlag(btCollisionObject::CF_NO_CONTACT_RESPONSE);

	m_trigger = p_trigger;
}

void AmberEngine::Physics::Entities::PhysicalObject::SetKinematic(bool p_kinematic)
{
	m_kinematic = p_kinematic;

	if (m_kinematic)
	{
		ClearForces();
		SetLinearVelocity(glm::vec3(0.0f));
		SetAngularVelocity(glm::vec3(0.0f));
	}

	RecreateBody();
}

void AmberEngine::Physics::Entities::PhysicalObject::SetActivationState(EActivationState p_activationState)
{
	m_body->setActivationState(static_cast<int>(p_activationState));
}

void AmberEngine::Physics::Entities::PhysicalObject::SetEnabled(bool p_enabled)
{
	m_enabled = p_enabled;

	if (!m_enabled)
		Unconsider();
	else
		Consider();
}

bool AmberEngine::Physics::Entities::PhysicalObject::IsEnabled() const
{
	return m_enabled;
}

void AmberEngine::Physics::Entities::PhysicalObject::UpdateBtTransform()
{
	m_body->setWorldTransform(Tools::Conversion::ToBtTransform(*m_transform));

	if (glm::distance(m_transform->GetWorldScale(), m_previousScale) >= 0.01f)
	{
		m_previousScale = m_transform->GetWorldScale();
		SetLocalScaling({ abs(m_previousScale.x), abs(m_previousScale.y), abs(m_previousScale.z) });
		RecreateBody();
	}
}

void AmberEngine::Physics::Entities::PhysicalObject::UpdateFTransform()
{
	if (!m_kinematic)
	{
		const btTransform& result = m_body->getWorldTransform();
		m_transform->SetLocalPosition(Tools::Conversion::ToOvVector3(result.getOrigin()));
		m_transform->SetLocalRotation(Tools::Conversion::ToOvQuaternion(result.getRotation()));
	}
}

void AmberEngine::Physics::Entities::PhysicalObject::RecreateBody()
{
	CreateBody(DestroyBody());
}

void AmberEngine::Physics::Entities::PhysicalObject::ApplyInertia()
{
	m_body->setMassProps(m_kinematic ? 0.0f : std::max(0.0000001f, m_mass), m_kinematic ? btVector3(0.0f, 0.0f, 0.0f) : CalculateInertia());
}

void AmberEngine::Physics::Entities::PhysicalObject::Consider()
{
	if (!m_considered)
	{
		m_considered = true;
		ConsiderEvent.Invoke(*m_body);
	}
}

void AmberEngine::Physics::Entities::PhysicalObject::Unconsider()
{
	if (m_considered)
	{
		m_considered = false;
		UnconsiderEvent.Invoke(*m_body);
	}
}

void AmberEngine::Physics::Entities::PhysicalObject::CreateBody(const Settings::BodySettings & p_bodySettings)
{
	m_motion = std::make_unique<btDefaultMotionState>(Tools::Conversion::ToBtTransform(*m_transform));

	m_body = std::make_unique<btRigidBody>(btRigidBody::btRigidBodyConstructionInfo{ 0.0f, m_motion.get(), m_shape.get(), btVector3(0.0f, 0.0f, 0.0f) });

	ApplyInertia();

	m_body->setRestitution(p_bodySettings.restitution);
	m_body->setFriction(p_bodySettings.friction);
	m_body->setLinearVelocity(p_bodySettings.linearVelocity);
	m_body->setAngularVelocity(p_bodySettings.angularVelocity);
	m_body->setLinearFactor(p_bodySettings.linearFactor);
	m_body->setAngularFactor(p_bodySettings.angularFactor);
	m_body->setUserPointer(this);

	AddFlag(btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

	if (p_bodySettings.isTrigger)
		AddFlag(btCollisionObject::CF_NO_CONTACT_RESPONSE);

	SetActivationState(EActivationState::ALWAYS_ACTIVE); // TODO: Avoid using always active

	if (m_enabled)
		Consider();
}

AmberEngine::Physics::Settings::BodySettings AmberEngine::Physics::Entities::PhysicalObject::DestroyBody()
{
	Settings::BodySettings result
	{
		m_body->getLinearVelocity(),
		m_body->getAngularVelocity(),
		m_body->getLinearFactor(),
		m_body->getAngularFactor(),
		GetBounciness(),
		GetFriction(),
		IsTrigger(),
		IsKinematic()
	};

	Unconsider();

	m_body.reset();
	m_motion.reset();

	return result;
}

btVector3 AmberEngine::Physics::Entities::PhysicalObject::CalculateInertia() const
{
	btVector3 result = { 0.0f, 0.0f, 0.0f };

	if (m_mass != 0.0f)
		m_shape->calculateLocalInertia(m_mass, result);

	return result;
}

btRigidBody& AmberEngine::Physics::Entities::PhysicalObject::GetBody()
{
	return *m_body;
}

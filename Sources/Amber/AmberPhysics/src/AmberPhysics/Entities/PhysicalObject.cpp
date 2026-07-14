#include "Amberpch.h"

#include "AmberPhysics/Entities/PhysicalObject.h"
#include "AmberPhysics/Tools/Conversion.h"

AmberTools::Eventing::Event<AmberPhysics::Entities::PhysicalObject&>	AmberPhysics::Entities::PhysicalObject::CreatedEvent;
AmberTools::Eventing::Event<AmberPhysics::Entities::PhysicalObject&>	AmberPhysics::Entities::PhysicalObject::DestroyedEvent;
AmberTools::Eventing::Event<btRigidBody&>							AmberPhysics::Entities::PhysicalObject::ConsiderEvent;
AmberTools::Eventing::Event<btRigidBody&>							AmberPhysics::Entities::PhysicalObject::UnconsiderEvent;

AmberPhysics::Entities::PhysicalObject::PhysicalObject() :
	m_transform(new AmberMaths::Transform()),
	m_internalTransform(true)
{
	CollisionStartEvent += [this](AmberPhysics::Entities::PhysicalObject& otherPhysicalObject)
	{
		UpdateBtTransform();
	};
}

AmberPhysics::Entities::PhysicalObject::PhysicalObject(AmberMaths::Transform& p_transform) :
	m_transform(&p_transform),
	m_internalTransform(false)
{
}

AmberPhysics::Entities::PhysicalObject::~PhysicalObject()
{
	DestroyBody();
	DestroyedEvent.Invoke(*this);

	if (m_internalTransform)
		delete m_transform;
}

void AmberPhysics::Entities::PhysicalObject::Init()
{
	CreatedEvent.Invoke(*this);
	CreateBody({});
}

void AmberPhysics::Entities::PhysicalObject::AddForce(const glm::vec3& p_force)
{
	m_body->applyCentralForce(AmberPhysics::Tools::Conversion::ToBtVector3(p_force));
}

void AmberPhysics::Entities::PhysicalObject::AddImpulse(const glm::vec3& p_impulse)
{
	m_body->applyCentralImpulse(AmberPhysics::Tools::Conversion::ToBtVector3(p_impulse));
}

void AmberPhysics::Entities::PhysicalObject::ClearForces()
{
	m_body->clearForces();
}

void AmberPhysics::Entities::PhysicalObject::AddFlag(btCollisionObject::CollisionFlags p_flag)
{
	m_body->setCollisionFlags(m_body->getCollisionFlags() | p_flag);
}

void AmberPhysics::Entities::PhysicalObject::RemoveFlag(btCollisionObject::CollisionFlags p_flag)
{
	m_body->setCollisionFlags(m_body->getCollisionFlags() & ~p_flag);
}

float AmberPhysics::Entities::PhysicalObject::GetMass() const
{
	return m_mass;
}

const AmberPhysics::Entities::PhysicalObject::ECollisionDetectionMode& AmberPhysics::Entities::PhysicalObject::GetCollisionDetectionMode() const
{
	return m_collisionMode;
}

float AmberPhysics::Entities::PhysicalObject::GetBounciness() const
{
	return m_body->getRestitution();
}

float AmberPhysics::Entities::PhysicalObject::GetFriction() const
{
	return m_body->getFriction();
}

glm::vec3 AmberPhysics::Entities::PhysicalObject::GetLinearVelocity() const
{
	return AmberPhysics::Tools::Conversion::ToOvVector3(m_body->getLinearVelocity());
}

glm::vec3 AmberPhysics::Entities::PhysicalObject::GetAngularVelocity() const
{
	return AmberPhysics::Tools::Conversion::ToOvVector3(m_body->getAngularVelocity());
}

glm::vec3 AmberPhysics::Entities::PhysicalObject::GetLinearFactor() const
{
	return AmberPhysics::Tools::Conversion::ToOvVector3(m_body->getLinearFactor());
}

glm::vec3 AmberPhysics::Entities::PhysicalObject::GetAngularFactor() const
{
	return AmberPhysics::Tools::Conversion::ToOvVector3(m_body->getAngularFactor());
}

bool AmberPhysics::Entities::PhysicalObject::IsTrigger() const
{
	return m_trigger;
}

bool AmberPhysics::Entities::PhysicalObject::IsKinematic() const
{
	return m_kinematic;
}

AmberPhysics::Entities::PhysicalObject::EActivationState AmberPhysics::Entities::PhysicalObject::GetActivationState() const
{
	return static_cast<EActivationState>(m_body->getActivationState());
}

AmberMaths::Transform& AmberPhysics::Entities::PhysicalObject::GetTransform()
{
	return *m_transform;
}

void AmberPhysics::Entities::PhysicalObject::SetMass(float p_mass)
{
	m_mass = p_mass;
	ApplyInertia();
}

void AmberPhysics::Entities::PhysicalObject::SetCollisionDetectionMode(ECollisionDetectionMode p_mode)
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

void AmberPhysics::Entities::PhysicalObject::SetBounciness(float p_bounciness)
{
	m_body->setRestitution(p_bounciness);
}

void AmberPhysics::Entities::PhysicalObject::SetFriction(float p_friction)
{
	m_body->setFriction(p_friction);
}

void AmberPhysics::Entities::PhysicalObject::SetLinearVelocity(const glm::vec3 & p_linearVelocity)
{
	m_body->setLinearVelocity(AmberPhysics::Tools::Conversion::ToBtVector3(p_linearVelocity));
}

void AmberPhysics::Entities::PhysicalObject::SetAngularVelocity(const glm::vec3 & p_angularVelocity)
{
	m_body->setAngularVelocity(AmberPhysics::Tools::Conversion::ToBtVector3(p_angularVelocity));
}

void AmberPhysics::Entities::PhysicalObject::SetLinearFactor(const glm::vec3 & p_linearFactor)
{
	m_body->setLinearFactor(AmberPhysics::Tools::Conversion::ToBtVector3(p_linearFactor));
}

void AmberPhysics::Entities::PhysicalObject::SetAngularFactor(const glm::vec3 & p_angularFactor)
{
	m_body->setAngularFactor(AmberPhysics::Tools::Conversion::ToBtVector3(p_angularFactor));
}

void AmberPhysics::Entities::PhysicalObject::SetTrigger(bool p_trigger)
{
	if (p_trigger)
		AddFlag(btCollisionObject::CF_NO_CONTACT_RESPONSE);
	else
		RemoveFlag(btCollisionObject::CF_NO_CONTACT_RESPONSE);

	m_trigger = p_trigger;
}

void AmberPhysics::Entities::PhysicalObject::SetKinematic(bool p_kinematic)
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

void AmberPhysics::Entities::PhysicalObject::SetActivationState(EActivationState p_activationState)
{
	m_body->setActivationState(static_cast<int>(p_activationState));
}

void AmberPhysics::Entities::PhysicalObject::SetEnabled(bool p_enabled)
{
	m_enabled = p_enabled;

	if (!m_enabled)
		Unconsider();
	else
		Consider();
}

bool AmberPhysics::Entities::PhysicalObject::IsEnabled() const
{
	return m_enabled;
}

void AmberPhysics::Entities::PhysicalObject::UpdateBtTransform()
{
	m_body->setWorldTransform(AmberPhysics::Tools::Conversion::ToBtTransform(*m_transform));

	if (glm::distance(m_transform->GetWorldScale(), m_previousScale) >= 0.01f)
	{
		m_previousScale = m_transform->GetWorldScale();
		SetLocalScaling({ abs(m_previousScale.x), abs(m_previousScale.y), abs(m_previousScale.z) });
		RecreateBody();
	}
}

void AmberPhysics::Entities::PhysicalObject::UpdateFTransform()
{
	if (!m_kinematic)
	{
		const btTransform& result = m_body->getWorldTransform();
		m_transform->SetLocalPosition(AmberPhysics::Tools::Conversion::ToOvVector3(result.getOrigin()));
		m_transform->SetLocalRotation(AmberPhysics::Tools::Conversion::ToOvQuaternion(result.getRotation()));
	}
}

void AmberPhysics::Entities::PhysicalObject::RecreateBody()
{
	CreateBody(DestroyBody());
}

void AmberPhysics::Entities::PhysicalObject::ApplyInertia()
{
	m_body->setMassProps(m_kinematic ? 0.0f : std::max(0.0000001f, m_mass), m_kinematic ? btVector3(0.0f, 0.0f, 0.0f) : CalculateInertia());
}

void AmberPhysics::Entities::PhysicalObject::Consider()
{
	if (!m_considered)
	{
		m_considered = true;
		ConsiderEvent.Invoke(*m_body);
	}
}

void AmberPhysics::Entities::PhysicalObject::Unconsider()
{
	if (m_considered)
	{
		m_considered = false;
		UnconsiderEvent.Invoke(*m_body);
	}
}

void AmberPhysics::Entities::PhysicalObject::CreateBody(const Settings::BodySettings & p_bodySettings)
{
	m_motion = std::make_unique<btDefaultMotionState>(AmberPhysics::Tools::Conversion::ToBtTransform(*m_transform));

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

AmberPhysics::Settings::BodySettings AmberPhysics::Entities::PhysicalObject::DestroyBody()
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

btVector3 AmberPhysics::Entities::PhysicalObject::CalculateInertia() const
{
	btVector3 result = { 0.0f, 0.0f, 0.0f };

	if (m_mass != 0.0f)
		m_shape->calculateLocalInertia(m_mass, result);

	return result;
}

btRigidBody& AmberPhysics::Entities::PhysicalObject::GetBody()
{
	return *m_body;
}

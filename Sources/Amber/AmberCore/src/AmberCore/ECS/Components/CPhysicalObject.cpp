#include "Amberpch.h"

#include "AmberCore/ECS/Components/CPhysicalObject.h"
#include "AmberCore/ECS/Actor.h"
#include "AmberCore/Helpers/Serializer.h"

#include "AmberCore/Helpers/GUIDrawer.h"
#include "AmberUI/Widgets/ComboBox.h"

AmberCore::ECS::Components::CPhysicalObject::CPhysicalObject(AmberCore::ECS::Actor & p_owner) : AComponent(p_owner)
{
}

void AmberCore::ECS::Components::CPhysicalObject::AddForce(const glm::vec3 & p_force)
{
	m_physicalObject->AddForce(p_force);
}

void AmberCore::ECS::Components::CPhysicalObject::AddImpulse(const glm::vec3 & p_impulse)
{
	m_physicalObject->AddImpulse(p_impulse);
}

void AmberCore::ECS::Components::CPhysicalObject::ClearForces()
{
	m_physicalObject->ClearForces();
}

void AmberCore::ECS::Components::CPhysicalObject::SetCollisionDetectionMode(AmberPhysics::Entities::PhysicalObject::ECollisionDetectionMode p_mode)
{
	m_physicalObject->SetCollisionDetectionMode(p_mode);
}

AmberPhysics::Entities::PhysicalObject::ECollisionDetectionMode AmberCore::ECS::Components::CPhysicalObject::GetCollisionDetectionMode() const
{
	return m_physicalObject->GetCollisionDetectionMode();
}

float AmberCore::ECS::Components::CPhysicalObject::GetMass() const
{
	return m_physicalObject->GetMass();
}

float AmberCore::ECS::Components::CPhysicalObject::GetBounciness() const
{
	return m_physicalObject->GetBounciness();
}

float AmberCore::ECS::Components::CPhysicalObject::GetFriction() const
{
	return m_physicalObject->GetFriction();
}

glm::vec3 AmberCore::ECS::Components::CPhysicalObject::GetLinearVelocity() const
{
	return m_physicalObject->GetLinearVelocity();
}

glm::vec3 AmberCore::ECS::Components::CPhysicalObject::GetAngularVelocity() const
{
	return m_physicalObject->GetAngularVelocity();
}

glm::vec3 AmberCore::ECS::Components::CPhysicalObject::GetLinearFactor() const
{
	return m_physicalObject->GetLinearFactor();
}

glm::vec3 AmberCore::ECS::Components::CPhysicalObject::GetAngularFactor() const
{
	return m_physicalObject->GetAngularFactor();
}

bool AmberCore::ECS::Components::CPhysicalObject::IsTrigger() const
{
	return m_physicalObject->IsTrigger();
}

bool AmberCore::ECS::Components::CPhysicalObject::IsKinematic() const
{
	return m_physicalObject->IsKinematic();
}

AmberPhysics::Entities::PhysicalObject::EActivationState AmberCore::ECS::Components::CPhysicalObject::GetActivationState() const
{
	return m_physicalObject->GetActivationState();
}

void AmberCore::ECS::Components::CPhysicalObject::SetMass(float p_mass)
{
	m_physicalObject->SetMass(p_mass);
}

void AmberCore::ECS::Components::CPhysicalObject::SetBounciness(float p_bounciness)
{
	m_physicalObject->SetBounciness(p_bounciness);
}

void AmberCore::ECS::Components::CPhysicalObject::SetFriction(float p_friction)
{
	m_physicalObject->SetFriction(p_friction);
}

void AmberCore::ECS::Components::CPhysicalObject::SetLinearVelocity(const glm::vec3 & p_linearVelocity)
{
	m_physicalObject->SetLinearVelocity(p_linearVelocity);
}

void AmberCore::ECS::Components::CPhysicalObject::SetAngularVelocity(const glm::vec3 & p_angularVelocity)
{
	m_physicalObject->SetAngularVelocity(p_angularVelocity);
}

void AmberCore::ECS::Components::CPhysicalObject::SetLinearFactor(const glm::vec3 & p_linearFactor)
{
	m_physicalObject->SetLinearFactor(p_linearFactor);
}

void AmberCore::ECS::Components::CPhysicalObject::SetAngularFactor(const glm::vec3 & p_angularFactor)
{
	m_physicalObject->SetAngularFactor(p_angularFactor);
}

void AmberCore::ECS::Components::CPhysicalObject::SetTrigger(bool p_trigger)
{
	m_physicalObject->SetTrigger(p_trigger);
}

void AmberCore::ECS::Components::CPhysicalObject::SetKinematic(bool p_kinematic)
{
	m_physicalObject->SetKinematic(p_kinematic);
}

void AmberCore::ECS::Components::CPhysicalObject::SetActivationState(AmberPhysics::Entities::PhysicalObject::EActivationState p_state)
{
	m_physicalObject->SetActivationState(p_state);
}

void AmberCore::ECS::Components::CPhysicalObject::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	Helpers::Serializer::SerializeBoolean(p_doc, p_node, "is_trigger", IsTrigger());
	Helpers::Serializer::SerializeBoolean(p_doc, p_node, "is_kinematic", IsKinematic());
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "bounciness", GetBounciness());
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "mass", GetMass());
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "friction", GetFriction());
	Helpers::Serializer::SerializeVec3(p_doc, p_node, "linear_factor", GetLinearFactor());
	Helpers::Serializer::SerializeVec3(p_doc, p_node, "angular_factor", GetAngularFactor());
	Helpers::Serializer::SerializeInt(p_doc, p_node, "collision_mode", static_cast<int>(GetCollisionDetectionMode()));
}

void AmberCore::ECS::Components::CPhysicalObject::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	SetTrigger(Helpers::Serializer::DeserializeBoolean(p_doc, p_node, "is_trigger"));
	SetKinematic(Helpers::Serializer::DeserializeBoolean(p_doc, p_node, "is_kinematic"));
	SetBounciness(Helpers::Serializer::DeserializeFloat(p_doc, p_node, "bounciness"));
	SetMass(Helpers::Serializer::DeserializeFloat(p_doc, p_node, "mass"));
	SetFriction(Helpers::Serializer::DeserializeFloat(p_doc, p_node, "friction"));
	SetLinearFactor(Helpers::Serializer::DeserializeVec3(p_doc, p_node, "linear_factor"));
	SetAngularFactor(Helpers::Serializer::DeserializeVec3(p_doc, p_node, "angular_factor"));
	SetCollisionDetectionMode(static_cast<AmberPhysics::Entities::PhysicalObject::ECollisionDetectionMode>(Helpers::Serializer::DeserializeInt(p_doc, p_node, "collision_mode")));
}

void AmberCore::ECS::Components::CPhysicalObject::OnInspector(AmberUI::WidgetContainer & p_root)
{
	AmberCore::Helpers::GUIDrawer::DrawBoolean(p_root, "Trigger", std::bind(&CPhysicalObject::IsTrigger, this), std::bind(&CPhysicalObject::SetTrigger, this, std::placeholders::_1));
	AmberCore::Helpers::GUIDrawer::DrawBoolean(p_root, "Kinematic", std::bind(&CPhysicalObject::IsKinematic, this), std::bind(&CPhysicalObject::SetKinematic, this, std::placeholders::_1));
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Mass", std::bind(&CPhysicalObject::GetMass, this), std::bind(&CPhysicalObject::SetMass, this, std::placeholders::_1), 0.1f, 0.f, 10000.f);
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Bounciness", std::bind(&CPhysicalObject::GetBounciness, this), std::bind(&CPhysicalObject::SetBounciness, this, std::placeholders::_1), 0.1f, 0.f, 1.f);
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Friction", std::bind(&CPhysicalObject::GetFriction, this), std::bind(&CPhysicalObject::SetFriction, this, std::placeholders::_1), 0.1f, 0.f, 1.f);
	AmberCore::Helpers::GUIDrawer::DrawVec3(p_root, "Linear Factor", std::bind(&CPhysicalObject::GetLinearFactor, this), std::bind(&CPhysicalObject::SetLinearFactor, this, std::placeholders::_1), 0.1f, 0.f, 1.f);
	AmberCore::Helpers::GUIDrawer::DrawVec3(p_root, "Angular Factor", std::bind(&CPhysicalObject::GetAngularFactor, this), std::bind(&CPhysicalObject::SetAngularFactor, this, std::placeholders::_1), 0.1f, 0.f, 1.f);

	AmberCore::Helpers::GUIDrawer::CreateTitle(p_root, "Collision Mode");
	auto& collisionMode = p_root.CreateWidget<AmberUI::Widgets::ComboBox>(static_cast<int>(GetCollisionDetectionMode()));
	collisionMode.Choices.emplace(0, "Discrete");
	collisionMode.Choices.emplace(1, "Continuous");
	collisionMode.ValueChangedEvent += [this](int p_choice)
	{
		SetCollisionDetectionMode(static_cast<AmberPhysics::Entities::PhysicalObject::ECollisionDetectionMode>(p_choice));
	};
}

void AmberCore::ECS::Components::CPhysicalObject::Init()
{
	m_physicalObject->SetEnabled(false);
}

void AmberCore::ECS::Components::CPhysicalObject::BindListener()
{
	/* Collision Events */
	m_physicalObject->CollisionStartEvent += [this](AmberPhysics::Entities::PhysicalObject& otherPhysicalObject)
	{
		auto& otherObject = otherPhysicalObject.GetUserData<std::reference_wrapper<CPhysicalObject>>().get();

		CollisionEnterEvent.Invoke(otherObject);
		Owner.OnCollisionEnter(otherObject);
	};
	m_physicalObject->CollisionStayEvent += [this](AmberPhysics::Entities::PhysicalObject& otherPhysicalObject)
	{
		auto& otherObject = otherPhysicalObject.GetUserData<std::reference_wrapper<CPhysicalObject>>().get();

		CollisionStayEvent.Invoke(otherObject);
		Owner.OnCollisionStay(otherObject);
	};
	m_physicalObject->CollisionStopEvent += [this](AmberPhysics::Entities::PhysicalObject& otherPhysicalObject)
	{
		auto& otherObject = otherPhysicalObject.GetUserData<std::reference_wrapper<CPhysicalObject>>().get();

		CollisionExitEvent.Invoke(otherObject);
		Owner.OnCollisionExit(otherObject);
	};

	/* Trigger Events */
	m_physicalObject->TriggerStartEvent += [this](AmberPhysics::Entities::PhysicalObject& otherPhysicalObject)
	{
		auto& otherObject = otherPhysicalObject.GetUserData<std::reference_wrapper<CPhysicalObject>>().get();

		TriggerEnterEvent.Invoke(otherObject);
		Owner.OnTriggerEnter(otherObject);
	};
	m_physicalObject->TriggerStayEvent += [this](AmberPhysics::Entities::PhysicalObject& otherPhysicalObject)
	{
		auto& otherObject = otherPhysicalObject.GetUserData<std::reference_wrapper<CPhysicalObject>>().get();

		TriggerStayEvent.Invoke(otherObject);
		Owner.OnTriggerStay(otherObject);
	};
	m_physicalObject->TriggerStopEvent += [this](AmberPhysics::Entities::PhysicalObject& otherPhysicalObject)
	{
		auto& otherObject = otherPhysicalObject.GetUserData<std::reference_wrapper<CPhysicalObject>>().get();

		TriggerExitEvent.Invoke(otherObject);
		Owner.OnTriggerExit(otherObject);
	};
}

void AmberCore::ECS::Components::CPhysicalObject::OnEnable()
{
	m_physicalObject->SetEnabled(true);
}

void AmberCore::ECS::Components::CPhysicalObject::OnDisable()
{
	m_physicalObject->SetEnabled(false);
}

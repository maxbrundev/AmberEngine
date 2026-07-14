#include "Amberpch.h"

#include "AmberCore/ECS/Components/CPhysicalCapsule.h"

#include "AmberCore/ECS/Actor.h"
#include "AmberCore/Helpers/Serializer.h"
#include "AmberPhysics/Entities/PhysicalCapsule.h"
#include "AmberCore/Helpers/GUIDrawer.h"

AmberCore::ECS::Components::CPhysicalCapsule::CPhysicalCapsule(ECS::Actor& p_owner) : CPhysicalObject(p_owner)
{
	m_physicalObject = std::make_unique<AmberPhysics::Entities::PhysicalCapsule>(p_owner.transform.GetTransform());

	m_physicalObject->SetUserData<std::reference_wrapper<CPhysicalObject>>(*this);

	BindListener();
	Init();
}

std::string AmberCore::ECS::Components::CPhysicalCapsule::GetName()
{
	return "Physical Capsule";
}

void AmberCore::ECS::Components::CPhysicalCapsule::SetRadius(float p_radius)
{
	GetPhysicalObjectAs<AmberPhysics::Entities::PhysicalCapsule>().SetRadius(p_radius);
}

void AmberCore::ECS::Components::CPhysicalCapsule::SetHeight(float p_height)
{
	GetPhysicalObjectAs<AmberPhysics::Entities::PhysicalCapsule>().SetHeight(p_height);
}

float AmberCore::ECS::Components::CPhysicalCapsule::GetRadius() const
{
	return GetPhysicalObjectAs<AmberPhysics::Entities::PhysicalCapsule>().GetRadius();
}

float AmberCore::ECS::Components::CPhysicalCapsule::GetHeight() const
{
	return GetPhysicalObjectAs<AmberPhysics::Entities::PhysicalCapsule>().GetHeight();
}

void AmberCore::ECS::Components::CPhysicalCapsule::OnSerialize(tinyxml2::XMLDocument& p_doc,
	tinyxml2::XMLNode* p_node)
{
	CPhysicalObject::OnSerialize(p_doc, p_node);

	Helpers::Serializer::SerializeFloat(p_doc, p_node, "radius", GetRadius());
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "height", GetHeight());
}

void AmberCore::ECS::Components::CPhysicalCapsule::OnDeserialize(tinyxml2::XMLDocument& p_doc,
	tinyxml2::XMLNode* p_node)
{
	CPhysicalObject::OnDeserialize(p_doc, p_node);

	SetRadius(Helpers::Serializer::DeserializeFloat(p_doc, p_node, "radius"));
	SetHeight(Helpers::Serializer::DeserializeFloat(p_doc, p_node, "height"));
}

void AmberCore::ECS::Components::CPhysicalCapsule::OnInspector(AmberUI::WidgetContainer& p_root)
{
	CPhysicalObject::OnInspector(p_root);

	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Radius", std::bind(&CPhysicalCapsule::GetRadius, this), std::bind(&CPhysicalCapsule::SetRadius, this, std::placeholders::_1), 0.1f, 0.f, 100000.f);
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Height", std::bind(&CPhysicalCapsule::GetHeight, this), std::bind(&CPhysicalCapsule::SetHeight, this, std::placeholders::_1), 0.1f, 0.f, 100000.f);
}

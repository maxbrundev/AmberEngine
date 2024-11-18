#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Components/CPhysicalCapsule.h"

#include "AmberEngine/Core/ECS/Actor.h"
#include "AmberEngine/Core/Helpers/Serializer.h"
#include "AmberEngine/Physics/Entities/PhysicalCapsule.h"
#include "AmberEngine/UI/GUIDrawer.h"

AmberEngine::Core::ECS::Components::CPhysicalCapsule::CPhysicalCapsule(ECS::Actor& p_owner) : CPhysicalObject(p_owner)
{
	m_physicalObject = std::make_unique<Physics::Entities::PhysicalCapsule>(p_owner.transform.GetTransform());

	m_physicalObject->SetUserData<std::reference_wrapper<CPhysicalObject>>(*this);

	BindListener();
	Init();
}

std::string AmberEngine::Core::ECS::Components::CPhysicalCapsule::GetName()
{
	return "Physical Capsule";
}

void AmberEngine::Core::ECS::Components::CPhysicalCapsule::SetRadius(float p_radius)
{
	GetPhysicalObjectAs<Physics::Entities::PhysicalCapsule>().SetRadius(p_radius);
}

void AmberEngine::Core::ECS::Components::CPhysicalCapsule::SetHeight(float p_height)
{
	GetPhysicalObjectAs<Physics::Entities::PhysicalCapsule>().SetHeight(p_height);
}

float AmberEngine::Core::ECS::Components::CPhysicalCapsule::GetRadius() const
{
	return GetPhysicalObjectAs<Physics::Entities::PhysicalCapsule>().GetRadius();
}

float AmberEngine::Core::ECS::Components::CPhysicalCapsule::GetHeight() const
{
	return GetPhysicalObjectAs<Physics::Entities::PhysicalCapsule>().GetHeight();
}

void AmberEngine::Core::ECS::Components::CPhysicalCapsule::OnSerialize(tinyxml2::XMLDocument& p_doc,
	tinyxml2::XMLNode* p_node)
{
	CPhysicalObject::OnSerialize(p_doc, p_node);

	Helpers::Serializer::SerializeFloat(p_doc, p_node, "radius", GetRadius());
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "height", GetHeight());
}

void AmberEngine::Core::ECS::Components::CPhysicalCapsule::OnDeserialize(tinyxml2::XMLDocument& p_doc,
	tinyxml2::XMLNode* p_node)
{
	CPhysicalObject::OnDeserialize(p_doc, p_node);

	SetRadius(Helpers::Serializer::DeserializeFloat(p_doc, p_node, "radius"));
	SetHeight(Helpers::Serializer::DeserializeFloat(p_doc, p_node, "height"));
}

void AmberEngine::Core::ECS::Components::CPhysicalCapsule::OnInspector(UI::WidgetContainer& p_root)
{
	CPhysicalObject::OnInspector(p_root);

	UI::GUIDrawer::DrawScalar<float>(p_root, "Radius", std::bind(&CPhysicalCapsule::GetRadius, this), std::bind(&CPhysicalCapsule::SetRadius, this, std::placeholders::_1), 0.1f, 0.f, 100000.f);
	UI::GUIDrawer::DrawScalar<float>(p_root, "Height", std::bind(&CPhysicalCapsule::GetHeight, this), std::bind(&CPhysicalCapsule::SetHeight, this, std::placeholders::_1), 0.1f, 0.f, 100000.f);
}

#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Components/CPhysicalSphere.h"
#include "AmberEngine/Core/ECS/Actor.h"
#include "AmberEngine/Core/Helpers/Serializer.h"

#include "AmberEngine/Physics/Entities/PhysicalSphere.h"

#include "AmberEngine/UI/GUIDrawer.h"

AmberEngine::Core::ECS::Components::CPhysicalSphere::CPhysicalSphere(ECS::Actor& p_owner) : CPhysicalObject(p_owner)
{
	m_physicalObject = std::make_unique<Physics::Entities::PhysicalSphere>(p_owner.transform.GetTransform());

	m_physicalObject->SetUserData<std::reference_wrapper<CPhysicalObject>>(*this);

	BindListener();
	Init();
}

std::string AmberEngine::Core::ECS::Components::CPhysicalSphere::GetName()
{
	return "Physical Sphere";
}

void AmberEngine::Core::ECS::Components::CPhysicalSphere::SetRadius(float p_radius)
{
	GetPhysicalObjectAs<Physics::Entities::PhysicalSphere>().SetRadius(p_radius);
}

float AmberEngine::Core::ECS::Components::CPhysicalSphere::GetRadius() const
{
	return GetPhysicalObjectAs<Physics::Entities::PhysicalSphere>().GetRadius();
}

void AmberEngine::Core::ECS::Components::CPhysicalSphere::OnSerialize(tinyxml2::XMLDocument& p_doc,
	tinyxml2::XMLNode* p_node)
{
	CPhysicalObject::OnSerialize(p_doc, p_node);

	Helpers::Serializer::SerializeFloat(p_doc, p_node, "radius", GetRadius());
}

void AmberEngine::Core::ECS::Components::CPhysicalSphere::OnDeserialize(tinyxml2::XMLDocument& p_doc,
	tinyxml2::XMLNode* p_node)
{
	CPhysicalObject::OnDeserialize(p_doc, p_node);

	SetRadius(Helpers::Serializer::DeserializeFloat(p_doc, p_node, "radius"));
}

void AmberEngine::Core::ECS::Components::CPhysicalSphere::OnInspector(UI::WidgetContainer& p_root)
{
	CPhysicalObject::OnInspector(p_root);

	UI::GUIDrawer::DrawScalar<float>(p_root, "Radius", std::bind(&CPhysicalSphere::GetRadius, this), std::bind(&CPhysicalSphere::SetRadius, this, std::placeholders::_1), 0.1f, 0.f, 100000.f);
}

#include "Amberpch.h"

#include "AmberCore/ECS/Components/CPhysicalSphere.h"
#include "AmberCore/ECS/Actor.h"
#include "AmberCore/Helpers/Serializer.h"

#include "AmberPhysics/Entities/PhysicalSphere.h"

#include "AmberCore/Helpers/GUIDrawer.h"

AmberCore::ECS::Components::CPhysicalSphere::CPhysicalSphere(ECS::Actor& p_owner) : CPhysicalObject(p_owner)
{
	m_physicalObject = std::make_unique<AmberPhysics::Entities::PhysicalSphere>(p_owner.transform.GetTransform());

	m_physicalObject->SetUserData<std::reference_wrapper<CPhysicalObject>>(*this);

	BindListener();
	Init();
}

std::string AmberCore::ECS::Components::CPhysicalSphere::GetName()
{
	return "Physical Sphere";
}

void AmberCore::ECS::Components::CPhysicalSphere::SetRadius(float p_radius)
{
	GetPhysicalObjectAs<AmberPhysics::Entities::PhysicalSphere>().SetRadius(p_radius);
}

float AmberCore::ECS::Components::CPhysicalSphere::GetRadius() const
{
	return GetPhysicalObjectAs<AmberPhysics::Entities::PhysicalSphere>().GetRadius();
}

void AmberCore::ECS::Components::CPhysicalSphere::OnSerialize(tinyxml2::XMLDocument& p_doc,
	tinyxml2::XMLNode* p_node)
{
	CPhysicalObject::OnSerialize(p_doc, p_node);

	Helpers::Serializer::SerializeFloat(p_doc, p_node, "radius", GetRadius());
}

void AmberCore::ECS::Components::CPhysicalSphere::OnDeserialize(tinyxml2::XMLDocument& p_doc,
	tinyxml2::XMLNode* p_node)
{
	CPhysicalObject::OnDeserialize(p_doc, p_node);

	SetRadius(Helpers::Serializer::DeserializeFloat(p_doc, p_node, "radius"));
}

void AmberCore::ECS::Components::CPhysicalSphere::OnInspector(AmberUI::WidgetContainer& p_root)
{
	CPhysicalObject::OnInspector(p_root);

	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Radius", std::bind(&CPhysicalSphere::GetRadius, this), std::bind(&CPhysicalSphere::SetRadius, this, std::placeholders::_1), 0.1f, 0.f, 100000.f);
}

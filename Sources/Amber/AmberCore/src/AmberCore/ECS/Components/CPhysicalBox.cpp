#include "Amberpch.h"
#include "AmberCore/ECS/Components/CPhysicalBox.h"

#include "AmberPhysics/Entities/PhysicalBox.h"
#include "AmberCore/ECS/Actor.h"
#include "AmberCore/Helpers/Serializer.h"
#include "AmberCore/Helpers/GUIDrawer.h"

AmberCore::ECS::Components::CPhysicalBox::CPhysicalBox(AmberCore::ECS::Actor & p_owner) :
	CPhysicalObject(p_owner)
{
	m_physicalObject = std::make_unique<AmberPhysics::Entities::PhysicalBox>(p_owner.transform.GetTransform());

	m_physicalObject->SetUserData<std::reference_wrapper<CPhysicalObject>>(*this);

	BindListener();
	Init();
}

std::string AmberCore::ECS::Components::CPhysicalBox::GetName()
{
	return "Physical Box";
}

void AmberCore::ECS::Components::CPhysicalBox::SetSize(const glm::vec3 & p_size)
{
	GetPhysicalObjectAs<AmberPhysics::Entities::PhysicalBox>().SetSize(p_size);
}

glm::vec3 AmberCore::ECS::Components::CPhysicalBox::GetSize() const
{
	return GetPhysicalObjectAs<AmberPhysics::Entities::PhysicalBox>().GetSize();
}

void AmberCore::ECS::Components::CPhysicalBox::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	CPhysicalObject::OnSerialize(p_doc, p_node);

	Helpers::Serializer::SerializeVec3(p_doc, p_node, "size", GetSize());
}

void AmberCore::ECS::Components::CPhysicalBox::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	CPhysicalObject::OnDeserialize(p_doc, p_node);

	SetSize(Helpers::Serializer::DeserializeVec3(p_doc, p_node, "size"));
}

void AmberCore::ECS::Components::CPhysicalBox::OnInspector(AmberUI::WidgetContainer & p_root)
{
	CPhysicalObject::OnInspector(p_root);

	AmberCore::Helpers::GUIDrawer::DrawVec3(p_root, "Size", std::bind(&CPhysicalBox::GetSize, this), std::bind(&CPhysicalBox::SetSize, this, std::placeholders::_1), 0.1f, 0.f, 100000.f);
}
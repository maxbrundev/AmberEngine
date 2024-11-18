/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Amberpch.h"
#include "AmberEngine/Core/ECS/Components/CPhysicalBox.h"

#include "AmberEngine/Physics/Entities/PhysicalBox.h"
#include "AmberEngine/Core/ECS/Actor.h"
#include "AmberEngine/Core/Helpers/Serializer.h"
#include "AmberEngine/UI/GUIDrawer.h"

AmberEngine::Core::ECS::Components::CPhysicalBox::CPhysicalBox(AmberEngine::Core::ECS::Actor & p_owner) :
	CPhysicalObject(p_owner)
{
	m_physicalObject = std::make_unique<Physics::Entities::PhysicalBox>(p_owner.transform.GetTransform());

	m_physicalObject->SetUserData<std::reference_wrapper<CPhysicalObject>>(*this);

	BindListener();
	Init();
}

std::string AmberEngine::Core::ECS::Components::CPhysicalBox::GetName()
{
	return "Physical Box";
}

void AmberEngine::Core::ECS::Components::CPhysicalBox::SetSize(const glm::vec3 & p_size)
{
	GetPhysicalObjectAs<Physics::Entities::PhysicalBox>().SetSize(p_size);
}

glm::vec3 AmberEngine::Core::ECS::Components::CPhysicalBox::GetSize() const
{
	return GetPhysicalObjectAs<Physics::Entities::PhysicalBox>().GetSize();
}

void AmberEngine::Core::ECS::Components::CPhysicalBox::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	CPhysicalObject::OnSerialize(p_doc, p_node);

	Helpers::Serializer::SerializeVec3(p_doc, p_node, "size", GetSize());
}

void AmberEngine::Core::ECS::Components::CPhysicalBox::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	CPhysicalObject::OnDeserialize(p_doc, p_node);

	SetSize(Helpers::Serializer::DeserializeVec3(p_doc, p_node, "size"));
}

void AmberEngine::Core::ECS::Components::CPhysicalBox::OnInspector(UI::WidgetContainer & p_root)
{
	CPhysicalObject::OnInspector(p_root);

	UI::GUIDrawer::DrawVec3(p_root, "Size", std::bind(&CPhysicalBox::GetSize, this), std::bind(&CPhysicalBox::SetSize, this, std::placeholders::_1), 0.1f, 0.f, 100000.f);
}
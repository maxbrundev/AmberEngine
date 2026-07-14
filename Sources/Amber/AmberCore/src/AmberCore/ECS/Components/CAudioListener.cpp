#include "Amberpch.h"

#include "AmberCore/ECS/Components/CAudioListener.h"

#include "AmberCore/ECS/Actor.h"

AmberCore::ECS::Components::CAudioListener::CAudioListener(Actor& p_owner) :
	AComponent(p_owner),
	m_audioListener(Owner.transform.GetTransform())
{
	m_audioListener.SetEnabled(false);
}

std::string AmberCore::ECS::Components::CAudioListener::GetName()
{
	return "Audio Listener";
}

void AmberCore::ECS::Components::CAudioListener::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
}

void AmberCore::ECS::Components::CAudioListener::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
}

void AmberCore::ECS::Components::CAudioListener::OnInspector(AmberUI::WidgetContainer& p_root)
{
}

void AmberCore::ECS::Components::CAudioListener::OnEnable()
{
	m_audioListener.SetEnabled(true);
}

void AmberCore::ECS::Components::CAudioListener::OnDisable()
{
	m_audioListener.SetEnabled(false);
}

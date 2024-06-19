#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Actor.h"

#include "AmberEngine/Core/ECS/Components/CDirectionalLight.h"
#include "AmberEngine/Core/ECS/Components/CMaterialRenderer.h"
#include "AmberEngine/Core/ECS/Components/CModelRenderer.h"
#include "AmberEngine/Core/ECS/Components/CPointLight.h"
#include "AmberEngine/Core/Helpers/Serializer.h"

AmberEngine::Eventing::Event<AmberEngine::Core::ECS::Actor&> AmberEngine::Core::ECS::Actor::CreatedEvent;
AmberEngine::Eventing::Event<AmberEngine::Core::ECS::Actor&> AmberEngine::Core::ECS::Actor::DestroyEvent;
AmberEngine::Eventing::Event<AmberEngine::Core::ECS::Actor&, AmberEngine::Core::ECS::Actor&> AmberEngine::Core::ECS::Actor::AttachEvent;
AmberEngine::Eventing::Event<AmberEngine::Core::ECS::Actor&, AmberEngine::Core::ECS::Actor*> AmberEngine::Core::ECS::Actor::DettachEvent;

AmberEngine::Core::ECS::Actor::Actor(int64_t p_actorID, const std::string& p_name, const std::string & p_tag) :
m_name(p_name),
m_tag(p_tag),
m_actorID(p_actorID),
transform(AddComponent<Components::CTransform>())
{
	CreatedEvent.Invoke(*this);
}

AmberEngine::Core::ECS::Actor::~Actor()
{
	DestroyEvent.Invoke(*this);

	std::vector<Actor*> toDetach = m_children;

	for (const auto child : toDetach)
	{
		child->RemoveParent();
	}

	toDetach.clear();
	m_children.clear();

	RemoveParent();

	std::for_each(m_components.begin(), m_components.end(), [&](const std::shared_ptr<Components::AComponent>& p_component) { ComponentRemovedEvent.Invoke(*p_component); });

	m_components.clear();
}

bool AmberEngine::Core::ECS::Actor::RemoveComponent(Components::AComponent& p_component)
{
	for (auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		if (it->get() == &p_component)
		{
			m_components.erase(it);
			return true;
		}
	}

	return false;
}

std::vector<std::shared_ptr<AmberEngine::Core::ECS::Components::AComponent>>& AmberEngine::Core::ECS::Actor::GetComponents()
{
	return m_components;
}

void AmberEngine::Core::ECS::Actor::SetName(std::string p_name)
{
	m_name = std::move(p_name);
}

void AmberEngine::Core::ECS::Actor::SetID(int64_t p_id)
{
	m_parentID = p_id;
}

void AmberEngine::Core::ECS::Actor::SetParent(Actor& p_parent)
{
	RemoveParent();

	m_parent = &p_parent;
	m_parentID = p_parent.m_actorID;
	transform.SetParent(p_parent.transform);
	p_parent.m_children.push_back(this);

	AttachEvent.Invoke(*this, p_parent);
}

void AmberEngine::Core::ECS::Actor::RemoveParent()
{
	DettachEvent.Invoke(*this, m_parent);

	if (m_parent)
	{
		auto predicate = [this](const Actor* p_element)
		{
			return p_element == this;
		};

		m_parent->m_children.erase(std::remove_if(m_parent->m_children.begin(), m_parent->m_children.end(),
		                                          std::ref(predicate)));
	}

	m_parent = nullptr;
	m_parentID = 0;

	transform.RemoveParent();
}

bool AmberEngine::Core::ECS::Actor::HasParent() const
{
	return m_parent != nullptr;
}

bool AmberEngine::Core::ECS::Actor::HasChildren() const
{
	return !m_children.empty();
}

bool AmberEngine::Core::ECS::Actor::IsDescendantOf(const Actor* p_actor) const
{
	const Actor* currentParentActor = m_parent;

	while (currentParentActor != nullptr)
	{
		if (currentParentActor == p_actor)
		{
			return true;
		}
		currentParentActor = currentParentActor->GetParent();
	}

	return false;
}

void AmberEngine::Core::ECS::Actor::MarkAsDestroy()
{
	m_destroyed = true;

	for (auto child : m_children)
		child->MarkAsDestroy();
}

bool AmberEngine::Core::ECS::Actor::IsAlive() const
{
	return !m_destroyed;
}

const std::string& AmberEngine::Core::ECS::Actor::GetName() const
{
	return m_name;
}

const std::string& AmberEngine::Core::ECS::Actor::GetTag() const
{
	return m_tag;
}

int64_t AmberEngine::Core::ECS::Actor::GetID() const
{
	return m_actorID;
}

AmberEngine::Core::ECS::Actor* AmberEngine::Core::ECS::Actor::GetParent() const
{
	return m_parent;
}

int64_t AmberEngine::Core::ECS::Actor::GetParentID() const
{
	return m_parentID;
}

void AmberEngine::Core::ECS::Actor::SetActive(bool p_active)
{
	if (p_active != m_active)
	{
		m_active = p_active;
	}
}

bool AmberEngine::Core::ECS::Actor::IsSelfActive() const
{
	return m_active;
}

bool AmberEngine::Core::ECS::Actor::IsActive() const
{
	return m_active && (m_parent ? m_parent->IsActive() : true);
}

std::vector<AmberEngine::Core::ECS::Actor*>& AmberEngine::Core::ECS::Actor::GetChildren()
{
	return m_children;
}

void AmberEngine::Core::ECS::Actor::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	tinyxml2::XMLNode* actorNode = p_doc.NewElement("actor");
	p_node->InsertEndChild(actorNode);

	Helpers::Serializer::SerializeString(p_doc, actorNode, "name", m_name);
	Helpers::Serializer::SerializeString(p_doc, actorNode, "tag", m_tag);
	Helpers::Serializer::SerializeBoolean(p_doc, actorNode, "active", m_active);
	Helpers::Serializer::SerializeInt64(p_doc, actorNode, "id", m_actorID);
	Helpers::Serializer::SerializeInt64(p_doc, actorNode, "parent", m_parentID);

	tinyxml2::XMLNode* componentsNode = p_doc.NewElement("components");
	actorNode->InsertEndChild(componentsNode);

	for (auto& component : m_components)
	{
		/* Current component root */
		tinyxml2::XMLNode* componentNode = p_doc.NewElement("component");
		componentsNode->InsertEndChild(componentNode);

		/* Component type */
		Helpers::Serializer::SerializeString(p_doc, componentNode, "type", typeid(*component).name());

		/* Data node (Will be passed to the component) */
		tinyxml2::XMLElement* data = p_doc.NewElement("data");
		componentNode->InsertEndChild(data);

		/* Data serialization of the component */
		component->OnSerialize(p_doc, data);
	}
}

void AmberEngine::Core::ECS::Actor::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Helpers::Serializer::DeserializeString(p_doc, p_node, "name", m_name);
	Helpers::Serializer::DeserializeString(p_doc, p_node, "tag", m_tag);
	Helpers::Serializer::DeserializeBoolean(p_doc, p_node, "active", m_active);
	Helpers::Serializer::DeserializeInt64(p_doc, p_node, "id", m_actorID);
	Helpers::Serializer::DeserializeInt64(p_doc, p_node, "parent", m_parentID);

	{
		tinyxml2::XMLNode* componentsRoot = p_node->FirstChildElement("components");
		if (componentsRoot)
		{
			tinyxml2::XMLElement* currentComponent = componentsRoot->FirstChildElement("component");

			while (currentComponent)
			{
				std::string componentType = currentComponent->FirstChildElement("type")->GetText();
				ECS::Components::AComponent* component = nullptr;

				// TODO: Use component name instead of typeid (unsafe)
				if (componentType == typeid(Components::CTransform).name())			component = &transform;
				else if (componentType == typeid(Components::CModelRenderer).name())			component = &AddComponent<ECS::Components::CModelRenderer>();
				else if (componentType == typeid(Components::CMaterialRenderer).name())		component = &AddComponent<ECS::Components::CMaterialRenderer>();
				else if (componentType == typeid(Components::CPointLight).name())			component = &AddComponent<ECS::Components::CPointLight>();
				else if (componentType == typeid(Components::CDirectionalLight).name())		component = &AddComponent<ECS::Components::CDirectionalLight>();

				if (component)
					component->OnDeserialize(p_doc, currentComponent->FirstChildElement("data"));

				currentComponent = currentComponent->NextSiblingElement("component");
			}
		}
	}
}

#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Components/CAmbientBoxLight.h"

#include "AmberEngine/Core/ECS/Actor.h"
#include "AmberEngine/Core/Helpers/Serializer.h"

#include "AmberEngine/UI/GUIDrawer.h"

AmberEngine::Core::ECS::Components::CAmbientBoxLight::CAmbientBoxLight(Actor& p_owner) : CLight(p_owner, Rendering::Settings::ELightType::AMBIENT_BOX)
{
	m_data.Type = static_cast<float>(Rendering::Settings::ELightType::AMBIENT_BOX);

	m_data.Intensity = 0.1f;
	m_data.Constant  = 1.0f;
	m_data.Linear    = 1.0f;
	m_data.Quadratic = 1.0f;
}

std::string AmberEngine::Core::ECS::Components::CAmbientBoxLight::GetName()
{
	return "Ambient Box Light";
}

glm::vec3 AmberEngine::Core::ECS::Components::CAmbientBoxLight::GetSize() const
{
	return { m_data.Constant, m_data.Linear, m_data.Quadratic };
}

void AmberEngine::Core::ECS::Components::CAmbientBoxLight::SetSize(const glm::vec3& p_size)
{
	m_data.Constant = p_size.x;
	m_data.Linear = p_size.y;
	m_data.Quadratic = p_size.z;
}

void AmberEngine::Core::ECS::Components::CAmbientBoxLight::OnSerialize(tinyxml2::XMLDocument& p_doc,
	tinyxml2::XMLNode* p_node)
{
	CLight::OnSerialize(p_doc, p_node);

	Helpers::Serializer::SerializeVec3(p_doc, p_node, "size", { m_data.Constant, m_data.Linear, m_data.Quadratic });
}

void AmberEngine::Core::ECS::Components::CAmbientBoxLight::OnDeserialize(tinyxml2::XMLDocument& p_doc,
	tinyxml2::XMLNode* p_node)
{
	CLight::OnDeserialize(p_doc, p_node);

	glm::vec3 size = Helpers::Serializer::DeserializeVec3(p_doc, p_node, "size");
	m_data.Constant = size.x;
	m_data.Linear = size.y;
	m_data.Quadratic = size.z;

	Helpers::Serializer::SerializeVec3(p_doc, p_node, "size", { m_data.Constant, m_data.Linear, m_data.Quadratic });
}

void AmberEngine::Core::ECS::Components::CAmbientBoxLight::OnInspector(UI::WidgetContainer& p_root)
{
	CLight::OnInspector(p_root);

	auto sizeGatherer = [this]() ->  glm::vec3 { return { m_data.Constant, m_data.Linear, m_data.Quadratic }; };
	auto sizeProvider = [this](const glm::vec3& p_data) { m_data.Constant = p_data.x; m_data.Linear = p_data.y; m_data.Quadratic = p_data.z; };

	UI::GUIDrawer::DrawVec3(p_root, "Size", sizeGatherer, sizeProvider, 0.1f, 0.0f);
}

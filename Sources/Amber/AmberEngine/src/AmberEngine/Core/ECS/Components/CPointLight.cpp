#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Components/CPointLight.h"

#include "AmberEngine/Core/Helpers/Serializer.h"
#include "AmberEngine/UI/GUIDrawer.h"

#include "AmberEngine/UI/Widgets/Button.h"
#include "AmberEngine/UI/Widgets/Group.h"

AmberEngine::Core::ECS::Components::CPointLight::CPointLight(Actor& p_owner) : CLight(p_owner, Rendering::Settings::ELightType::POINT)
{
}

float AmberEngine::Core::ECS::Components::CPointLight::GetConstant() const
{
	return m_data.Constant;
}

float AmberEngine::Core::ECS::Components::CPointLight::GetLinear() const
{
	return m_data.Linear;
}

float AmberEngine::Core::ECS::Components::CPointLight::GetQuadratic() const
{
	return m_data.Quadratic;
}

void AmberEngine::Core::ECS::Components::CPointLight::SetConstant(float p_constant)
{
	m_data.Constant = p_constant;
}

void AmberEngine::Core::ECS::Components::CPointLight::SetLinear(float p_linear)
{
	m_data.Linear = p_linear;
}

void AmberEngine::Core::ECS::Components::CPointLight::SetQuadratic(float p_quadratic)
{
	m_data.Quadratic = p_quadratic;
}

std::string AmberEngine::Core::ECS::Components::CPointLight::GetName()
{
	return "Point Light";
}

void AmberEngine::Core::ECS::Components::CPointLight::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	CLight::OnSerialize(p_doc, p_node);

	Helpers::Serializer::SerializeFloat(p_doc, p_node, "constant", m_data.Constant);
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "linear", m_data.Linear);
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "quadratic", m_data.Quadratic);
}

void AmberEngine::Core::ECS::Components::CPointLight::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	CLight::OnDeserialize(p_doc, p_node);

	Helpers::Serializer::DeserializeFloat(p_doc, p_node, "constant", m_data.Constant);
	Helpers::Serializer::DeserializeFloat(p_doc, p_node, "linear", m_data.Linear);
	Helpers::Serializer::DeserializeFloat(p_doc, p_node, "quadratic", m_data.Quadratic);
}

void AmberEngine::Core::ECS::Components::CPointLight::OnInspector(UI::WidgetContainer& p_root)
{
	CLight::OnInspector(p_root);

	UI::GUIDrawer::CreateTitle(p_root, "Fall-off presets");
	auto& presetsRoot = p_root.CreateWidget<UI::Widgets::Group>();

	auto& constantPreset = presetsRoot.CreateWidget<UI::Widgets::Button>("Constant");
	constantPreset.ClickedEvent += [this] { m_data.Constant = 1.0f; m_data.Linear = m_data.Quadratic = 0.0f; };
	constantPreset.lineBreak = false;
	constantPreset.idleBackgroundColor = { 0.7f, 0.5f, 0.0f };

	auto& linearPreset = presetsRoot.CreateWidget<UI::Widgets::Button>("Linear");
	linearPreset.ClickedEvent += [this] { m_data.Linear = 1.0f; m_data.Constant = m_data.Quadratic = 0.0f; };
	linearPreset.lineBreak = false;
	linearPreset.idleBackgroundColor = { 0.7f, 0.5f, 0.0f };

	auto& quadraticPreset = presetsRoot.CreateWidget<UI::Widgets::Button>("Quadratic");
	quadraticPreset.ClickedEvent += [this] { m_data.Quadratic = 1.0f; m_data.Constant = m_data.Linear = 0.0f; };
	quadraticPreset.idleBackgroundColor = { 0.7f, 0.5f, 0.0f };

	UI::GUIDrawer::DrawScalar<float>(p_root, "Constant", m_data.Constant, 0.005f, 0.0f);
	UI::GUIDrawer::DrawScalar<float>(p_root, "Linear", m_data.Linear, 0.005f, 0.0f);
	UI::GUIDrawer::DrawScalar<float>(p_root, "Quadratic", m_data.Quadratic, 0.005f, 0.0f);
}

#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Components/CSpotLight.h"

#include "AmberEngine/Core/Helpers/Serializer.h"

#include "AmberEngine/UI/GUIDrawer.h"
#include "AmberEngine/UI/Widgets/Button.h"
#include "AmberEngine/UI/Widgets/Group.h"

AmberEngine::Core::ECS::Components::CSpotLight::CSpotLight(Actor& p_owner) : CLight(p_owner, Rendering::Settings::ELightType::SPOT)
{
	m_data.Type = static_cast<float>(Rendering::Settings::ELightType::SPOT);
}

float AmberEngine::Core::ECS::Components::CSpotLight::GetConstant() const
{
	return m_data.Constant;
}

float AmberEngine::Core::ECS::Components::CSpotLight::GetLinear() const
{
	return m_data.Linear;
}

float AmberEngine::Core::ECS::Components::CSpotLight::GetQuadratic() const
{
	return m_data.Quadratic;
}

float AmberEngine::Core::ECS::Components::CSpotLight::GetCutoff() const
{
	return m_data.Cutoff;
}

float AmberEngine::Core::ECS::Components::CSpotLight::GetOuterCutoff() const
{
	return m_data.OuterCutoff;
}

void AmberEngine::Core::ECS::Components::CSpotLight::SetConstant(float p_constant)
{
	m_data.Constant = p_constant;
}

void AmberEngine::Core::ECS::Components::CSpotLight::SetLinear(float p_linear)
{
	m_data.Linear = p_linear;
}

void AmberEngine::Core::ECS::Components::CSpotLight::SetQuadratic(float p_quadratic)
{
	m_data.Quadratic = p_quadratic;
}

void AmberEngine::Core::ECS::Components::CSpotLight::SetCutoff(float p_cutoff)
{
	m_data.Cutoff = p_cutoff;
}

void AmberEngine::Core::ECS::Components::CSpotLight::SetOuterCutoff(float p_outerCutoff)
{
	m_data.OuterCutoff = p_outerCutoff;
}

std::string AmberEngine::Core::ECS::Components::CSpotLight::GetName()
{
	return "Spot Light";
}

void AmberEngine::Core::ECS::Components::CSpotLight::OnSerialize(tinyxml2::XMLDocument& p_doc,
	tinyxml2::XMLNode* p_node)
{
	CLight::OnSerialize(p_doc, p_node);

	Helpers::Serializer::SerializeFloat(p_doc, p_node, "constant", m_data.Constant);
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "linear", m_data.Linear);
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "quadratic", m_data.Quadratic);
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "cutoff", m_data.Cutoff);
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "outercutoff", m_data.OuterCutoff);
}

void AmberEngine::Core::ECS::Components::CSpotLight::OnDeserialize(tinyxml2::XMLDocument& p_doc,
	tinyxml2::XMLNode* p_node)
{
	CLight::OnDeserialize(p_doc, p_node);

	Helpers::Serializer::DeserializeFloat(p_doc, p_node, "constant", m_data.Constant);
	Helpers::Serializer::DeserializeFloat(p_doc, p_node, "linear", m_data.Linear);
	Helpers::Serializer::DeserializeFloat(p_doc, p_node, "quadratic", m_data.Quadratic);
	Helpers::Serializer::DeserializeFloat(p_doc, p_node, "cutoff", m_data.Cutoff);
	Helpers::Serializer::DeserializeFloat(p_doc, p_node, "outercutoff", m_data.OuterCutoff);
}

void AmberEngine::Core::ECS::Components::CSpotLight::OnInspector(UI::WidgetContainer& p_root)
{
	CLight::OnInspector(p_root);

	UI::GUIDrawer::DrawScalar<float>(p_root, "Cut-Off", m_data.Cutoff, 0.1f, 0.f, 180.f);
	UI::GUIDrawer::DrawScalar<float>(p_root, "Outer Cut-Off", m_data.OuterCutoff, 0.1f, 0.f, 180.f);

	UI::GUIDrawer::CreateTitle(p_root, "Fall-off presets");
	auto& presetsRoot = p_root.CreateWidget<UI::Widgets::Group>();

	auto& constantPreset = presetsRoot.CreateWidget<UI::Widgets::Button>("Constant");
	constantPreset.ClickedEvent += [this] { m_data.Constant = 1.f, m_data.Linear = m_data.Quadratic = 0.f; };
	constantPreset.lineBreak = false;
	constantPreset.idleBackgroundColor = { 0.7f, 0.5f, 0.f };

	auto& linearPreset = presetsRoot.CreateWidget<UI::Widgets::Button>("Linear");
	linearPreset.ClickedEvent += [this] { m_data.Linear = 1.f, m_data.Constant = m_data.Quadratic = 0.f; };
	linearPreset.lineBreak = false;
	linearPreset.idleBackgroundColor = { 0.7f, 0.5f, 0.f };

	auto& quadraticPreset = presetsRoot.CreateWidget<UI::Widgets::Button>("Quadratic");
	quadraticPreset.ClickedEvent += [this] { m_data.Quadratic = 1.f, m_data.Constant = m_data.Linear = 0.f; };
	quadraticPreset.idleBackgroundColor = { 0.7f, 0.5f, 0.f };

	UI::GUIDrawer::DrawScalar<float>(p_root, "Constant", m_data.Constant, 0.005f, 0.f);
	UI::GUIDrawer::DrawScalar<float>(p_root, "Linear", m_data.Linear, 0.005f, 0.f);
	UI::GUIDrawer::DrawScalar<float>(p_root, "Quadratic", m_data.Quadratic, 0.005f, 0.f);
}

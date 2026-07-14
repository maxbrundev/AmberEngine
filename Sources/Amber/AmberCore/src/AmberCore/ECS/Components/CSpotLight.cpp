#include "Amberpch.h"

#include "AmberCore/ECS/Components/CSpotLight.h"

#include "AmberCore/Helpers/Serializer.h"

#include "AmberCore/Helpers/GUIDrawer.h"
#include "AmberUI/Widgets/Button.h"
#include "AmberUI/Widgets/Group.h"

AmberCore::ECS::Components::CSpotLight::CSpotLight(Actor& p_owner) : CLight(p_owner, AmberRendering::Settings::ELightType::SPOT)
{
	m_data.Type = static_cast<float>(AmberRendering::Settings::ELightType::SPOT);
}

float AmberCore::ECS::Components::CSpotLight::GetConstant() const
{
	return m_data.Constant;
}

float AmberCore::ECS::Components::CSpotLight::GetLinear() const
{
	return m_data.Linear;
}

float AmberCore::ECS::Components::CSpotLight::GetQuadratic() const
{
	return m_data.Quadratic;
}

float AmberCore::ECS::Components::CSpotLight::GetCutoff() const
{
	return m_data.Cutoff;
}

float AmberCore::ECS::Components::CSpotLight::GetOuterCutoff() const
{
	return m_data.OuterCutoff;
}

void AmberCore::ECS::Components::CSpotLight::SetConstant(float p_constant)
{
	m_data.Constant = p_constant;
}

void AmberCore::ECS::Components::CSpotLight::SetLinear(float p_linear)
{
	m_data.Linear = p_linear;
}

void AmberCore::ECS::Components::CSpotLight::SetQuadratic(float p_quadratic)
{
	m_data.Quadratic = p_quadratic;
}

void AmberCore::ECS::Components::CSpotLight::SetCutoff(float p_cutoff)
{
	m_data.Cutoff = p_cutoff;
}

void AmberCore::ECS::Components::CSpotLight::SetOuterCutoff(float p_outerCutoff)
{
	m_data.OuterCutoff = p_outerCutoff;
}

std::string AmberCore::ECS::Components::CSpotLight::GetName()
{
	return "Spot Light";
}

void AmberCore::ECS::Components::CSpotLight::OnSerialize(tinyxml2::XMLDocument& p_doc,
	tinyxml2::XMLNode* p_node)
{
	CLight::OnSerialize(p_doc, p_node);

	Helpers::Serializer::SerializeFloat(p_doc, p_node, "constant", m_data.Constant);
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "linear", m_data.Linear);
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "quadratic", m_data.Quadratic);
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "cutoff", m_data.Cutoff);
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "outercutoff", m_data.OuterCutoff);
}

void AmberCore::ECS::Components::CSpotLight::OnDeserialize(tinyxml2::XMLDocument& p_doc,
	tinyxml2::XMLNode* p_node)
{
	CLight::OnDeserialize(p_doc, p_node);

	Helpers::Serializer::DeserializeFloat(p_doc, p_node, "constant", m_data.Constant);
	Helpers::Serializer::DeserializeFloat(p_doc, p_node, "linear", m_data.Linear);
	Helpers::Serializer::DeserializeFloat(p_doc, p_node, "quadratic", m_data.Quadratic);
	Helpers::Serializer::DeserializeFloat(p_doc, p_node, "cutoff", m_data.Cutoff);
	Helpers::Serializer::DeserializeFloat(p_doc, p_node, "outercutoff", m_data.OuterCutoff);
}

void AmberCore::ECS::Components::CSpotLight::OnInspector(AmberUI::WidgetContainer& p_root)
{
	CLight::OnInspector(p_root);

	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Cut-Off", m_data.Cutoff, 0.1f, 0.f, 180.f);
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Outer Cut-Off", m_data.OuterCutoff, 0.1f, 0.f, 180.f);

	AmberCore::Helpers::GUIDrawer::CreateTitle(p_root, "Fall-off presets");
	auto& presetsRoot = p_root.CreateWidget<AmberUI::Widgets::Group>();

	auto& constantPreset = presetsRoot.CreateWidget<AmberUI::Widgets::Button>("Constant");
	constantPreset.ClickedEvent += [this] { m_data.Constant = 1.f, m_data.Linear = m_data.Quadratic = 0.f; };
	constantPreset.LineBreak = false;
	constantPreset.IdleBackgroundColor = { 0.7f, 0.5f, 0.f };

	auto& linearPreset = presetsRoot.CreateWidget<AmberUI::Widgets::Button>("Linear");
	linearPreset.ClickedEvent += [this] { m_data.Linear = 1.f, m_data.Constant = m_data.Quadratic = 0.f; };
	linearPreset.LineBreak = false;
	linearPreset.IdleBackgroundColor = { 0.7f, 0.5f, 0.f };

	auto& quadraticPreset = presetsRoot.CreateWidget<AmberUI::Widgets::Button>("Quadratic");
	quadraticPreset.ClickedEvent += [this] { m_data.Quadratic = 1.f, m_data.Constant = m_data.Linear = 0.f; };
	quadraticPreset.IdleBackgroundColor = { 0.7f, 0.5f, 0.f };

	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Constant", m_data.Constant, 0.005f, 0.f);
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Linear", m_data.Linear, 0.005f, 0.f);
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Quadratic", m_data.Quadratic, 0.005f, 0.f);
}

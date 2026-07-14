#include "Amberpch.h"

#include "AmberCore/ECS/Components/CPointLight.h"

#include "AmberCore/Helpers/Serializer.h"
#include "AmberCore/Helpers/GUIDrawer.h"

#include "AmberUI/Widgets/Button.h"
#include "AmberUI/Widgets/Group.h"
#include "AmberCore/ECS/Actor.h"

AmberCore::ECS::Components::CPointLight::CPointLight(Actor& p_owner) : CLight(p_owner, AmberRendering::Settings::ELightType::POINT)
{
}

float AmberCore::ECS::Components::CPointLight::GetConstant() const
{
	return m_data.Constant;
}

float AmberCore::ECS::Components::CPointLight::GetLinear() const
{
	return m_data.Linear;
}

float AmberCore::ECS::Components::CPointLight::GetQuadratic() const
{
	return m_data.Quadratic;
}

void AmberCore::ECS::Components::CPointLight::SetConstant(float p_constant)
{
	m_data.Constant = p_constant;
}

void AmberCore::ECS::Components::CPointLight::SetLinear(float p_linear)
{
	m_data.Linear = p_linear;
}

void AmberCore::ECS::Components::CPointLight::SetQuadratic(float p_quadratic)
{
	m_data.Quadratic = p_quadratic;
}

std::string AmberCore::ECS::Components::CPointLight::GetName()
{
	return "Point Light";
}

void AmberCore::ECS::Components::CPointLight::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	CLight::OnSerialize(p_doc, p_node);

	Helpers::Serializer::SerializeFloat(p_doc, p_node, "constant", m_data.Constant);
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "linear", m_data.Linear);
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "quadratic", m_data.Quadratic);
}

void AmberCore::ECS::Components::CPointLight::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	CLight::OnDeserialize(p_doc, p_node);

	Helpers::Serializer::DeserializeFloat(p_doc, p_node, "constant", m_data.Constant);
	Helpers::Serializer::DeserializeFloat(p_doc, p_node, "linear", m_data.Linear);
	Helpers::Serializer::DeserializeFloat(p_doc, p_node, "quadratic", m_data.Quadratic);
}

void AmberCore::ECS::Components::CPointLight::OnInspector(AmberUI::WidgetContainer& p_root)
{
	CLight::OnInspector(p_root);

	AmberCore::Helpers::GUIDrawer::CreateTitle(p_root, "Fall-off presets");
	auto& presetsRoot = p_root.CreateWidget<AmberUI::Widgets::Group>();

	auto& constantPreset = presetsRoot.CreateWidget<AmberUI::Widgets::Button>("Constant");
	constantPreset.ClickedEvent += [this] { m_data.Constant = 1.0f; m_data.Linear = m_data.Quadratic = 0.0f; };
	constantPreset.LineBreak = false;
	constantPreset.IdleBackgroundColor = { 0.7f, 0.5f, 0.0f };

	auto& linearPreset = presetsRoot.CreateWidget<AmberUI::Widgets::Button>("Linear");
	linearPreset.ClickedEvent += [this] { m_data.Linear = 1.0f; m_data.Constant = m_data.Quadratic = 0.0f; };
	linearPreset.LineBreak = false;
	linearPreset.IdleBackgroundColor = { 0.7f, 0.5f, 0.0f };

	auto& quadraticPreset = presetsRoot.CreateWidget<AmberUI::Widgets::Button>("Quadratic");
	quadraticPreset.ClickedEvent += [this] { m_data.Quadratic = 1.0f; m_data.Constant = m_data.Linear = 0.0f; };
	quadraticPreset.IdleBackgroundColor = { 0.7f, 0.5f, 0.0f };

	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Constant", m_data.Constant, 0.005f, 0.0f);
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Linear", m_data.Linear, 0.005f, 0.0f);
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Quadratic", m_data.Quadratic, 0.005f, 0.0f);
}

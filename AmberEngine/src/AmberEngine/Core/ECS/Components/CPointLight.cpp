#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Components/CPointLight.h"

#include "AmberEngine/UI/GUIDrawer.h"

#include "AmberEngine/UI/Widgets/Button.h"
#include "AmberEngine/UI/Widgets/Group.h"

AmberEngine::Core::ECS::Components::CPointLight::CPointLight(Actor& p_owner) : CLight(p_owner, Rendering::Entities::ELightType::POINT)
{
}

float AmberEngine::Core::ECS::Components::CPointLight::GetConstant() const
{
	return m_data.constant;
}

float AmberEngine::Core::ECS::Components::CPointLight::GetLinear() const
{
	return m_data.linear;
}

float AmberEngine::Core::ECS::Components::CPointLight::GetQuadratic() const
{
	return m_data.quadratic;
}

void AmberEngine::Core::ECS::Components::CPointLight::SetConstant(float p_constant)
{
	m_data.constant = p_constant;
}

void AmberEngine::Core::ECS::Components::CPointLight::SetLinear(float p_linear)
{
	m_data.linear = p_linear;
}

void AmberEngine::Core::ECS::Components::CPointLight::SetQuadratic(float p_quadratic)
{
	m_data.quadratic = p_quadratic;
}

std::string AmberEngine::Core::ECS::Components::CPointLight::GetName()
{
	return "Point Light";
}

void AmberEngine::Core::ECS::Components::CPointLight::OnInspector(UI::WidgetContainer& p_root)
{
	CLight::OnInspector(p_root);

	UI::GUIDrawer::CreateTitle(p_root, "Fall-off presets");
	auto& presetsRoot = p_root.CreateWidget<UI::Widgets::Group>();

	auto& constantPreset = presetsRoot.CreateWidget<UI::Widgets::Button>("Constant");
	constantPreset.ClickedEvent += [this] { m_data.constant = 1.0f; m_data.linear = m_data.quadratic = 0.0f; };
	constantPreset.lineBreak = false;
	constantPreset.idleBackgroundColor = { 0.7f, 0.5f, 0.0f };

	auto& linearPreset = presetsRoot.CreateWidget<UI::Widgets::Button>("Linear");
	linearPreset.ClickedEvent += [this] { m_data.linear = 1.0f; m_data.constant = m_data.quadratic = 0.0f; };
	linearPreset.lineBreak = false;
	linearPreset.idleBackgroundColor = { 0.7f, 0.5f, 0.0f };

	auto& quadraticPreset = presetsRoot.CreateWidget<UI::Widgets::Button>("Quadratic");
	quadraticPreset.ClickedEvent += [this] { m_data.quadratic = 1.0f; m_data.constant = m_data.linear = 0.0f; };
	quadraticPreset.idleBackgroundColor = { 0.7f, 0.5f, 0.0f };

	UI::GUIDrawer::DrawScalar<float>(p_root, "Constant", m_data.constant, 0.005f, 0.0f);
	UI::GUIDrawer::DrawScalar<float>(p_root, "Linear", m_data.linear, 0.005f, 0.0f);
	UI::GUIDrawer::DrawScalar<float>(p_root, "Quadratic", m_data.quadratic, 0.005f, 0.0f);
}

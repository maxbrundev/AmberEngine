#include "Amberpch.h"

#ifdef _DEBUG

#include "AmberGame/Debug/DriverInfo.h"

#include "AmberUI/Widgets/TextColored.h"

AmberGame::Debug::DriverInfo::DriverInfo(AmberRendering::Context::Driver& p_driver, AmberWindowing::Context::Window& p_window)
{
	m_defaultHorizontalAlignment = AmberUI::EHorizontalAlignment::RIGHT;
	m_defaultVerticalAlignment = AmberUI::EVerticalAlignment::BOTTOM;
	m_defaultPosition.x = static_cast<float>(p_window.GetSize().first) - 10.f;
	m_defaultPosition.y = static_cast<float>(p_window.GetSize().second) - 10.f;

	CreateWidget<AmberUI::Widgets::TextColored>("Vendor: " + p_driver.GetString(GL_VENDOR), AmberRendering::Data::Color::Yellow);
	CreateWidget<AmberUI::Widgets::TextColored>("Hardware: " + p_driver.GetString(GL_RENDERER), AmberRendering::Data::Color::Yellow);
	CreateWidget<AmberUI::Widgets::TextColored>("OpenGL Version: " + p_driver.GetString(GL_VERSION), AmberRendering::Data::Color::Yellow);
	CreateWidget<AmberUI::Widgets::TextColored>("GLSL Version: " + p_driver.GetString(GL_SHADING_LANGUAGE_VERSION), AmberRendering::Data::Color::Yellow);
}

#endif

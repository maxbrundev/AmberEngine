#include "Amberpch.h"

#include "AmberEngine/UI/Panels/DriverInfoPanel.h"

#include "AmberEngine/Core/Context.h"
#include "AmberEngine/Tools/Global/ServiceLocator.h"
#include "AmberEngine/UI/Widgets/Text.h"

AmberEngine::UI::Panels::DriverInfoPanel::DriverInfoPanel(const std::string& p_title, bool p_opened, PanelSettings p_panelSettings) :
APanelWindow(p_title, p_opened, p_panelSettings)
{
	auto& driver = *Tools::Global::ServiceLocator::Get<AmberEngine::Core::Context>().driver.get();

	CreateWidget<Widgets::Text>("Vendor: " + driver.GetString(GL_VENDOR));
	CreateWidget<Widgets::Text>("Hardware: " + driver.GetString(GL_RENDERER));
	CreateWidget<Widgets::Text>("OpenGL Version: " + driver.GetString(GL_VERSION));
	CreateWidget<Widgets::Text>("GLSL Version: " + driver.GetString(GL_SHADING_LANGUAGE_VERSION));
}

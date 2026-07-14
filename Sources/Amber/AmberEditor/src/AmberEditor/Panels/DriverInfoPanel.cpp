#include "Amberpch.h"

#include "AmberEditor/Panels/DriverInfoPanel.h"

#include "AmberEditor/Core/Context.h"
#include "AmberTools/Global/ServiceLocator.h"
#include "AmberUI/Widgets/Text.h"

AmberEditor::Panels::DriverInfoPanel::DriverInfoPanel(const std::string& p_title, bool p_opened, AmberUI::Panels::PanelSettings p_panelSettings) :
AmberUI::Panels::APanelWindow(p_title, p_opened, p_panelSettings)
{
	auto& driver = *AmberTools::Global::ServiceLocator::Get<AmberEditor::Core::Context>().driver.get();

	CreateWidget<AmberUI::Widgets::Text>("Vendor: " + driver.GetString(GL_VENDOR));
	CreateWidget<AmberUI::Widgets::Text>("Hardware: " + driver.GetString(GL_RENDERER));
	CreateWidget<AmberUI::Widgets::Text>("OpenGL Version: " + driver.GetString(GL_VERSION));
	CreateWidget<AmberUI::Widgets::Text>("GLSL Version: " + driver.GetString(GL_SHADING_LANGUAGE_VERSION));
}

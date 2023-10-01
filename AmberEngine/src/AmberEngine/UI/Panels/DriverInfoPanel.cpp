#include "Amberpch.h"

#include "AmberEngine/UI/DriverInfoPanel.h"

#include "AmberEngine/Core/Context.h"
#include "AmberEngine/Tools/Utils/ServiceLocator.h"

DriverInfoPanel::DriverInfoPanel(const std::string& p_title, bool p_opened, PanelSettings p_panelSettings) :
APanelWindow(p_title, p_opened, p_panelSettings)
{
	const auto& driver = AmberEngine::Utils::ServiceLocator::Get<AmberEngine::Core::Context>().driver;

	const std::string_view glewVersion            = "GLEW Version: ";
	const std::string_view glImplementation       = "GL implementation: ";
	const std::string_view renderer               = "GPU: ";
	const std::string_view version                = "Version: ";
	const std::string_view shadingLanguageVersion = "Shading Language Version: ";

	m_info.reserve(driver->m_library_version().size() 
		+ driver->m_vendor1().size() 
		+ driver->m_render1().size()
		+ driver->m_api_version().size() 
		+ driver->m_shading_language_version1().size() 
		+ glewVersion.size() 
		+ glImplementation.size() 
		+ renderer.size() 
		+ version.size() 
		+ shadingLanguageVersion.size() 
		+ 4);

	m_info.append(glewVersion);
	m_info.append(driver->m_library_version());
	m_info.append("\n");
	m_info.append(glImplementation);
	m_info.append(driver->m_vendor1());
	m_info.append("\n");
	m_info.append(renderer);
	m_info.append(driver->m_render1());
	m_info.append("\n");
	m_info.append(version);
	m_info.append(driver->m_api_version());
	m_info.append("\n");
	m_info.append(shadingLanguageVersion);
	m_info.append(driver->m_shading_language_version1());
}

void DriverInfoPanel::DrawContent()
{
	ImGui::Text(m_info.c_str());
}

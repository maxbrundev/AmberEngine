#include "Amberpch.h"

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Core/Context.h"

AmberEngine::Core::Context::Context(const AmberEngine::Settings::DeviceSettings& p_deviceSettings, const AmberEngine::Settings::WindowSettings& p_windowSettings, const AmberEngine::Settings::DriverSettings& p_driverSettings)
{
	m_device = std::make_unique<AmberEngine::Context::Device>(p_deviceSettings);
	m_window = std::make_unique<AmberEngine::Context::Window>(*m_device, p_windowSettings);
	m_driver = std::make_unique<AmberEngine::Context::Driver>(p_driverSettings);
	m_renderer = std::make_unique<AmberEngine::Core::Renderer>(*m_driver);
	m_uiManager = std::make_unique<AmberEngine::Manager::UIManager>(m_window->GetGlfwWindow());
	m_uiManager->EnableDocking(true);
	m_inputManager = std::make_unique<AmberEngine::Inputs::InputManager>(*m_window);
}

AmberEngine::Core::Context::~Context()
{
	
}

#include "AmberEngine/Managers/WindowManager.h"

AmberEngine::Managers::WindowManager::WindowManager(const Settings::DeviceSettings& p_deviceSettings, const Settings::WindowSettings& p_windowSettings, const Settings::DriverSettings& p_driverSettings)
: m_device(p_deviceSettings), m_window(m_device, p_windowSettings), m_driver(p_driverSettings)
{
}

AmberEngine::Managers::WindowManager::~WindowManager()
{
	m_window.Close();
}

bool AmberEngine::Managers::WindowManager::IsOpen()
{
	return m_window.IsActive();
}

AmberEngine::Context::Device& AmberEngine::Managers::WindowManager::GetDevice()
{
	return m_device;
}

AmberEngine::Context::Window& AmberEngine::Managers::WindowManager::GetWindow()
{
	return m_window;
}

AmberEngine::Context::Driver& AmberEngine::Managers::WindowManager::GetDriver()
{
	return m_driver;
}
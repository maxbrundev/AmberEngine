#include "AmberEngine/Managers/WindowManager.h"

AmberEngine::Managers::WindowManager::WindowManager(const Core::DeviceSettings& p_deviceSettings, const Core::DriverSettings& p_driverSettings) : m_device(p_deviceSettings), m_driver(p_driverSettings)
{
}

AmberEngine::Managers::WindowManager::~WindowManager()
{
	m_device.Close();
}

bool AmberEngine::Managers::WindowManager::IsOpen()
{
	return m_device.IsActive();
}

AmberEngine::Core::Device& AmberEngine::Managers::WindowManager::GetDevice()
{
	return m_device;
}

AmberEngine::Core::Driver& AmberEngine::Managers::WindowManager::GetDriver()
{
	return m_driver;
}
#include "AmberEngine/Managers/WindowManager.h"

AmberEngine::Managers::WindowManager::WindowManager()
{
	m_device = std::make_unique<Core::Device>();
	m_driver = std::make_unique<Core::Driver>();

	Setup();
}

AmberEngine::Managers::WindowManager::~WindowManager()
{
	m_device->Close();
}

void AmberEngine::Managers::WindowManager::Setup()
{
	m_device->Setup();
	m_driver->Setup();
}

bool AmberEngine::Managers::WindowManager::IsOpen() const
{
	return m_device->IsActive();
}

AmberEngine::Core::Device& AmberEngine::Managers::WindowManager::GetDevice() const
{
	return *m_device;
}

AmberEngine::Core::Driver& AmberEngine::Managers::WindowManager::GetDriver() const
{
	return *m_driver;
}
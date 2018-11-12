#include "pch.h"

#include "Managers/WindowManager.h"

RenderEngine::Managers::WindowManager::WindowManager()
{
	m_device = std::make_unique<Core::Device>();
	m_driver = std::make_unique<Core::Driver>();

	Setup();
}

RenderEngine::Managers::WindowManager::~WindowManager()
{
	m_device->Close();
}

void RenderEngine::Managers::WindowManager::Setup()
{
	m_device->Setup();
	m_driver->Setup();
}

void RenderEngine::Managers::WindowManager::Update()
{
	m_device->Update();
}

bool RenderEngine::Managers::WindowManager::IsOpen() const
{
	return m_device->IsActive();
}

RenderEngine::Core::Device& RenderEngine::Managers::WindowManager::GetDevice() const
{
	return *m_device;
}

RenderEngine::Core::Driver& RenderEngine::Managers::WindowManager::GetDriver() const
{
	return *m_driver;
}
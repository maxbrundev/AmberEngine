#include "Amberpch.h"

#include "AmberEngine/Core/Application.h"

AmberEngine::Core::Application::Application(const AmberEngine::Settings::DeviceSettings& p_deviceSettings,
	const AmberEngine::Settings::WindowSettings& p_windowSettings,
	const AmberEngine::Settings::DriverSettings& p_driverSettings) :
m_context(p_deviceSettings, p_windowSettings, p_driverSettings),
m_editor(m_context)
{
	m_layerManager = std::make_unique<Managers::LayerManager>();
	isRunning = true;
}

AmberEngine::Core::Application::~Application()
{
	m_layerManager->DestroyLayers();
}

void AmberEngine::Core::Application::Run()
{
	while (IsRunning())
	{
		UpdateDeltaTime();
		m_editor.PreUpdate();
		m_editor.Update(m_deltaTime);
		m_layerManager->UpdateLayers(m_deltaTime);
		m_editor.PostUpdate();
		
	}
}

bool AmberEngine::Core::Application::PushLayer(ALayer* p_layer)
{
	return m_layerManager->AddLayer(p_layer);
}

bool AmberEngine::Core::Application::IsRunning()
{
	return isRunning && m_context.m_window->IsActive();
}

void AmberEngine::Core::Application::UpdateDeltaTime()
{
	float currentTime = glfwGetTime();
	m_deltaTime = currentTime - m_lastTime;
	m_lastTime = currentTime;
}

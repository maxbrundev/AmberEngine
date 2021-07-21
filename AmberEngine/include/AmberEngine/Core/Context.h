#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Context/Driver.h"
#include "AmberEngine/Context/Device.h"
#include "AmberEngine/Context/Window.h"

#include "AmberEngine/Inputs/InputManager.h"

#include "AmberEngine/Core/Renderer.h"

#include "AmberEngine/Managers/ResourcesManager.h"

namespace AmberEngine::Core
{
	class API_AMBERENGINE Context
	{
	public:
		Context(const AmberEngine::Settings::DeviceSettings& p_deviceSettings, const AmberEngine::Settings::WindowSettings& p_windowSettings, const AmberEngine::Settings::DriverSettings& p_driverSettings);
		~Context();
	
	public:
		std::unique_ptr<AmberEngine::Context::Device> m_device;
		std::unique_ptr<AmberEngine::Context::Window> m_window;
		std::unique_ptr<AmberEngine::Context::Driver> m_driver;
		
		std::unique_ptr<Inputs::InputManager> m_inputManager;
		
		std::unique_ptr<Renderer> m_renderer;
		
		Managers::ResourcesManager m_resourcesManager;
	};
}

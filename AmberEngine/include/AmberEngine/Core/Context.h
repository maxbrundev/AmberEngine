#pragma once

#include "AmberEngine/API/Export.h"


#include "AmberEngine/Context/Driver.h"
#include "AmberEngine/Context/Device.h"
#include "AmberEngine/Context/Window.h"

#include "AmberEngine/Inputs/InputManager.h"

#include "AmberEngine/Core/Renderer.h"
#include "AmberEngine/Core/EditorResources.h"
#include "AmberEngine/Core/UIManager.h"

#include "AmberEngine/Buffers/UniformBuffer.h"

#include "AmberEngine/Core/SceneSystem/Scene.h"

namespace AmberEngine::Core
{
	class API_AMBERENGINE Context
	{
	public:
		Context(const Settings::DeviceSettings& p_deviceSettings, const Settings::WindowSettings& p_windowSettings, const Settings::DriverSettings& p_driverSettings);
		~Context();
	
	public:
		const std::string editorAssetsPath;

		std::unique_ptr<AmberEngine::Context::Device>	device;
		std::unique_ptr<AmberEngine::Context::Window>	window;
		std::unique_ptr<AmberEngine::Context::Driver>	driver;
		std::unique_ptr<Inputs::InputManager>			inputManager;
		std::unique_ptr<Renderer>						renderer;
		std::unique_ptr<UIManager>						uiManager;

		std::unique_ptr<Buffers::UniformBuffer>			engineUBO;
		std::unique_ptr<Core::EditorResources>			m_editorResources;

		SceneSystem::Scene m_scene;
	};
}

#pragma once


#include "AmberEngine/API/Export.h"

#include "AmberEngine/Context/Driver.h"
#include "AmberEngine/Context/Device.h"
#include "AmberEngine/Context/Window.h"

#include "AmberEngine/Inputs/InputManager.h"

#include "AmberEngine/Core/ECSRenderer.h"
#include "AmberEngine/Core/EditorResources.h"

#include "AmberEngine/UI/Core/UIManager.h"

#include "AmberEngine/Buffers/UniformBuffer.h"
#include "AmberEngine/Buffers/ShaderStorageBuffer.h"

#include "AmberEngine/Managers/ModelManager.h"
#include "AmberEngine/Managers/ShaderManager.h"
#include "AmberEngine/Managers/TextureManager.h"

#include "SceneSystem/SceneManager.h"

namespace AmberEngine::Core
{
	class API_AMBERENGINE Context final
	{
	public:
		Context(const std::string& p_projectPath, const Settings::DeviceSettings& p_deviceSettings, const Settings::WindowSettings& p_windowSettings, const Settings::DriverSettings& p_driverSettings);
		~Context();

		Context(const Context& other)            = delete;
		Context(Context&& other)                 = delete;
		Context& operator=(const Context& other) = delete;
		Context& operator=(Context&& other)      = delete;
	
	public:
		const std::string engineAssetsPath;
		const std::string editorAssetsPath;
		const std::string projectAssetsPath;

		std::unique_ptr<AmberEngine::Context::Device> device;
		std::unique_ptr<AmberEngine::Context::Window> window;
		std::unique_ptr<AmberEngine::Context::Driver> driver;
		std::unique_ptr<Inputs::InputManager>         inputManager;
		std::unique_ptr<ECSRenderer>                  renderer;
		std::unique_ptr<UI::Core::UIManager>          uiManager;
		std::unique_ptr<Buffers::UniformBuffer>       engineUBO;
		std::unique_ptr<Buffers::ShaderStorageBuffer> lightSSBO;
		std::unique_ptr<EditorResources>              editorResources;

		ResourceManagement::ModelManager modelManager;
		ResourceManagement::TextureManager textureManager;
		ResourceManagement::ShaderManager shaderManager;

		SceneSystem::SceneManager sceneManager;
	};
}

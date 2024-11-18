#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Context/Driver.h"
#include "AmberEngine/Context/Device.h"
#include "AmberEngine/Context/Window.h"

#include "AmberEngine/Inputs/InputManager.h"

#include "AmberEngine/Core/ECSRenderer.h"
#include "AmberEngine/Core/EditorResources.h"
#include "AmberEngine/Core/ShapeDrawer.h"
#include "AmberEngine/UI/Core/UIManager.h"
#include "AmberEngine/Core/SceneSystem/SceneManager.h"

#include "AmberEngine/Buffers/ShaderStorageBuffer.h"
#include "AmberEngine/Buffers/UniformBuffer.h"

#include "AmberEngine/Managers/MaterialManager.h"
#include "AmberEngine/Managers/ModelManager.h"
#include "AmberEngine/Managers/ShaderManager.h"
#include "AmberEngine/Managers/TextureManager.h"

#include "AmberEngine/Physics/Core/PhysicsEngine.h"

#include "AmberEngine/Tools/FileSystem/IniFile.h"

namespace AmberEngine::Core
{
	class API_AMBERENGINE Context final
	{
	public:
		Context(const std::string& p_projectPath, const std::string& p_projectName);
		~Context();

		Context(const Context& other)            = delete;
		Context(Context&& other)                 = delete;
		Context& operator=(const Context& other) = delete;
		Context& operator=(Context&& other)      = delete;

		void ResetProjectSettings();
		bool IsProjectSettingsIntegrityVerified();
		void ApplyProjectSettings();
	
	public:
		const std::string projectPath;
		const std::string projectName;
		const std::string projectFilePath;
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
		std::unique_ptr<Buffers::ShaderStorageBuffer> simulatedLightSSBO;
		std::unique_ptr<EditorResources>              editorResources;
		std::unique_ptr<ShapeDrawer>                  shapeDrawer;
		std::unique_ptr<Physics::Core::PhysicsEngine> physicsEngine;

		ResourceManagement::ModelManager modelManager;
		ResourceManagement::TextureManager textureManager;
		ResourceManagement::ShaderManager shaderManager;
		ResourceManagement::MaterialManager	materialManager;

		SceneSystem::SceneManager sceneManager;

		Tools::Filesystem::IniFile projectSettings;

		Maths::Transform simulatedLightTransform;
	};
}

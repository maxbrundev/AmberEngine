#pragma once


#include "AmberRendering/Context/Driver.h"
#include "AmberWindowing/Context/Device.h"
#include "AmberWindowing/Context/Window.h"

#include "AmberWindowing/Inputs/InputManager.h"

#include "AmberCore/Rendering/ECSRenderer.h"
#include "AmberEditor/Core/EditorResources.h"
#include "AmberEditor/Core/ShapeDrawer.h"
#include "AmberUI/Core/UIManager.h"
#include "AmberCore/SceneSystem/SceneManager.h"

#include "AmberRendering/Buffers/ShaderStorageBuffer.h"
#include "AmberRendering/Buffers/UniformBuffer.h"

#include "AmberCore/ResourceManagement/MaterialManager.h"
#include "AmberCore/ResourceManagement/ModelManager.h"
#include "AmberCore/ResourceManagement/ShaderManager.h"
#include "AmberCore/ResourceManagement/TextureManager.h"
#include "AmberCore/ResourceManagement/SoundManager.h"

#include "AmberPhysics/Core/PhysicsEngine.h"

#include "AmberAudio/Core/AudioEngine.h"
#include "AmberAudio/Core/AudioPlayer.h"

#include "AmberCore/Scripting/ScriptInterpreter.h"

#include "AmberTools/FileSystem/IniFile.h"

namespace AmberEditor::Core
{
	class Context final
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
		const std::string projectScriptsPath;

		std::unique_ptr<AmberWindowing::Context::Device> device;
		std::unique_ptr<AmberWindowing::Context::Window> window;
		std::unique_ptr<AmberRendering::Context::Driver> driver;
		std::unique_ptr<AmberWindowing::Inputs::InputManager>         inputManager;
		std::unique_ptr<AmberCore::Rendering::ECSRenderer>                  renderer;
		std::unique_ptr<AmberUI::Core::UIManager>          uiManager;
		std::unique_ptr<AmberRendering::Buffers::UniformBuffer>       engineUBO;
		std::unique_ptr<AmberRendering::Buffers::ShaderStorageBuffer> lightSSBO;
		std::unique_ptr<AmberRendering::Buffers::ShaderStorageBuffer> simulatedLightSSBO;
		std::unique_ptr<EditorResources>              editorResources;
		std::unique_ptr<ShapeDrawer>                  shapeDrawer;
		std::unique_ptr<AmberPhysics::Core::PhysicsEngine> physicsEngine;
		std::unique_ptr<AmberAudio::Core::AudioEngine> audioEngine;
		std::unique_ptr<AmberAudio::Core::AudioPlayer> audioPlayer;
		std::unique_ptr<AmberCore::Scripting::ScriptInterpreter> scriptInterpreter;

		AmberCore::ResourceManagement::ModelManager modelManager;
		AmberCore::ResourceManagement::TextureManager textureManager;
		AmberCore::ResourceManagement::ShaderManager shaderManager;
		AmberCore::ResourceManagement::MaterialManager	materialManager;
		AmberCore::ResourceManagement::SoundManager soundManager;

		AmberCore::SceneSystem::SceneManager sceneManager;

		AmberTools::FileSystem::IniFile projectSettings;

		AmberMaths::Transform simulatedLightTransform;
	};
}

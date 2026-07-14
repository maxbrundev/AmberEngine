#include "Amberpch.h"

#include "AmberGame/Core/Context.h"

#include "AmberTools/Global/ServiceLocator.h"

AmberGame::Core::Context::Context() :
	engineAssetsPath("Data\\Engine\\"),
	projectAssetsPath("Data\\User\\Assets\\"),
	projectScriptsPath("Data\\User\\Scripts\\"),
	projectSettings("Data\\User\\Game.ini"),
	sceneManager(projectAssetsPath)
{
	AmberCore::ResourceManagement::ModelManager::ProvideAssetPaths(projectAssetsPath, engineAssetsPath);
	AmberCore::ResourceManagement::TextureManager::ProvideAssetPaths(projectAssetsPath, engineAssetsPath);
	AmberCore::ResourceManagement::ShaderManager::ProvideAssetPaths(projectAssetsPath, engineAssetsPath);
	AmberCore::ResourceManagement::MaterialManager::ProvideAssetPaths(projectAssetsPath, engineAssetsPath);
	AmberCore::ResourceManagement::SoundManager::ProvideAssetPaths(projectAssetsPath, engineAssetsPath);

	AmberWindowing::Settings::DeviceSettings deviceSettings;
	deviceSettings.contextVersionMajor = 4;
	deviceSettings.contextVersionMinor = 3;
	deviceSettings.debugProfile = false;

	const std::string executableName = projectSettings.GetOrDefault<std::string>("executable_name", "Amber Game");

	AmberWindowing::Settings::WindowSettings windowSettings;
	windowSettings.title = executableName.c_str();
	windowSettings.width = static_cast<uint16_t>(projectSettings.GetOrDefault<int>("x_resolution", 1280));
	windowSettings.height = static_cast<uint16_t>(projectSettings.GetOrDefault<int>("y_resolution", 720));
	windowSettings.fullScreen = projectSettings.GetOrDefault<bool>("fullscreen", false);
	windowSettings.resizable = false;
	windowSettings.vsync = projectSettings.GetOrDefault<bool>("vsync", true);
	windowSettings.samples = static_cast<uint8_t>(projectSettings.GetOrDefault<int>("samples", 4));

	device = std::make_unique<AmberWindowing::Context::Device>(deviceSettings);
	window = std::make_unique<AmberWindowing::Context::Window>(*device, windowSettings);

	auto[monWidth, monHeight] = device->GetMonitorSize();
	auto[winWidth, winHeight] = window->GetSize();
	window->SetPosition(monWidth / 2 - winWidth / 2, monHeight / 2 - winHeight / 2);

	inputManager = std::make_unique<AmberWindowing::Inputs::InputManager>(*window);

	AmberRendering::Settings::DriverSettings driverSettings;
	driverSettings.enableDepthTest = true;
	driverSettings.enableBlend = true;
	driverSettings.enableBackFaceCulling = true;
	driverSettings.enableMultisample = projectSettings.GetOrDefault<bool>("multisampling", true);
	driverSettings.enableDebugCallback = false;

	driver = std::make_unique<AmberRendering::Context::Driver>(driverSettings);
	renderer = std::make_unique<AmberCore::Rendering::ECSRenderer>(*driver);

	uiManager = std::make_unique<AmberUI::Core::UIManager>(window->GetGlfwWindow());
	uiManager->LoadFont("Ruda_Small", engineAssetsPath + "Fonts\\Ruda-Bold.ttf", 12);
	uiManager->LoadFont("Ruda_Medium", engineAssetsPath + "Fonts\\Ruda-Bold.ttf", 14);
	uiManager->LoadFont("Ruda_Big", engineAssetsPath + "Fonts\\Ruda-Bold.ttf", 16);
	uiManager->UseFont("Ruda_Medium");
	uiManager->EnableEditorLayoutSave(false);
	uiManager->EnableDocking(false);

	physicsEngine = std::make_unique<AmberPhysics::Core::PhysicsEngine>(AmberPhysics::Settings::PhysicsSettings{ { 0.0f, projectSettings.GetOrDefault<float>("gravity", -9.81f), 0.0f } });

	audioEngine = std::make_unique<AmberAudio::Core::AudioEngine>(projectAssetsPath);
	audioPlayer = std::make_unique<AmberAudio::Core::AudioPlayer>(*audioEngine);

	scriptInterpreter = std::make_unique<AmberCore::Scripting::ScriptInterpreter>(projectScriptsPath);

	AmberTools::Global::ServiceLocator::Provide(*window);
	AmberTools::Global::ServiceLocator::Provide(*inputManager);
	AmberTools::Global::ServiceLocator::Provide(*renderer);
	AmberTools::Global::ServiceLocator::Provide(modelManager);
	AmberTools::Global::ServiceLocator::Provide(textureManager);
	AmberTools::Global::ServiceLocator::Provide(shaderManager);
	AmberTools::Global::ServiceLocator::Provide(materialManager);
	AmberTools::Global::ServiceLocator::Provide(soundManager);
	AmberTools::Global::ServiceLocator::Provide<AmberCore::SceneSystem::SceneManager>(sceneManager);
	AmberTools::Global::ServiceLocator::Provide<AmberUI::Core::UIManager>(*uiManager);
	AmberTools::Global::ServiceLocator::Provide<AmberPhysics::Core::PhysicsEngine>(*physicsEngine);
	AmberTools::Global::ServiceLocator::Provide<AmberAudio::Core::AudioEngine>(*audioEngine);
	AmberTools::Global::ServiceLocator::Provide<AmberAudio::Core::AudioPlayer>(*audioPlayer);

	engineUBO = std::make_unique<AmberRendering::Buffers::UniformBuffer>
	(
		sizeof(glm::mat4) +
		sizeof(glm::mat4) +
		sizeof(glm::mat4) +
		sizeof(glm::vec3) +
		sizeof(float) +
		sizeof(glm::mat4),
		0, 0,
		AmberRendering::Buffers::EAccessSpecifier::STREAM_DRAW
	);

	lightSSBO = std::make_unique<AmberRendering::Buffers::ShaderStorageBuffer>(AmberRendering::Buffers::EAccessSpecifier::STREAM_DRAW);
	lightSSBO->Bind(0);
}

AmberGame::Core::Context::~Context()
{
	modelManager.UnloadResources();
	textureManager.UnloadResources();
	shaderManager.UnloadResources();
	materialManager.UnloadResources();
	soundManager.UnloadResources();
}

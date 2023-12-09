#include "pch.h"

#include "Application.h"

#include <AmberEngine/Core/ECS/Actor.h>

#include <AmberEngine/Core/ECS/Components/CModelRenderer.h>
#include <AmberEngine/Core/ECS/Components/CDirectionalLight.h>
#include <AmberEngine/Core/ECS/Components/CPointLight.h>

#include <AmberEngine/Core/SceneSystem/Scene.h>
#include <AmberEngine/Core/SceneSystem/SceneManager.h>

#include <AmberEngine/Managers/ResourcesManager.h>
#include <AmberEngine/Resources/Loaders/ShaderLoader.h>

#include <AmberEngine/Tools/Time/Clock.h>

Example::Application::Application(const AmberEngine::Settings::DeviceSettings & p_deviceSettings, const AmberEngine::Settings::WindowSettings & p_windowSettings, const AmberEngine::Settings::DriverSettings & p_driverSettings) :
	m_context("res\\", p_deviceSettings, p_windowSettings, p_driverSettings),
	m_editor(m_context),
	isRunning(true)
{
	m_context.sceneManager.LoadEmptyLightedScene();
}

void Example::Application::Setup()
{
	//auto& resourcesManager = AmberEngine::Managers::ResourcesManager::Instance();
	//resourcesManager.LoadTexture("diffuse", "Textures\\crystal.jpg", AmberEngine::Resources::ETextureFilteringMode::NEAREST_MIPMAP_LINEAR, AmberEngine::Settings::ETextureFilteringMode::NEAREST, AmberEngine::Settings::ETextureType::DIFFUSE_MAP,true, true);
	//resourcesManager.LoadTexture("specular", "Textures\\crystal_spec.jpg", AmberEngine::Resources::ETextureFilteringMode::NEAREST_MIPMAP_LINEAR, AmberEngine::Settings::ETextureFilteringMode::NEAREST, AmberEngine::Settings::ETextureType::SPECULAR_MAP, true, true);
}

void Example::Application::Run()
{
	Tools::Time::Clock clock;

	auto& resourcesManager = AmberEngine::Managers::ResourcesManager::Instance();

	auto& testActor = m_context.sceneManager.GetCurrentScene()->CreateActor("Actor1");
	auto& testActor2 = m_context.sceneManager.GetCurrentScene()->CreateActor("Actor2");
	auto& testActor3 = m_context.sceneManager.GetCurrentScene()->CreateActor("Actor3");
	auto& testActor4 = m_context.sceneManager.GetCurrentScene()->CreateActor("Actor4");
	auto& testActor5 = m_context.sceneManager.GetCurrentScene()->CreateActor("Actor5");

	testActor.AddComponent<AmberEngine::Core::ECS::Components::CModelRenderer>("Sponza", "Models\\Sponza\\sponza.obj");
	testActor.GetComponent<AmberEngine::Core::ECS::Components::CModelRenderer>()->GetModel()->SetShader(resourcesManager.GetShader("Standard"));
	testActor.transform.SetWorldScale({ 0.05f, 0.05f, 0.05f });

	testActor4.AddComponent<AmberEngine::Core::ECS::Components::CModelRenderer>("Cube", ":Models\\Cube.fbx");

	testActor2.AddComponent<AmberEngine::Core::ECS::Components::CModelRenderer>("DamagedHelmet", "Models\\DamagedHelmet\\glTF\\DamagedHelmet.gltf");
	testActor2.GetComponent<AmberEngine::Core::ECS::Components::CModelRenderer>()->GetModel()->SetShader(resourcesManager.GetShader("Standard"));

	testActor3.AddComponent<AmberEngine::Core::ECS::Components::CModelRenderer>("Nanosuit", "Models\\Nanosuit\\nanosuit.obj");
	testActor3.GetComponent<AmberEngine::Core::ECS::Components::CModelRenderer>()->GetModel()->SetShader(resourcesManager.GetShader("Standard"));

	testActor3.SetParent(testActor2);
	testActor4.SetParent(testActor2);
	testActor.SetParent(testActor2);
	testActor5.AddComponent<AmberEngine::Core::ECS::Components::CPointLight>();

	testActor3.transform.SetLocalPosition({ 10.0f, 0.0f, 0.0f });

	while (IsRunning())
	{
		m_editor.PreUpdate();
		m_editor.Update(clock.GetDeltaTime());
		{
			if (m_context.inputManager->IsKeyPressed(AmberEngine::Inputs::EKey::KEY_R))
			{
				std::string realPath = AmberEngine::Managers::ResourcesManager::Instance().GetRealPath(":Shaders\\Standard.glsl");
				AmberEngine::Resources::Loaders::ShaderLoader::Recompile(resourcesManager.GetShader("Standard"), realPath);
			}
			
			//Test
			if (m_context.inputManager->IsKeyPressed(AmberEngine::Inputs::EKey::KEY_F))
			{
				//working
				//testActor3.RemoveComponent<AmberEngine::Core::ECS::Components::CModelRenderer>();
				//working
				//m_context.sceneManager.GetCurrentScene()->DestroyActor(testActor2);
				//working
				testActor3.RemoveParent();
			}
		}

		
		m_editor.PostUpdate();

		clock.Update();
	}
}

bool Example::Application::IsRunning() const
{
	return isRunning && m_context.window->IsActive();
}

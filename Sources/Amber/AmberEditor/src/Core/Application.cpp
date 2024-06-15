#include "pch.h"

#include "Core/Application.h"

#include <AmberEngine/Core/ECS/Actor.h>


#include <AmberEngine/Core/ECS/Components/CModelRenderer.h>
#include <AmberEngine/Core/ECS/Components/CMaterialRenderer.h>
#include <AmberEngine/Core/ECS/Components/CDirectionalLight.h>
#include <AmberEngine/Core/ECS/Components/CPointLight.h>

#include <AmberEngine/Core/SceneSystem/Scene.h>
#include <AmberEngine/Core/SceneSystem/SceneManager.h>

#include <AmberEngine/Managers/ModelManager.h>
#include <AmberEngine/Managers/ShaderManager.h>
#include <AmberEngine/Managers/TextureManager.h>


#include <AmberEngine/Tools/Time/Clock.h>

static char filePath[1024] = "";
Core::Application::Application(const AmberEngine::Settings::DeviceSettings& p_deviceSettings, const AmberEngine::Settings::WindowSettings & p_windowSettings, const AmberEngine::Settings::DriverSettings & p_driverSettings) :
	m_context("", p_deviceSettings, p_windowSettings, p_driverSettings),
	m_editor(m_context),
	isRunning(true)
{
	m_context.sceneManager.LoadEmptyLightedScene();
}

void Core::Application::Setup()
{
	//auto& resourcesManager = AmberEngine::Managers::ResourcesManager::Instance();
	//resourcesManager.LoadTexture("diffuse", "Textures\\crystal.jpg", AmberEngine::Resources::ETextureFilteringMode::NEAREST_MIPMAP_LINEAR, AmberEngine::Settings::ETextureFilteringMode::NEAREST, AmberEngine::Settings::ETextureType::DIFFUSE_MAP,true, true);
	//resourcesManager.LoadTexture("specular", "Textures\\crystal_spec.jpg", AmberEngine::Resources::ETextureFilteringMode::NEAREST_MIPMAP_LINEAR, AmberEngine::Settings::ETextureFilteringMode::NEAREST, AmberEngine::Settings::ETextureType::SPECULAR_MAP, true, true);
}

void Core::Application::Run()
{
	Tools::Time::Clock clock;

	auto& testActor = m_context.sceneManager.GetCurrentScene()->CreateActor("Actor1");
	auto& testActor2 = m_context.sceneManager.GetCurrentScene()->CreateActor("Actor2");
	auto& testActor3 = m_context.sceneManager.GetCurrentScene()->CreateActor("Actor3");
	auto& testActor4 = m_context.sceneManager.GetCurrentScene()->CreateActor("Actor4");
	auto& testActor5 = m_context.sceneManager.GetCurrentScene()->CreateActor("Actor5");

	//testActor.AddComponent<AmberEngine::Core::ECS::Components::CModelRenderer>("Sponza", "Models\\Sponza\\sponza.obj");
	//testActor.GetComponent<AmberEngine::Core::ECS::Components::CModelRenderer>()->GetModel()->SetShader(resourcesManager.GetShader("Standard"));
	//testActor.transform.SetWorldScale({ 0.05f, 0.05f, 0.05f });

	//testActor4.AddComponent<AmberEngine::Core::ECS::Components::CModelRenderer>();
	//testActor4.AddComponent<AmberEngine::Core::ECS::Components::CMaterialRenderer>();

	{
		//auto& modelComponent = testActor.AddComponent<AmberEngine::Core::ECS::Components::CModelRenderer>();
		//modelComponent.SetModel(m_context.modelManager.GetResource("Models\\Sponza\\sponza.obj"));

		//auto& materialRenderer = testActor.AddComponent<AmberEngine::Core::ECS::Components::CMaterialRenderer>();
		//materialRenderer.FillTextureData(modelComponent.GetModel()->LoadedTextureData);
		//for (const auto& material : materialRenderer.GetMaterials())
		//{
		//	if (material)
		//	{
		//		material->SetShader(m_context.shaderManager.GetResource(":Shaders\\Standard.glsl"));
		//	}
		//}
	}

	{
		auto& modelComponent = testActor2.AddComponent<AmberEngine::Core::ECS::Components::CModelRenderer>();
		modelComponent.SetModel(m_context.modelManager.GetResource("Models\\DamagedHelmet\\glTF\\DamagedHelmet.gltf"));

		auto& materialRenderer = testActor2.AddComponent<AmberEngine::Core::ECS::Components::CMaterialRenderer>();
		materialRenderer.FillTextureData(modelComponent.GetModel()->LoadedTextureData);
		for (const auto& material : materialRenderer.GetMaterials())
		{
			if(material)
			{
				material->SetShader(m_context.shaderManager.GetResource(":Shaders\\Standard.glsl"));
				material->SetBlendable(true);
				material->SetUniform("u_Diffuse", glm::vec4(1.f, 0.f, 0.f, 0.5f));
			}
		}
	}

	//const auto& modelComponent3 = testActor3.AddComponent<AmberEngine::Core::ECS::Components::CModelRenderer>("Nanosuit", "Models\\Nanosuit\\nanosuit.obj");
	//testActor3.AddComponent<AmberEngine::Core::ECS::Components::CMaterialRenderer>();
	//auto& materialRenderer3 = testActor3.AddComponent<AmberEngine::Core::ECS::Components::CMaterialRenderer>();
	//materialRenderer3.FillWithMaterials(modelComponent3.GetModel()->GetMaterials());
	//const auto& mat3 = testActor3.GetComponent<AmberEngine::Core::ECS::Components::CModelRenderer>()->GetModel()->GetMaterials();
	//for (const auto& material : mat3)
	//{
	//	if (material)
	//	{
	//		material->SetShader(&resourcesManager.GetShader("Standard"));
	//	}
	//}
	//
	//testActor3.GetComponent<AmberEngine::Core::ECS::Components::CModelRenderer>()->GetModel()->SetShader(resourcesManager.GetShader("Standard"));

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
				//std::string realPath = AmberEngine::Managers::ResourcesManager::Instance().GetRealPath(":Shaders\\Standard.glsl");
				//AmberEngine::Resources::Loaders::ShaderLoader::Recompile(resourcesManager.GetShader("Standard"), realPath);
			}
			
			//Test
			if (m_context.inputManager->IsKeyPressed(AmberEngine::Inputs::EKey::KEY_F))
			{
				//working
				//testActor3.RemoveComponent<AmberEngine::Core::ECS::Components::CModelRenderer>();
				//working
				testActor2.MarkAsDestroy();
				
				//working
				//testActor3.RemoveParent();
			}
		}

		m_editor.PostUpdate();

		clock.Update();
	}
}

bool Core::Application::IsRunning() const
{
	return isRunning && m_context.window->IsActive();
}

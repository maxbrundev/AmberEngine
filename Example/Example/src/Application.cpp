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
}

void Example::Application::Setup()
{
	auto& resourcesManager = AmberEngine::Managers::ResourcesManager::Instance();

	//resourcesManager.LoadTexture("diffuse", "Textures\\crystal.jpg", AmberEngine::Settings::ETextureFilteringMode::NEAREST_MIPMAP_LINEAR, AmberEngine::Settings::ETextureFilteringMode::NEAREST, AmberEngine::Settings::ETextureType::DIFFUSE_MAP,true, true);
	//resourcesManager.LoadTexture("specular", "Textures\\crystal_spec.jpg", AmberEngine::Settings::ETextureFilteringMode::NEAREST_MIPMAP_LINEAR, AmberEngine::Settings::ETextureFilteringMode::NEAREST, AmberEngine::Settings::ETextureType::SPECULAR_MAP, true, true);
}

void Example::Application::Run()
{
	Tools::Time::Clock clock;

	auto& resourcesManager = AmberEngine::Managers::ResourcesManager::Instance();

	AmberEngine::Core::ECS::Actor* testActor = new AmberEngine::Core::ECS::Actor(1, "Actor1", "");
	AmberEngine::Core::ECS::Actor* testActor2 = new AmberEngine::Core::ECS::Actor(2, "Actor2", "");
	AmberEngine::Core::ECS::Actor* testActor3 = new AmberEngine::Core::ECS::Actor(3, "Actor3", "");
	AmberEngine::Core::ECS::Actor* testActor4 = new AmberEngine::Core::ECS::Actor(4, "Actor4", "");

	AmberEngine::Core::ECS::Actor* directionalLight = new AmberEngine::Core::ECS::Actor(5, "Directional Light", "");
	directionalLight->AddComponent<AmberEngine::Core::ECS::Components::CDirectionalLight>();
	directionalLight->GetComponent<AmberEngine::Core::ECS::Components::CLight>()->SetIntensity(0.750f);

	//testActor->AddComponent<AmberEngine::Core::ECS::Components::ModelComponent>("Sponza", "Models\\Sponza\\sponza.obj");
	//testActor->GetComponent<AmberEngine::Core::ECS::Components::ModelComponent>()->GetModel()->SetShader(resourcesManager.GetShader("Standard"));
	//testActor->GetTransform().SetWorldScale({ 0.05f, 0.05f, 0.05f });

	testActor4->AddComponent<AmberEngine::Core::ECS::Components::CModelRenderer>("Cube", ":Models\\Cube.fbx");

	testActor2->AddComponent<AmberEngine::Core::ECS::Components::CModelRenderer>("Nanosuit", "Models\\DamagedHelmet\\glTF\\DamagedHelmet.gltf");
	testActor2->GetComponent<AmberEngine::Core::ECS::Components::CModelRenderer>()->GetModel()->SetShader(resourcesManager.GetShader("Standard"));

	testActor3->AddComponent<AmberEngine::Core::ECS::Components::CModelRenderer>("Nanosuit", "Models\\Nanosuit\\nanosuit.obj");
	testActor3->GetComponent<AmberEngine::Core::ECS::Components::CModelRenderer>()->GetModel()->SetShader(resourcesManager.GetShader("Standard"));

	testActor3->SetParent(*testActor2);
	testActor4->SetParent(*testActor2);
	testActor->SetParent(*testActor2);
	testActor->AddComponent<AmberEngine::Core::ECS::Components::CPointLight>();
	directionalLight->SetParent(*testActor2);

	testActor3->transform.SetLocalPosition({ 10.0f, 0.0f, 0.0f });
	m_context.sceneManager.GetCurrentScene()->AddActor(testActor);
	m_context.sceneManager.GetCurrentScene()->AddActor(testActor2);
	m_context.sceneManager.GetCurrentScene()->AddActor(testActor3);
	m_context.sceneManager.GetCurrentScene()->AddActor(testActor4);
	m_context.sceneManager.GetCurrentScene()->AddActor(directionalLight);


	float rotationX;
	float rotationY;
	float rotationZ;
	
	float positionX2;
	float positionY2;
	float positionZ2;
	
	float rotationX3;
	float rotationY3;
	float rotationZ3;
	float intensity;

	while (IsRunning())
	{
		m_editor.PreUpdate();
		m_editor.Update(clock.GetDeltaTime());
		{
			if (directionalLight)
			{
				rotationX = directionalLight->transform.GetLocalRotation().x;
				rotationY = directionalLight->transform.GetLocalRotation().y;
				rotationZ = directionalLight->transform.GetLocalRotation().z;
				intensity = directionalLight->GetComponent<AmberEngine::Core::ECS::Components::CLight>()->GetData().intensity;
			}
			
			if (testActor2)
			{
				positionX2 = testActor2->transform.GetLocalPosition().x;
				positionY2 = testActor2->transform.GetLocalPosition().y;
				positionZ2 = testActor2->transform.GetLocalPosition().z;
			}
			
			if (testActor3)
			{
				rotationX3 = testActor3->transform.GetLocalRotation().x;
				rotationY3 = testActor3->transform.GetLocalRotation().y;
				rotationZ3 = testActor3->transform.GetLocalRotation().z;
			}
			
			ImGui::Begin("Directional Test");
			ImGui::DragFloat("Rotation X", &rotationX);
			ImGui::DragFloat("Rotation Y", &rotationY);
			ImGui::DragFloat("Rotation Z", &rotationZ);
			
			ImGui::DragFloat("Position X2", &positionX2);
			ImGui::DragFloat("Position Y2", &positionY2);
			ImGui::DragFloat("Position Z2", &positionZ2);
			
			ImGui::DragFloat("Position X3", &rotationX3);
			ImGui::DragFloat("Position Y3", &rotationY3);
			ImGui::DragFloat("Position Z3", &rotationZ3);
			
			ImGui::DragFloat("Intensity", &intensity);
			ImGui::End();
			
			if (directionalLight)
			{
				directionalLight->transform.SetLocalRotation({ rotationX, rotationY, rotationZ });
				directionalLight->GetComponent<AmberEngine::Core::ECS::Components::CDirectionalLight>()->SetIntensity(intensity);
			}
			
			if (testActor2)
				testActor2->transform.SetLocalPosition({ positionX2, positionY2, positionZ2 });
			
			if (testActor3)
				testActor3->transform.SetLocalRotation({ rotationX3, rotationY3, rotationZ3 });
			
			if (m_context.inputManager->IsKeyPressed(AmberEngine::Inputs::EKey::KEY_R))
			{
				std::string realPath = AmberEngine::Managers::ResourcesManager::Instance().GetRealPath(":Shaders\\Standard.glsl");
				AmberEngine::Resources::Loaders::ShaderLoader::Recompile(resourcesManager.GetShader("Standard"), realPath);
			}
			
			//Test
			if (m_context.inputManager->IsKeyPressed(AmberEngine::Inputs::EKey::KEY_F))
			{
				//working
				//testActor3->RemoveComponent<AmberEngine::Core::ECS::Components::ModelComponent>();
				//working
				m_context.sceneManager.GetCurrentScene()->DestroyActor(*testActor2);
				//working
				//testActor3->RemoveParent();
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

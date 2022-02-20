#include "pch.h"

#include "Application.h"

#include <AmberEngine/Tools/Time/Clock.h>

#include <AmberEngine/Managers/ResourcesManager.h>

#include <AmberEngine/Core/ECS/Actor.h>
#include <AmberEngine/Core/ECS/Components/ModelComponent.h>

#include <AmberEngine/Rendering/Entities/ELightType.h>
#include <AmberEngine/Resources/Loaders/ShaderLoader.h>

#include <AmberEngine/ImGui/imgui.h>

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
	Utils::Clock clock;

	auto& resourcesManager = AmberEngine::Managers::ResourcesManager::Instance();

	AmberEngine::ECS::Actor* testActor = new AmberEngine::ECS::Actor("Actor1");
	AmberEngine::ECS::Actor* testActor2 = new AmberEngine::ECS::Actor("Actor2");
	AmberEngine::ECS::Actor* testActor3 = new AmberEngine::ECS::Actor("Actor3");
	AmberEngine::ECS::Actor* testActor4 = new AmberEngine::ECS::Actor("Actor4");

	AmberEngine::ECS::Actor* directionalLight = new AmberEngine::ECS::Actor("Directional Light");
	directionalLight->AddComponent<AmberEngine::ECS::Components::LightComponent>(AmberEngine::Rendering::Entities::ELightType::DIRECTIONAL);
	directionalLight->GetComponent<AmberEngine::ECS::Components::LightComponent>()->SetIntensity(0.750f);

	testActor->AddComponent<AmberEngine::ECS::Components::ModelComponent>("Sponza", "Models\\Sponza\\sponza.obj");
	testActor->GetComponent<AmberEngine::ECS::Components::ModelComponent>()->GetModel()->SetShader(resourcesManager.GetShader("Standard"));
	testActor->GetTransform().SetWorldScale({ 0.05f, 0.05f, 0.05f });

	testActor4->AddComponent<AmberEngine::ECS::Components::ModelComponent>("Cube", ":Models\\Cube.fbx");

	testActor2->AddComponent<AmberEngine::ECS::Components::ModelComponent>("Nanosuit", "Models\\Nanosuit\\nanosuit.obj");
	testActor2->GetComponent<AmberEngine::ECS::Components::ModelComponent>()->GetModel()->SetShader(resourcesManager.GetShader("Standard"));

	testActor3->AddComponent<AmberEngine::ECS::Components::ModelComponent>("Nanosuit", "Models\\Nanosuit\\nanosuit.obj");
	testActor3->GetComponent<AmberEngine::ECS::Components::ModelComponent>()->GetModel()->SetShader(resourcesManager.GetShader("Standard"));

	testActor3->SetParent(*testActor2);
	testActor3->GetTransform().SetLocalPosition({ 10.0f, 0.0f, 0.0f });

	m_context.m_scene.AddActor(testActor);
	m_context.m_scene.AddActor(testActor2);
	m_context.m_scene.AddActor(testActor3);
	m_context.m_scene.AddActor(testActor4);
	m_context.m_scene.AddActor(directionalLight);

	float rotationX;
	float rotationY;
	float rotationZ;

	float positionX2;
	float positionY2;
	float positionZ2;

	float positionX3;
	float positionY3;
	float positionZ3;
	float intensity;

	while (IsRunning())
	{
		m_editor.PreUpdate();

		{
			if (directionalLight)
			{
				rotationX = directionalLight->GetTransform().GetLocalRotationEuler().x;
				rotationY = directionalLight->GetTransform().GetLocalRotationEuler().y;
				rotationZ = directionalLight->GetTransform().GetLocalRotationEuler().z;
				intensity = directionalLight->GetComponent<AmberEngine::ECS::Components::LightComponent>()->GetLightData().intensity;
			}
			
			if (testActor2)
			{
				positionX2 = testActor2->GetTransform().GetLocalPosition().x;
				positionY2 = testActor2->GetTransform().GetLocalPosition().y;
				positionZ2 = testActor2->GetTransform().GetLocalPosition().z;
			}
			
			if (testActor3)
			{
				positionX3 = testActor3->GetTransform().GetLocalRotationEuler().x;
				positionY3 = testActor3->GetTransform().GetLocalRotationEuler().y;
				positionZ3 = testActor3->GetTransform().GetLocalRotationEuler().z;
			}
			
			ImGui::Begin("Directional Test");
			ImGui::SliderFloat("Rotation X", &rotationX, 0.0f, 360.0f);
			ImGui::SliderFloat("Rotation Y", &rotationY, 0.0f, 360.0f);
			ImGui::SliderFloat("Rotation Z", &rotationZ, 0.0f, 360.0f);
			
			ImGui::SliderFloat("Position X2", &positionX2, -100.0f, 100.0f);
			ImGui::SliderFloat("Position Y2", &positionY2, -100.0f, 100.0f);
			ImGui::SliderFloat("Position Z2", &positionZ2, -100.0f, 100.0f);
			
			ImGui::SliderFloat("Position X3", &positionX3, -360.0f, 360.0f);
			ImGui::SliderFloat("Position Y3", &positionY3, -360.0f, 360.0f);
			ImGui::SliderFloat("Position Z3", &positionZ3, -360.0f, 360.0f);
			
			ImGui::SliderFloat("Intensity", &intensity, 0.0f, 1.0f);
			ImGui::End();
			
			if (directionalLight)
			{
				directionalLight->GetTransform().SetLocalRotation({ rotationX, rotationY, rotationZ });
				directionalLight->GetComponent<AmberEngine::ECS::Components::LightComponent>()->SetIntensity(intensity);
			}
			
			if (testActor2)
				testActor2->GetTransform().SetLocalPosition({ positionX2, positionY2, positionZ2 });
			
			if (testActor3)
				testActor3->GetTransform().SetLocalRotation({ positionX3, positionY3, positionZ3 });
			
			if (m_context.inputManager->IsKeyPressed(AmberEngine::Inputs::EKey::KEY_R))
			{
				std::string realPath = AmberEngine::Managers::ResourcesManager::Instance().GetRealPath(":Shaders\\Standard.glsl");
				AmberEngine::Resources::Loaders::ShaderLoader::Recompile(resourcesManager.GetShader("Standard"), realPath);
			}
			
			//Test
			if (m_context.inputManager->IsKeyPressed(AmberEngine::Inputs::EKey::KEY_F))
			{
				//working
				//testActor3->RemoveComponent<AmberEngine::ECS::Components::ModelComponent>();
				//working
				m_context.m_scene.DestroyActor(testActor3);
				//working
				//testActor3->RemoveParent();
			}
		}

		m_editor.Update(clock.GetDeltaTime());
		m_editor.PostUpdate();

		clock.Update();
	}
}

bool Example::Application::IsRunning() const
{
	return isRunning && m_context.window->IsActive();
}

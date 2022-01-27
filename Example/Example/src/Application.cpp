#include "pch.h"

#include "Application.h"

#include <AmberEngine/Tools/Time/Clock.h>

#include <AmberEngine/Managers/ResourcesManager.h>

#include <AmberEngine/Core/ECS/Actor.h>
#include <AmberEngine/Core/ECS/Components/ModelComponent.h>

#include <AmberEngine/Rendering/Entities/ELightType.h>
#include <AmberEngine/Resources/Loaders/ShaderLoader.h>

/*#include <AmberEngine/Resources/Primitives/Cube.h>
#include <AmberEngine/Buffers/VertexBuffer.h>
#include <AmberEngine/Buffers/VertexArray.h>*/

#include <AmberEngine/ImGui/imgui.h>

Example::Application::Application(const AmberEngine::Settings::DeviceSettings & p_deviceSettings,
	const AmberEngine::Settings::WindowSettings & p_windowSettings,
	const AmberEngine::Settings::DriverSettings & p_driverSettings) :
	m_context(p_deviceSettings, p_windowSettings, p_driverSettings),
	m_editor(m_context),
	isRunning(true)
{
}

void Example::Application::Setup()
{
	auto& resourcesManager = AmberEngine::Managers::ResourcesManager::Instance();

	resourcesManager.LoadShader("StandardLighting", "res/shaders/StandardLighting.glsl");

	resourcesManager.LoadTexture("diffuse", "crystal.jpg", AmberEngine::Settings::ETextureFilteringMode::NEAREST_MIPMAP_LINEAR, AmberEngine::Settings::ETextureFilteringMode::NEAREST, AmberEngine::Settings::ETextureType::DIFFUSE,true, true);
	resourcesManager.LoadTexture("specular", "crystal_spec.jpg", AmberEngine::Settings::ETextureFilteringMode::NEAREST_MIPMAP_LINEAR, AmberEngine::Settings::ETextureFilteringMode::NEAREST, AmberEngine::Settings::ETextureType::SPECULAR, true, true);
}

void Example::Application::Run()
{
	/*AmberEngine::PrimitivesShapes::Cube::Setup();

	std::vector<float> vertices;
	std::vector<AmberEngine::PrimitivesShapes::Vertex> verticesCube = AmberEngine::PrimitivesShapes::Cube::GetVertices();

	for (auto& vertex : verticesCube)
	{
		vertices.push_back(vertex.position[0]);
		vertices.push_back(vertex.position[1]);
		vertices.push_back(vertex.position[2]);
		vertices.push_back(vertex.textureCoord[0]);
		vertices.push_back(vertex.textureCoord[1]);
		vertices.push_back(vertex.normals[0]);
		vertices.push_back(vertex.normals[1]);
		vertices.push_back(vertex.normals[2]);
	}

	AmberEngine::Buffers::VertexArray vao;
	AmberEngine::Buffers::VertexBuffer vbo(vertices.data(), vertices.size());
	vao.BindAttribPointer(3, GL_FLOAT, GL_FALSE, sizeof(AmberEngine::PrimitivesShapes::Vertex), nullptr);
	vao.BindAttribPointer(2, GL_FLOAT, GL_FALSE, sizeof(AmberEngine::PrimitivesShapes::Vertex), reinterpret_cast<GLvoid*>(offsetof(AmberEngine::PrimitivesShapes::Vertex, AmberEngine::PrimitivesShapes::Vertex::textureCoord)));
	vao.BindAttribPointer(3, GL_FLOAT, GL_FALSE, sizeof(AmberEngine::PrimitivesShapes::Vertex), reinterpret_cast<GLvoid*>(offsetof(AmberEngine::PrimitivesShapes::Vertex, AmberEngine::PrimitivesShapes::Vertex::normals)));
	vao.Unbind();
	vbo.Unbind();*/

	/*glm::vec3 verticesVec3[] =
	{
		glm::vec3(-1, -1, -1), // 0
		glm::vec3(1, -1, -1), // 1
		glm::vec3(1, 1, -1.0f), // 2
		glm::vec3(-1, 1, -1), // 3
		glm::vec3(-1, -1, 1), // 4
		glm::vec3(1, -1, 1), // 5
		glm::vec3(1, 1, 1), // 6
		glm::vec3(-1, 1, 1) // 7
	};

	float vertices[] =
	{
		-1, -1, -1, // 0

		1, -1, -1, // 1

		1, 1, -1.0f, // 2

		-1, 1, -1, // 3

		-1, -1, 1, // 4

		1, -1, 1, // 5

		1, 1, 1, // 6

		-1, 1, 1 // 7
	};

	int indices[36] =
	{
		0, 1, 3, 3, 1, 2,
		1, 5, 2, 2, 5, 6,
		5, 4, 6, 6, 4, 7,
		4, 0, 7, 7, 0, 3,
		3, 2, 7, 7, 2, 6,
		4, 5, 0, 0, 5, 1
	};

	//108 vertex positions -> 3 * 36 -> 3 vertex positions (x,y,z) by 1 index
	float vertexBuffer[108]{ 0.0f };
	for (int i = 0; i < 36; i++) {

		int indexX = indices[i];
		int indexY = indices[i];
		int indexZ = indices[i];

		float verticeX = vertices[(indexX) * 3 + 0];
		float verticeY = vertices[(indexY) * 3 + 1];
		float verticeZ = vertices[(indexZ) * 3 + 2];

		vertexBuffer[i * 3 + 0] = verticeX;
		vertexBuffer[i * 3 + 1] = verticeY;
		vertexBuffer[i * 3 + 2] = verticeZ;
	}*/

	Utils::Clock clock;

	auto& resourcesManager = AmberEngine::Managers::ResourcesManager::Instance();

	AmberEngine::ECS::Actor* testActor = new AmberEngine::ECS::Actor("Actor1");
	AmberEngine::ECS::Actor* testActor2 = new AmberEngine::ECS::Actor("Actor2");
	AmberEngine::ECS::Actor* testActor3 = new AmberEngine::ECS::Actor("Actor3");

	AmberEngine::ECS::Actor* directionalLight = new AmberEngine::ECS::Actor("Directional Light");
	directionalLight->AddComponent<AmberEngine::ECS::Components::LightComponent>(AmberEngine::Rendering::Entities::ELightType::DIRECTIONAL);
	directionalLight->GetComponent<AmberEngine::ECS::Components::LightComponent>()->GetLightData().color = glm::vec3(1.0f, 0.9f, 0.8f);

	testActor->AddComponent<AmberEngine::ECS::Components::ModelComponent>("Sponza", "res/Mesh/Sponza/sponza.obj");
	testActor->GetComponent<AmberEngine::ECS::Components::ModelComponent>()->GetModel()->SetShader(&resourcesManager.GetShader("StandardLighting"));
	
	testActor->GetTransform().SetWorldScale({ 0.05f, 0.05f, 0.05f });

	testActor2->AddComponent<AmberEngine::ECS::Components::ModelComponent>("Nanosuit", "res/Mesh/Nanosuit/nanosuit.obj");
	testActor2->GetComponent<AmberEngine::ECS::Components::ModelComponent>()->GetModel()->SetShader(&resourcesManager.GetShader("StandardLighting"));

	testActor3->AddComponent<AmberEngine::ECS::Components::ModelComponent>("Nanosuit", "res/Mesh/Nanosuit/nanosuit.obj");
	testActor3->GetComponent<AmberEngine::ECS::Components::ModelComponent>()->GetModel()->SetShader(&resourcesManager.GetShader("StandardLighting"));

	testActor3->SetParent(*testActor2);
	testActor3->GetTransform().SetLocalPosition({ 10.0f, 0.0f, 0.0f });

	m_context.m_scene.AddActor(testActor);
	m_context.m_scene.AddActor(testActor2);
	m_context.m_scene.AddActor(testActor3);
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

	while (IsRunning())
	{
		m_editor.PreUpdate();

		{
			if (directionalLight)
			{
				rotationX = directionalLight->GetTransform().GetLocalRotationEuler().x;
				rotationY = directionalLight->GetTransform().GetLocalRotationEuler().y;
				rotationZ = directionalLight->GetTransform().GetLocalRotationEuler().z;
			}

			if (testActor2)
			{
				positionX2 = testActor2->GetTransform().GetLocalPosition().x;
				positionY2 = testActor2->GetTransform().GetLocalPosition().y;
				positionZ2 = testActor2->GetTransform().GetLocalPosition().z;
			}

			if (testActor3)
			{
				positionX3 = testActor3->GetTransform().GetLocalPosition().x;
				positionY3 = testActor3->GetTransform().GetLocalPosition().y;
				positionZ3 = testActor3->GetTransform().GetLocalPosition().z;
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
			ImGui::End();

			if (directionalLight)
				directionalLight->GetTransform().SetLocalRotation({ rotationX, rotationY, rotationZ });

			if (testActor2)
				testActor2->GetTransform().SetLocalPosition({ positionX2, positionY2, positionZ2 });

			if (testActor3)
				testActor3->GetTransform().SetLocalPosition({ positionX3, positionY3, positionZ3 });

			if (m_context.inputManager->IsKeyPressed(AmberEngine::Inputs::EKey::KEY_R))
			{
				AmberEngine::Resources::Loaders::ShaderLoader::Recompile(resourcesManager.GetShader("StandardLighting"), "res/shaders/StandardLighting.glsl");
			}

			if (m_context.inputManager->IsKeyPressed(AmberEngine::Inputs::EKey::KEY_F))
			{
				//working
				m_context.m_scene.DestroyActor(testActor2);
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

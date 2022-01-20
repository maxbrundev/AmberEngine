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

	resourcesManager.LoadModel("Helmet", "res/Mesh/nanosuit/nanosuit.obj");

	AmberEngine::Resources::Shader& lightingShader = resourcesManager.LoadShader("StandardLighting", "res/shaders/StandardLighting.glsl");
	
	//resourcesManager.LoadTexture("diffuse", "crystal.jpg", AmberEngine::Settings::ETextureFilteringMode::NEAREST_MIPMAP_LINEAR, AmberEngine::Settings::ETextureFilteringMode::NEAREST, AmberEngine::Settings::ETextureType::DIFFUSE,true, true);
	//resourcesManager.LoadTexture("specular", "crystal_spec.jpg", AmberEngine::Settings::ETextureFilteringMode::NEAREST_MIPMAP_LINEAR, AmberEngine::Settings::ETextureFilteringMode::NEAREST, AmberEngine::Settings::ETextureType::DIFFUSE, true, true);

	resourcesManager.GetModel("Helmet").SetShader(lightingShader);

	//glm::vec3 lighDir = glm::vec3(1.0f, 1.0f, 1.0f);
	//
	//lightingShader.Bind();
	////lightingShader.SetUniform1i("u_DiffuseMap", 0);
	////lightingShader.SetUniform1i("u_SpecularMap", 1);
	//lightingShader.SetUniformVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	//lightingShader.SetUniformVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	//lightingShader.SetUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	//lightingShader.SetUniformVec3("light.direction", lighDir);
	//lightingShader.Unbind();
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

	AmberEngine::ECS::Actor* testActor = new AmberEngine::ECS::Actor();
	AmberEngine::ECS::Actor* testActor2 = new AmberEngine::ECS::Actor();
	AmberEngine::ECS::Actor* testActor3 = new AmberEngine::ECS::Actor();
	AmberEngine::ECS::Actor* directionalLight = new AmberEngine::ECS::Actor();
	
	testActor->AddComponent<AmberEngine::ECS::Components::ModelComponent>("Helmet", "res/Mesh/nanosuit/nanosuit.obj");
	testActor->GetComponent<AmberEngine::ECS::Components::ModelComponent>()->GetModel()->SetShader(resourcesManager.GetShader("StandardLighting"));

	testActor2->AddComponent<AmberEngine::ECS::Components::ModelComponent>("Helmet", "res/Mesh/nanosuit/nanosuit.obj");
	testActor2->GetComponent<AmberEngine::ECS::Components::ModelComponent>()->GetModel()->SetShader(resourcesManager.GetShader("StandardLighting"));

	testActor3->AddComponent<AmberEngine::ECS::Components::ModelComponent>("Helmet", "res/Mesh/nanosuit/nanosuit.obj");
	testActor3->GetComponent<AmberEngine::ECS::Components::ModelComponent>()->GetModel()->SetShader(resourcesManager.GetShader("StandardLighting"));

	directionalLight->AddComponent<AmberEngine::ECS::Components::LightComponent>(AmberEngine::Rendering::Entities::ELightType::DIRECTIONAL);

	m_context.m_scene.AddActor(testActor, "Actor1");
	m_context.m_scene.AddActor(testActor2, "Actor2");
	m_context.m_scene.AddActor(testActor3, "Actor3");
	m_context.m_scene.AddActor(directionalLight, "Directional Light");

	testActor2->GetTransform().SetParent(testActor->GetTransform());
	testActor2->GetTransform().SetLocalPosition({ 10.0f, 0.0f, 0.0f });

	testActor3->GetTransform().SetParent(testActor->GetTransform());
	testActor3->GetTransform().SetLocalPosition({ 20.0f, 0.0f, 0.0f });

	float positionX = testActor->GetTransform().GetWorldPosition().x;
	float positionY = testActor->GetTransform().GetWorldPosition().y;
	float positionZ = testActor->GetTransform().GetWorldPosition().z;

	float positionX2 = testActor2->GetTransform().GetWorldPosition().x;
	float positionY2 = testActor2->GetTransform().GetWorldPosition().y;
	float positionZ2 = testActor2->GetTransform().GetWorldPosition().z;

	float positionX3 = testActor3->GetTransform().GetWorldPosition().x;
	float positionY3 = testActor3->GetTransform().GetWorldPosition().y;
	float positionZ3 = testActor3->GetTransform().GetWorldPosition().z;

	while (IsRunning())
	{
		m_editor.PreUpdate();

		ImGui::Begin("Transform Test");
		ImGui::SliderFloat("Position X", &positionX, 0.0f, 100.0f);
		ImGui::SliderFloat("Position Y", &positionY, 0.0f, 100.0f);
		ImGui::SliderFloat("Position Z", &positionZ, 0.0f, 100.0f);

		ImGui::SliderFloat("Position X2", &positionX2, 0.0f, 100.0f);
		ImGui::SliderFloat("Position Y2", &positionY2, 0.0f, 100.0f);
		ImGui::SliderFloat("Position Z2", &positionZ2, 0.0f, 100.0f);

		ImGui::SliderFloat("Position X3", &positionX3, 0.0f, 100.0f);
		ImGui::SliderFloat("Position Y3", &positionY3, 0.0f, 100.0f);
		ImGui::SliderFloat("Position Z3", &positionZ3, 0.0f, 100.0f);
		ImGui::End();

		if(testActor)
			testActor->GetTransform().SetLocalPosition({ positionX, positionY, positionZ });

		if(testActor2)
			testActor2->GetTransform().SetLocalPosition({ positionX2, positionY2, positionZ2 });

		if(testActor3)
			testActor3->GetTransform().SetLocalPosition({ positionX3, positionY3, positionZ3 });

		if(m_context.inputManager->IsKeyPressed(AmberEngine::Inputs::EKey::KEY_F))
		{
			m_editor.m_context.m_scene.DestroyActor(testActor2);
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

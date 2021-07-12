#include "pch.h"

#include "ExampleLayer.h"

#define VOXELIZER_IMPLEMENTATION
#include <AmberEngine/Tools/Utils/voxelizer.h>

#include <AmberEngine/Buffers/VertexArray.h>
#include <AmberEngine/Buffers/VertexBuffer.h>
#include <AmberEngine/Buffers/VertexBufferLayout.h>
#include <AmberEngine/Resources/Shader.h>
#include <AmberEngine/Resources/AssimpModel.h>
#include <AmberEngine/Resources/Primitives/Cube.h>
#include <AmberEngine/ImGUI/imgui.h>
#include <AmberEngine/Core/UIManager.h>
#include <AmberEngine/Tools/Utils/String.h>

ExampleLayer::ExampleLayer(AmberEngine::Core::Context& p_context, AmberEngine::Core::Editor& p_editor) : ALayer(p_context, p_editor, "Demo"),ui(*p_context.m_window)
{
}

void ExampleLayer::OnStart()
{
	m_context.m_resourcesManager.LoadModel("Suit", "res/Mesh/nanosuit/nanosuit.obj");

	AmberEngine::Resources::Shader& lightingShader = m_context.m_resourcesManager.LoadShader("StandardLighting", "res/shaders/StandardLighting.glsl");
	AmberEngine::Resources::Shader& depthShader = m_context.m_resourcesManager.LoadShader("Depth", "res/shaders/DepthVisualization.glsl");
	//m_context.m_resourcesManager.LoadTexture("diffuse", "crystal.jpg");
	//m_context.m_resourcesManager.LoadTexture("specular", "crystal_spec.jpg");
	depthShader.Bind();
	depthShader.Unbind();
	lightingShader.Bind();
	lightingShader.SetUniform1i("u_DiffuseMap", 0);
	lightingShader.SetUniform1i("u_SpecularMap", 1);
	lightingShader.SetUniformVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	lightingShader.SetUniformVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	lightingShader.SetUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	lightingShader.Unbind();

	// Playing with indexation
	//-------------------------------------
	glm::vec3 verticesVec3[] =
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
	float vertexBuffer[108]{0.0f};
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
	}

	glm::vec3 vertexBufferGLM[36]{ glm::vec3{{0.0f}} };
	for (int i = 0; i < 36; i++) 
	{
		vertexBufferGLM[i].x = verticesVec3[indices[i]].x;
		vertexBufferGLM[i].y = verticesVec3[indices[i]].y;
		vertexBufferGLM[i].z = verticesVec3[indices[i]].z;
	}

	float vertexBufferTest[108]{0.0f};
	for (int i = 0; i < 36; i++) {

		int indexX = indices[i];
		int indexY = indices[i];
		int indexZ = indices[i];

		float verticeX = verticesVec3[indexX].x;
		float verticeY = verticesVec3[indexY].y;
		float verticeZ = verticesVec3[indexZ].z;

		vertexBufferTest[i * 3 + 0] = verticeX;
		vertexBufferTest[i * 3 + 1] = verticeY;
		vertexBufferTest[i * 3 + 2] = verticeZ;
	}

	for (int i = 0; i < 36; i++)
	{
		if (vertexBuffer[i * 3 + 0] != vertexBufferGLM[i].x || vertexBufferTest[i * 3 + 0] != vertexBufferGLM[i].x)
		{
			std::cout << "error" << std::endl;
		}
		if (vertexBuffer[i * 3 + 1] != vertexBufferGLM[i].y || vertexBufferTest[i * 3 + 1] != vertexBufferGLM[i].y)
		{
			std::cout << "error" << std::endl;
		}
		if (vertexBuffer[i * 3 + 2] != vertexBufferGLM[i].z || vertexBufferTest[i * 3 + 2] != vertexBufferGLM[i].z)
		{
			std::cout << "error" << std::endl;
		}
	}

	//-------------------------------------

	//Quick and Dirty Test for Cube PrimitivesShapes.
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
	AmberEngine::Buffers::VertexBuffer vbo(vertices.data(), vertices.size() * sizeof(vertices));
	AmberEngine::Buffers::VertexBufferLayout layout;

	layout.PushFloat(3);
	layout.PushFloat(2);
	layout.PushFloat(3);

	vao.AddBuffer(vbo, layout);
	vao.Unbind();
	vbo.Unbind();*/

	// Quick and Dirty Test Voxelizer.
	/*float precision = 0.01f;

	for (size_t i = 0; i < model->m_meshes.size(); i++)
	{
		AmberEngine::Resources::AssimpMesh currentMesh = model->m_meshes[i];

		vx_mesh_t* vxMesh = vx_mesh_alloc(currentMesh.m_vertices.size(), currentMesh.m_indices.size());

		for (size_t j = 0; j < currentMesh.m_indices.size(); j++)
		{
			vxMesh->indices[j] = currentMesh.m_indices[j];
		}

		for (size_t k = 0; k < currentMesh.m_vertices.size(); k++)
		{
			vxMesh->vertices[k].x = currentMesh.m_vertices[k].position.x;
			vxMesh->vertices[k].y = currentMesh.m_vertices[k].position.y;
			vxMesh->vertices[k].z = currentMesh.m_vertices[k].position.z;
		}

		vx_mesh_t* voxelizedMesh = vx_voxelize(vxMesh, 0.025f, 0.025f, 0.025f, precision);

		meshIndices.push_back(voxelizedMesh->nindices);

		unsigned int vao;
		unsigned int vbo;
		unsigned int ebo;

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, voxelizedMesh->nvertices * sizeof(vx_vertex_t), &voxelizedMesh->vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, voxelizedMesh->nindices * sizeof(unsigned int), &voxelizedMesh->indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vx_vertex_t), static_cast<void*>(nullptr));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vx_color_t), reinterpret_cast<void*>(offsetof(vx_mesh_t, colors)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vx_vec3_t), reinterpret_cast<void*>(offsetof(vx_mesh_t, normals)));
		glBindVertexArray(0);

		vertexArrays.push_back(vao);

		delete vxMesh;
		delete voxelizedMesh;
	}*/
}

void ExampleLayer::OnUpdate(float p_deltaTime)
{
	// TODO: Create Demo Scenes, cleaning all these tests.
	glm::vec3 cameraPosition = m_editor.GetCameraController().GetPosition();

	ui.BeginFrame();

	ui.DisplayDeviceInfos();
	ui.DisplayMenu();
	ui.BeginWindow("Scene");
	ImGui::Text("Camera Position X: %.1f Y: %.1f Z: %.1f", cameraPosition.x, cameraPosition.y, cameraPosition.z);
	ImGui::DragFloat("Light Direction X", &lighDir.x, 0.005f, 0.0f, 0.0f, "X: %.1f");
	ImGui::DragFloat("Light Direction Y", &lighDir.y, 0.005f, 0.0f, 0.0f, "Y: %.1f");
	ImGui::DragFloat("Light Direction Z", &lighDir.z, 0.005f, 0.0f, 0.0f, "Z: %.1f");
	ui.EndWindow();

	glm::mat4 projectionMatrix = m_editor.GetCameraController().GetCamera().GetProjectionMatrix();
	glm::mat4 viewMatrix       = m_editor.GetCameraController().GetCamera().GetViewMatrix();
	glm::mat4 modelMatrix      = glm::mat4(1.0f);
	auto& shader = m_context.m_resourcesManager.GetShader("StandardLighting");
	auto& shaderDepth = m_context.m_resourcesManager.GetShader("Depth");
	if (m_context.m_inputManager->IsKeyPressed(AmberEngine::Inputs::EKey::KEY_B))
	{
		isDepth = !isDepth;
		std::cout << "ok pelo" << std::endl;
	}


	if (isDepth)
	{
		shaderDepth.Bind();
		shaderDepth.SetUniformMat4("projection", projectionMatrix);
		shaderDepth.SetUniformMat4("view", viewMatrix);
		shaderDepth.SetUniformMat4("model", modelMatrix);
		shaderDepth.Unbind();
	}
	else
	{
		shader.Bind();
		shader.SetUniformMat4("projection", projectionMatrix);
		shader.SetUniformMat4("view", viewMatrix);
		shader.SetUniformMat4("model", modelMatrix);
		shader.SetUniformVec3("viewPos", cameraPosition);
		shader.SetUniformVec3("light.direction", lighDir);
		shader.Unbind();
	}
	
	//m_context.m_resourcesManager.GetTexture("diffuse").Bind();
	//m_context.m_resourcesManager.GetTexture("specular").Bind(1);

	m_context.m_renderer->DrawModelWithShader(m_context.m_resourcesManager.GetModel("Suit"), isDepth ? shaderDepth : shader);
	
	//for (size_t i = 0; i < model->m_meshes.size(); i++)
	//{
	//	glBindVertexArray(vertexArrays[i]);
	//	glDrawElements(GL_TRIANGLES, meshIndices[i], GL_UNSIGNED_INT, 0);
	//	glBindVertexArray(0);
	//}

	shaderDepth.Unbind();
	shader.Unbind();

	ui.EndFrame();
	ui.Render();
}

void ExampleLayer::OnDestroy()
{

}

#include "pch.h"

#include "Application.h"
#include <AmberEngine/Buffers/VertexArray.h>
#include <AmberEngine/Buffers/VertexBuffer.h>
#include <AmberEngine/Buffers/VertexBufferLayout.h>
#include <AmberEngine/Resources/Shader.h>
#include <AmberEngine/Resources/AssimpModel.h>
#include <AmberEngine/Resources/Primitives/Cube.h>
#include <AmberEngine/ImGUI/imgui.h>
#include <AmberEngine/Managers/UIManager.h>

Example::Application::Application(const AmberEngine::Settings::DeviceSettings& p_deviceSettings, const AmberEngine::Settings::WindowSettings& p_windowSettings, const AmberEngine::Settings::DriverSettings& p_driverSettings) : m_renderer(p_deviceSettings, p_windowSettings, p_driverSettings)
{
}

void Example::Application::Setup()
{
	AmberEngine::Resources::Shader& lightingShader = m_renderer.GetResourcesManager().LoadShader("StandardLighting", "StandardLighting.glsl");
	lightingShader.Bind();
	lightingShader.SetUniform1i("u_DiffuseMap", 0);
	lightingShader.SetUniform1i("u_SpecularMap", 1);
	lightingShader.SetUniformVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	lightingShader.SetUniformVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	lightingShader.SetUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	lightingShader.Unbind();
}

void Example::Application::Run()
{
	glm::vec3 lighDir = glm::vec3(-0.2f, -1.0f, -0.3f);

	AmberEngine::Resources::AssimpModel model("res/Mesh/nanosuit/nanosuit.obj");

	AmberEngine::Managers::UIManager ui(m_renderer.GetWindow());
	
	//AmberEngine::PrimitivesShapes::Cube::Setup();
	//
	//std::vector<float> vertices;
	//std::vector<AmberEngine::PrimitivesShapes::Vertex> verticesCube = AmberEngine::PrimitivesShapes::Cube::GetVertices();
	//
	//for (auto& vertex : verticesCube)
	//{
	//	vertices.push_back(vertex.position[0]);
	//	vertices.push_back(vertex.position[1]);
	//	vertices.push_back(vertex.position[2]);
	//	vertices.push_back(vertex.textureCoord[0]);
	//	vertices.push_back(vertex.textureCoord[1]);
	//	vertices.push_back(vertex.normals[0]);
	//	vertices.push_back(vertex.normals[1]);
	//	vertices.push_back(vertex.normals[2]);
	//}
	//
	//AmberEngine::Buffers::VertexArray vao;
	//AmberEngine::Buffers::VertexBuffer vbo(vertices.data(), vertices.size() * sizeof(vertices));
	//AmberEngine::Buffers::VertexBufferLayout layout;
	//
	//layout.PushFloat(3);
	//layout.PushFloat(2);
	//layout.PushFloat(3);
	//
	//vao.AddBuffer(vbo, layout);
	//vao.Unbind();
	//vbo.Unbind();
	
	while (m_renderer.IsRunning())
	{
		m_renderer.SetClearColor(0.1f, 0.1f, 0.1f);
		m_renderer.Clear(true, true, false);
		m_renderer.Update();

		glm::vec3 cameraPosition = m_renderer.GetCameraController().GetCamera().GetPosition();

		ui.BeginFrame();

		ui.DisplayDeviceInfos();
		ui.DisplayMenu();
		ui.BeginWindow("Scene");
		ImGui::Text("Camera Position X: %.1f Y: %.1f Z: %.1f", cameraPosition.x, cameraPosition.y, cameraPosition.z);
		ImGui::DragFloat("Light Direction X", &lighDir.x, 0.005f, 0.0f, 0.0f, "X: %.1f");
		ImGui::DragFloat("Light Direction Y", &lighDir.y, 0.005f, 0.0f, 0.0f, "Y: %.1f");
		ImGui::DragFloat("Light Direction Z", &lighDir.z, 0.005f, 0.0f, 0.0f, "Z: %.1f");
		ui.EndWindow();

		glm::mat4 projectionMatrix = m_renderer.CalculateProjectionMatrix();
		glm::mat4 viewMatrix = m_renderer.CalculateViewMatrix();
		glm::mat4 modelMatrix = m_renderer.GetUnitModelMatrix();

		m_renderer.GetResourcesManager().GetShader("StandardLighting").Bind();
		m_renderer.GetResourcesManager().GetShader("StandardLighting").SetUniformMat4("projection", projectionMatrix);
		m_renderer.GetResourcesManager().GetShader("StandardLighting").SetUniformMat4("view", viewMatrix);
		m_renderer.GetResourcesManager().GetShader("StandardLighting").SetUniformMat4("model", modelMatrix);
		m_renderer.GetResourcesManager().GetShader("StandardLighting").SetUniformVec3("viewPos", cameraPosition);
		m_renderer.GetResourcesManager().GetShader("StandardLighting").SetUniformVec3("light.direction", lighDir);
		
		model.Draw(m_renderer.GetResourcesManager().GetShader("StandardLighting"));
		
		//vao.Bind();
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		
		ui.EndFrame();
		ui.Render();

		m_renderer.SwapBuffers();
	}
}
#include "Amberpch.h"

#include "AmberEngine/Core/EditorRenderer.h"

#include "AmberEngine/Managers/ResourcesManager.h"

AmberEngine::Core::EditorRenderer::EditorRenderer(Context& p_context) : m_context(p_context)
{
	m_context.renderer->RegisterModelMatrixSender([this](const glm::mat4& p_modelMatrix)
	{
		m_context.engineUBO->SetSubData(p_modelMatrix, 0);
	});

	InitMaterials();
}

void AmberEngine::Core::EditorRenderer::InitMaterials()
{
	Managers::ResourcesManager::Instance().LoadShader("Standard", ":Shaders\\Standard.glsl");
	Managers::ResourcesManager::Instance().LoadShader("Lambert", ":Shaders\\Lambert.glsl");

	m_defaultMaterial.SetShader(&Managers::ResourcesManager::Instance().GetShader("Standard"));
	m_defaultMaterial.SetUniform("u_Diffuse", glm::vec4(1.f, 1.f, 1.f, 1.f));
	m_defaultMaterial.SetUniform("u_Shininess", 100.0f);
	m_defaultMaterial.SetUniform<Resources::Texture*>("u_DiffuseMap", nullptr);

	m_emptyMaterial.SetShader(&Managers::ResourcesManager::Instance().GetShader("Standard"));
	m_emptyMaterial.SetUniform("u_Diffuse", glm::vec4(1.f, 0.f, 1.f, 1.0f));
	m_emptyMaterial.SetUniform<Resources::Texture*>("u_DiffuseMap", nullptr);
}

void AmberEngine::Core::EditorRenderer::RenderScene(const glm::vec3& p_cameraPosition,
	const LowRenderer::Camera& p_camera)
{
	m_context.lightSSBO->Bind(0);
	m_context.renderer->RenderScene(*m_context.sceneManager.GetCurrentScene(), &m_emptyMaterial);
	m_context.lightSSBO->Unbind();
}

void AmberEngine::Core::EditorRenderer::RenderUI()
{
	m_context.uiManager->Render();
}

void AmberEngine::Core::EditorRenderer::UpdateLights(SceneSystem::Scene& p_scene)
{
	auto lightMatrices = m_context.renderer->FindLightMatrices(p_scene);
	m_context.lightSSBO->SendBlocks<glm::mat4>(lightMatrices.data(), lightMatrices.size() * sizeof(glm::mat4));
}

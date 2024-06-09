#include "Amberpch.h"

#include "AmberEngine/Core/EditorRenderer.h"

#include "AmberEngine/Core/ECS/Actor.h"
#include "AmberEngine/Core/ECS/Components/CModelRenderer.h"
#include "AmberEngine/Core/ECS/Components/CLight.h"

#include "AmberEngine/Managers/ShaderManager.h"

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
	m_defaultMaterial.SetShader(m_context.shaderManager[":Shaders\\Standard.glsl"]);
	m_defaultMaterial.SetUniform("u_Diffuse", glm::vec4(1.f, 1.f, 1.f, 1.f));
	m_defaultMaterial.SetUniform("u_Shininess", 100.0f);
	m_defaultMaterial.SetUniform<Resources::Texture*>("u_DiffuseMap", nullptr);

	m_emptyMaterial.SetShader(m_context.shaderManager[":Shaders\\Unlit.glsl"]);
	m_emptyMaterial.SetUniform("u_Diffuse", glm::vec4(1.f, 0.f, 1.f, 1.0f));
	m_emptyMaterial.SetUniform<Resources::Texture*>("u_DiffuseMap", nullptr);
}

void AmberEngine::Core::EditorRenderer::RenderScene(const glm::vec3& p_cameraPosition, const Rendering::Entities::Camera& p_camera)
{
	m_context.lightSSBO->Bind(0);

	m_context.renderer->RenderScene(*m_context.sceneManager.GetCurrentScene(), p_cameraPosition, p_camera, &m_emptyMaterial);

	m_context.lightSSBO->Unbind();
}

void AmberEngine::Core::EditorRenderer::RenderUI() const
{
	m_context.uiManager->Render();
}

void AmberEngine::Core::EditorRenderer::UpdateLights(SceneSystem::Scene& p_scene) const
{
	auto lightMatrices = m_context.renderer->FindLightMatrices(p_scene);
	m_context.lightSSBO->SendBlocks<glm::mat4>(lightMatrices.data(), lightMatrices.size() * sizeof(glm::mat4));
}


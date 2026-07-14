#include "Amberpch.h"

#include "AmberGame/Core/GameRenderer.h"

#include "AmberCore/ECS/Actor.h"
#include "AmberCore/SceneSystem/Scene.h"
#include "AmberRendering/Resources/Texture.h"
#include "AmberTools/Analytics/Profiling/ProfilerSpy.h"

AmberGame::Core::GameRenderer::GameRenderer(Context& p_context) :
	m_context(p_context)
{
	m_emptyMaterial.SetShader(m_context.shaderManager[":Shaders\\Unlit.glsl"]);
	m_emptyMaterial.SetUniform("u_Diffuse", glm::vec4(1.f, 0.f, 1.f, 1.f));
	m_emptyMaterial.SetUniform<AmberRendering::Resources::Texture*>("u_DiffuseMap", nullptr);

	m_context.renderer->RegisterModelMatrixSender([this](const glm::mat4& p_modelMatrix)
	{
		m_context.engineUBO->SetSubData(p_modelMatrix, 0);
	});

	m_context.renderer->RegisterUserMatrixSender([this](const glm::mat4& p_userMatrix)
	{
		m_context.engineUBO->SetSubData
		(
			p_userMatrix,

			// UBO layout offset
			sizeof(glm::mat4) +
			sizeof(glm::mat4) +
			sizeof(glm::mat4) +
			sizeof(glm::vec3) +
			sizeof(float)
		);
	});
}

void AmberGame::Core::GameRenderer::RenderScene()
{
	if (auto currentScene = m_context.sceneManager.GetCurrentScene())
	{
		if (AmberCore::ECS::Components::CCamera* mainCameraComponent = m_context.renderer->FindMainCamera(*currentScene))
		{
			UpdateLights(*currentScene);

			auto [winWidth, winHeight] = m_context.window->GetSize();

			if (winWidth == 0 || winHeight == 0)
				return;

			const glm::vec3& cameraPosition = mainCameraComponent->Owner.transform.GetWorldPosition();
			const glm::quat& cameraRotation = mainCameraComponent->Owner.transform.GetWorldRotation();
			auto& camera = mainCameraComponent->GetCamera();

			camera.ComputeMatrices(winWidth, winHeight, cameraPosition, cameraRotation);

			UpdateEngineUBO(*mainCameraComponent);

			m_context.renderer->Clear(camera, true, true, false);

			uint8_t glState = m_context.renderer->FetchGLState();
			m_context.renderer->ApplyStateMask(glState);
			m_context.renderer->RenderScene(*currentScene, cameraPosition, camera, &m_emptyMaterial);
			m_context.renderer->ApplyStateMask(glState);
		}
		else
		{
			m_context.renderer->SetClearColor(0.0f, 0.0f, 0.0f);
			m_context.renderer->Clear(true, true, false);
		}
	}
}

void AmberGame::Core::GameRenderer::UpdateEngineUBO(AmberCore::ECS::Components::CCamera& p_mainCamera)
{
	size_t offset = sizeof(glm::mat4);
	auto& camera = p_mainCamera.GetCamera();

	m_context.engineUBO->SetSubData(camera.GetViewMatrix(), offset);
	m_context.engineUBO->SetSubData(camera.GetProjectionMatrix(), offset);
	m_context.engineUBO->SetSubData(p_mainCamera.Owner.transform.GetWorldPosition(), offset);
}

void AmberGame::Core::GameRenderer::UpdateLights(AmberCore::SceneSystem::Scene& p_scene)
{
	PROFILER_SPY("Light SSBO Update");
	std::vector<glm::mat4> lightMatrices = m_context.renderer->FindLightMatrices(p_scene);
	m_context.lightSSBO->SendBlocks<glm::mat4>(lightMatrices.data(), lightMatrices.size() * sizeof(glm::mat4));
}

#include "Amberpch.h"

#include "AmberEngine/Core/Editor.h"

#include "AmberEngine/Core/ECS/Components/ModelComponent.h"

AmberEngine::Core::Editor::Editor(Context& p_context) :
	m_context(p_context),
	m_sceneView(m_context),
	m_hierarchy("Hierarchy"),
	m_menuBar(m_context)
{
	m_context.renderer->RegisterModelMatrixSender([this](const glm::mat4& p_modelMatrix)
	{
		m_context.engineUBO->SetSubData(p_modelMatrix, 0);
	});

	m_context.uiManager->EnableDocking(true);

	m_menuBar.NormalsColorsVisualizationCallback = Eventing::QuickBind(&Editor::OnNormalsColorsVisualization, this);
}

void AmberEngine::Core::Editor::PreUpdate() const
{
	m_context.device->PollEvents();

	m_context.renderer->SetClearColor(0.1f, 0.1f, 0.1f);
	m_context.renderer->Clear(true, true, true);

	m_context.uiManager->PreDraw();
}

void AmberEngine::Core::Editor::Update(float p_deltaTime)
{
	m_context.lightSSBO->Bind(0);
	RenderViews(p_deltaTime);
	m_context.lightSSBO->Unbind();
	HandleInput();
}

void AmberEngine::Core::Editor::PostUpdate() const
{
	m_context.window->SwapBuffers();
	m_context.inputManager->clearEvents();
}

void AmberEngine::Core::Editor::UpdateLights(SceneSystem::Scene& p_scene) const
{
	auto lightMatrices = m_context.renderer->FindLightMatrices(p_scene);
	m_context.lightSSBO->SendBlocks<glm::mat4>(lightMatrices.data(), lightMatrices.size() * sizeof(glm::mat4));
}

void AmberEngine::Core::Editor::RenderViews(float p_deltaTime)
{
	UpdateLights(m_context.m_scene);
	m_sceneView.Update(p_deltaTime);
	m_sceneView.Render();

	m_sceneView.Draw();
	m_hierarchy.Draw();
	m_menuBar.Draw();
	m_context.uiManager->PostDraw();
}

void AmberEngine::Core::Editor::HandleInput() const
{
	if (m_context.inputManager->IsKeyPressed(Inputs::EKey::KEY_ESCAPE))
		m_context.window->SetShouldClose(true);
}

void AmberEngine::Core::Editor::OnNormalsColorsVisualization(bool p_value)
{
	if (isNormalsColorsVisualization == p_value)
		return;

	if(p_value)
	{
		EnableNormalVisualization();
		isNormalsColorsVisualization = true;
	}
	else
	{
		DisableNormalVisualization();
		isNormalsColorsVisualization = false;
	}
}

void AmberEngine::Core::Editor::EnableNormalVisualization() const
{
	for (const auto& actor : m_context.m_scene.GetActors())
	{
		if (const auto model = actor.second->GetComponent<ECS::Components::ModelComponent>())
		{
			for (const auto mesh : model->GetMeshes())
			{
				model->GetModel()->GetMaterials()[mesh->GetMaterialIndex()]->SetShader(*m_context.m_editorResources->GetShader("NormalsColors"));
			}
		}
	}
}

void AmberEngine::Core::Editor::DisableNormalVisualization() const
{
	for (const auto& actor : m_context.m_scene.GetActors())
	{
		if (const auto model = actor.second->GetComponent<ECS::Components::ModelComponent>())
		{
			for (const auto mesh : model->GetMeshes())
			{
				model->GetModel()->GetMaterials()[mesh->GetMaterialIndex()]->ResetToPreviousShader();
			}
		}
	}
}

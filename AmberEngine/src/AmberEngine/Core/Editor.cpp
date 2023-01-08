#include "Amberpch.h"

#include "AmberEngine/Core/Editor.h"

#include "AmberEngine/Core/ECS/Components/ModelComponent.h"
#include "AmberEngine/Data/Constants.h"
#include "AmberEngine/Managers/ResourcesManager.h"
#include "AmberEngine/Tools/Utils/ServiceLocator.h"

AmberEngine::Core::Editor::Editor(Context& p_context) : m_context(p_context)
{
	m_context.renderer->RegisterModelMatrixSender([this](const glm::mat4& p_modelMatrix)
	{
		m_context.engineUBO->SetSubData(p_modelMatrix, 0);
	});

	m_sceneView = std::make_unique<UI::SceneView>(Data::Constants::EDITOR_PANEL_SCENE_NAME, true);

	InitMaterials();
	InitializeUI();

	Utils::ServiceLocator::Provide(*this);
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
	m_context.m_scene.Update(p_deltaTime);
	RenderViews(p_deltaTime);
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
	m_sceneView->Update(p_deltaTime);
	
	m_sceneView->Render();
	m_sceneView->Draw();

	m_hierarchy.Draw();
	m_menuBar.Draw();

	m_context.uiManager->PostDraw();
}

void AmberEngine::Core::Editor::RenderScene()
{
	UpdateLights(m_context.m_scene);
	m_context.lightSSBO->Bind(0);
	m_context.m_scene.DrawAll(*m_context.renderer, &m_defaultMaterial);
	m_context.lightSSBO->Unbind();
}

void AmberEngine::Core::Editor::HandleInput() const
{
	if (m_context.inputManager->IsKeyPressed(Inputs::EKey::KEY_ESCAPE))
		m_context.window->SetShouldClose(true);
}

void AmberEngine::Core::Editor::InitMaterials()
{
	Managers::ResourcesManager::Instance().LoadShader("Standard", ":Shaders\\Standard.glsl");
	Managers::ResourcesManager::Instance().LoadShader("Lambert", ":Shaders\\Lambert.glsl");
	
	m_defaultMaterial.SetShader(&Managers::ResourcesManager::Instance().GetShader("Standard"));
}

void AmberEngine::Core::Editor::InitializeUI()
{
	m_context.uiManager->EnableDocking(true);

	m_menuBar.NormalsColorsVisualizationCallback = Eventing::QuickBind(&Editor::OnNormalsColorsVisualization, this);
}

void AmberEngine::Core::Editor::OnNormalsColorsVisualization(bool p_value)
{
	if (isNormalsColorsVisualization == p_value)
		return;

	isNormalsColorsVisualization = p_value;

	ToggleNormalVisualization();
}

void AmberEngine::Core::Editor::ToggleNormalVisualization() const
{
	for (const auto& actor : m_context.m_scene.GetActors())
	{
		if (const auto model = actor.second->GetComponent<ECS::Components::ModelComponent>())
		{
			for (const auto mesh : model->GetMeshes())
			{
				if(isNormalsColorsVisualization)
				{
					model->GetModel()->GetMaterials()[mesh->GetMaterialIndex()]->SetShader(m_context.m_editorResources->GetShader("NormalsColors"));
				}
				else
				{
					model->GetModel()->GetMaterials()[mesh->GetMaterialIndex()]->ResetToPreviousShader();
				}
			}
		}
	}
}
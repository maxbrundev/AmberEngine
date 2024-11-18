#include "Amberpch.h"

#include "AmberEngine/Core/EditorRenderer.h"

#include "AmberEngine/Analytics/Profiling/Profiler.h"
#include "AmberEngine/Analytics/Profiling/ProfilerSpy.h"
#include "AmberEngine/Core/EditorAction.h"
#include "AmberEngine/Core/ShapeDrawer.h"
#include "AmberEngine/Core/ECS/Actor.h"
#include "AmberEngine/Core/ECS/Components/CModelRenderer.h"
#include "AmberEngine/Core/ECS/Components/CLight.h"
#include "AmberEngine/Core/ECS/Components/CMaterialRenderer.h"
#include "AmberEngine/Core/ECS/Components/CPhysicalBox.h"
#include "AmberEngine/Core/ECS/Components/CPhysicalCapsule.h"
#include "AmberEngine/Core/ECS/Components/CPhysicalObject.h"
#include "AmberEngine/Core/ECS/Components/CPhysicalSphere.h"
#include "AmberEngine/Core/SceneSystem/Scene.h"

#include "AmberEngine/Managers/ShaderManager.h"
#include "AmberEngine/UI/Panels/Views/GameView.h"
#include "AmberEngine/UI/Panels/Views/SceneView.h"

const glm::vec3 LIGHT_VOLUME_COLOR = { 1.0f, 1.0f, 0.0f };
const glm::vec3 FRUSTUM_COLOR = { 1.0f, 1.0f, 1.0f };
const  glm::vec3 DEBUG_BOUNDS_COLOR = { 1.0f, 0.0f, 0.0f };

AmberEngine::Core::EditorRenderer::EditorRenderer(Context& p_context) : m_context(p_context)
{
	m_context.driver->SetCapability(Rendering::Settings::ERenderingCapability::STENCIL_TEST, true);
	m_context.driver->SetStencilOperations(Rendering::Settings::EOperation::KEEP, Rendering::Settings::EOperation::KEEP, Rendering::Settings::EOperation::REPLACE);
	m_context.driver->SetStencilAlgorithm(Rendering::Settings::EComparisonAlgorithm::ALWAYS, 1, 0xFF);

	InitMaterials();

	m_context.renderer->RegisterModelMatrixSender([this](const glm::mat4& p_modelMatrix)
	{
		m_context.engineUBO->SetSubData(p_modelMatrix, 0);
	});

	m_context.renderer->RegisterUserMatrixSender([this](const glm::mat4 & p_userMatrix)
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

void AmberEngine::Core::EditorRenderer::InitMaterials()
{
	m_defaultMaterial.SetShader(m_context.shaderManager[":Shaders\\Standard.glsl"]);
	m_defaultMaterial.SetUniform("u_Diffuse", glm::vec4(1.f, 1.f, 1.f, 1.f));
	m_defaultMaterial.SetUniform("u_Shininess", 100.0f);
	m_defaultMaterial.SetUniform<Resources::Texture*>("u_DiffuseMap", nullptr);

	m_emptyMaterial.SetShader(m_context.shaderManager[":Shaders\\Unlit.glsl"]);
	m_emptyMaterial.SetUniform("u_Diffuse", glm::vec4(1.f, 0.f, 1.f, 1.0f));
	m_emptyMaterial.SetUniform<Resources::Texture*>("u_DiffuseMap", nullptr);

	m_gridMaterial.SetShader(m_context.editorResources->GetShader("Grid"));
	m_gridMaterial.SetBlendable(true);
	m_gridMaterial.SetBackFaceCulling(false);
	m_gridMaterial.SetDepthTest(false);

	m_cameraMaterial.SetShader(m_context.shaderManager[":Shaders\\Lambert.glsl"]);
	m_cameraMaterial.SetUniform("u_Diffuse", glm::vec4(0.0f, 0.3f, 0.7f, 1.0f));
	m_cameraMaterial.SetUniform<Resources::Texture*>("u_DiffuseMap", nullptr);

	m_textureMaterial.SetShader(m_context.shaderManager[":Shaders\\Unlit.glsl"]);
	m_textureMaterial.SetUniform("u_Diffuse", glm::vec4(1.f, 1.f, 1.f, 1.f));
	m_textureMaterial.SetBackFaceCulling(false);
	m_textureMaterial.SetBlendable(true);
	m_textureMaterial.SetUniform<Resources::Texture*>("u_DiffuseMap", nullptr);

	m_outlineMaterial.SetShader(m_context.shaderManager[":Shaders\\Unlit.glsl"]);
	m_outlineMaterial.SetUniform<Resources::Texture*>("u_DiffuseMap", nullptr);
	m_outlineMaterial.SetDepthTest(false);

	m_stencilFillMaterial.SetShader(m_context.shaderManager[":Shaders\\Unlit.glsl"]);
	m_stencilFillMaterial.SetBackFaceCulling(true);
	m_stencilFillMaterial.SetDepthTest(false);
	m_stencilFillMaterial.SetColorWriting(false);
	m_stencilFillMaterial.SetUniform<Resources::Texture*>("u_DiffuseMap", nullptr);

	m_actorPickingMaterial.SetShader(m_context.shaderManager[":Shaders\\Unlit.glsl"]);
	m_actorPickingMaterial.SetUniform("u_Diffuse", glm::vec4(1.f, 1.f, 1.f, 1.0f));
	m_actorPickingMaterial.SetUniform<Resources::Texture*>("u_DiffuseMap", nullptr);
	m_actorPickingMaterial.SetFrontFaceCulling(false);
	m_actorPickingMaterial.SetBackFaceCulling(false);

	m_gizmoArrowMaterial.SetShader(m_context.editorResources->GetShader("Gizmo"));
	m_gizmoArrowMaterial.SetGPUInstances(3);
	m_gizmoArrowMaterial.SetUniform("u_IsBall", false);
	m_gizmoArrowMaterial.SetUniform("u_IsPickable", false);

	/* Gizmo Ball Material */
	m_gizmoBallMaterial.SetShader(m_context.editorResources->GetShader("Gizmo"));
	m_gizmoBallMaterial.SetUniform("u_IsBall", true);
	m_gizmoBallMaterial.SetUniform("u_IsPickable", false);

	/* Gizmo Pickable Material */
	m_gizmoPickingMaterial.SetShader(m_context.editorResources->GetShader("Gizmo"));
	m_gizmoPickingMaterial.SetGPUInstances(3);
	m_gizmoPickingMaterial.SetUniform("u_IsBall", false);
	m_gizmoPickingMaterial.SetUniform("u_IsPickable", true);
}

void AmberEngine::Core::EditorRenderer::PreparePickingMaterial(ECS::Actor& p_actor, Resources::Material& p_material)
{
	uint32_t actorID = static_cast<uint32_t>(p_actor.GetID());

	auto bytes = reinterpret_cast<uint8_t*>(&actorID);
	auto color = glm::vec4{ bytes[0] / 255.0f, bytes[1] / 255.0f, bytes[2] / 255.0f, 1.0f };

	p_material.SetUniform("u_Diffuse", color);
}

void AmberEngine::Core::EditorRenderer::RenderSceneForActorPicking()
{
	auto& scene = *m_context.sceneManager.GetCurrentScene();

	/* Render models */
	for (auto modelRenderer : scene.GetFastAccessComponents().modelRenderers)
	{
		auto& actor = modelRenderer->Owner;

		if (actor.IsActive())
		{
			if (auto model = modelRenderer->GetModel())
			{
				if (auto materialRenderer = modelRenderer->Owner.GetComponent<ECS::Components::CMaterialRenderer>())
				{
					const ECS::Components::CMaterialRenderer::MaterialList& materials = materialRenderer->GetMaterials();
					const auto& modelMatrix = actor.transform.GetWorldMatrix();

					PreparePickingMaterial(actor, m_actorPickingMaterial);

					for (auto mesh : model->GetMeshes())
					{
						Resources::Material* material = nullptr;

						if (mesh->GetMaterialIndex() < MAX_MATERIAL_COUNT)
						{
							material = materials.at(mesh->GetMaterialIndex());
							if (!material || !material->GetShader())
								material = &m_emptyMaterial;
						}

						if (material)
						{
							m_actorPickingMaterial.SetBackFaceCulling(material->HasBackFaceCulling());
							m_actorPickingMaterial.SetFrontFaceCulling(material->HasFrontFaceCulling());
							m_actorPickingMaterial.SetColorWriting(material->HasColorWriting());
							m_actorPickingMaterial.SetDepthTest(material->HasDepthTest());
							m_actorPickingMaterial.SetDepthWriting(material->HasDepthWriting());

							m_context.renderer->DrawMesh(*mesh, m_actorPickingMaterial, &modelMatrix);
						}
					}
				}
			}
		}
	}

	for (auto camera : m_context.sceneManager.GetCurrentScene()->GetFastAccessComponents().cameras)
	{
		auto& actor = camera->Owner;

		if (actor.IsActive())
		{
			PreparePickingMaterial(actor, m_actorPickingMaterial);
			auto& model = *m_context.editorResources->GetModel("Camera");
			auto modelMatrix = CalculateCameraModelMatrix(actor);

			m_context.renderer->DrawModelWithSingleMaterial(model, m_actorPickingMaterial, &modelMatrix);
		}
	}
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

void AmberEngine::Core::EditorRenderer::RenderCameras()
{
	for (auto camera : m_context.sceneManager.GetCurrentScene()->GetFastAccessComponents().cameras)
	{
		auto& actor = camera->Owner;

		if (actor.IsActive())
		{
			auto& model = *m_context.editorResources->GetModel("Camera");
			auto modelMatrix = CalculateCameraModelMatrix(actor);

			m_context.renderer->DrawModelWithSingleMaterial(model, m_cameraMaterial, &modelMatrix);
		}
	}
}

void AmberEngine::Core::EditorRenderer::RenderGrid(const glm::vec3& p_viewPos, const glm::vec3& p_color, float p_div, float p_bias, float p_LineWidth, float p_MajorLineWidth)
{
	constexpr float gridSize = 5000.0f;

	glm::mat4 model = glm::translate(glm::mat4(1.0f), { p_viewPos.x, 0.0f, p_viewPos.z }) * glm::scale(glm::mat4(1.0f), { gridSize * 2.0f, 1.f, gridSize * 2.0f });
	m_gridMaterial.SetUniform("u_Color", p_color);
	m_gridMaterial.SetUniform("GridDiv", p_div);
	m_gridMaterial.SetUniform("GridBias", p_bias);
	m_gridMaterial.SetUniform("LineWidth", p_LineWidth);
	m_gridMaterial.SetUniform("MajorLineWidth", p_MajorLineWidth);
	m_context.renderer->DrawModelWithSingleMaterial(*m_context.editorResources->GetModel("Plane"), m_gridMaterial, &model);
}

void AmberEngine::Core::EditorRenderer::UpdateLights(SceneSystem::Scene& p_scene) const
{
	PROFILER_SPY("Light SSBO Update");

	auto lightMatrices = m_context.renderer->FindLightMatrices(p_scene);
	m_context.lightSSBO->SendBlocks<glm::mat4>(lightMatrices.data(), lightMatrices.size() * sizeof(glm::mat4));
}

void DrawFrustumLines(AmberEngine::Core::ShapeDrawer& p_drawer,
                      const glm::vec3& pos,
                      const glm::vec3& forward,
                      float nearr,
                      const float farr,
                      const glm::vec3& a,
                      const glm::vec3& b,
                      const glm::vec3& c,
                      const glm::vec3& d,
                      const glm::vec3& e,
                      const glm::vec3& f,
                      const glm::vec3& g,
                      const glm::vec3& h)
{
	// Convenient lambda to draw a frustum line
	auto draw = [&](const glm::vec3& p_start, const glm::vec3& p_end, const float planeDistance)
	{
		auto offset = pos + forward * planeDistance;
		auto start = offset + p_start;
		auto end = offset + p_end;
		p_drawer.DrawLine(start, end, FRUSTUM_COLOR);
	};

	// Draw near plane
	draw(a, b, nearr);
	draw(b, d, nearr);
	draw(d, c, nearr);
	draw(c, a, nearr);

	// Draw far plane
	draw(e, f, farr);
	draw(f, h, farr);
	draw(h, g, farr);
	draw(g, e, farr);

	// Draw lines between near and far planes
	draw(a + forward * nearr, e + forward * farr, 0);
	draw(b + forward * nearr, f + forward * farr, 0);
	draw(c + forward * nearr, g + forward * farr, 0);
	draw(d + forward * nearr, h + forward * farr, 0);
}

void AmberEngine::Core::EditorRenderer::RenderCameraPerspectiveFrustum(std::pair<uint16_t, uint16_t>& p_size,
	ECS::Components::CCamera& p_camera)
{
	const auto& owner = p_camera.Owner;
	auto& camera = p_camera.GetCamera();

	const auto& cameraPos = owner.transform.GetWorldPosition();
	const auto& cameraRotation = owner.transform.GetWorldRotation();
	const auto& cameraForward = owner.transform.GetWorldForward();

	camera.ComputeMatrices(p_size.first, p_size.second, cameraPos, cameraRotation);
	const auto proj = glm::transpose(camera.GetProjectionMatrix());
	const auto nearr = camera.GetNear();
	const auto farr = camera.GetFar();

	float nLeft = nearr * (proj[2][0] - 1.0f) / proj[0][0];
	float nRight = nearr * (1.0f + proj[2][0]) / proj[0][0];
	float nTop = nearr * (1.0f + proj[2][1]) / proj[1][1];
	float nBottom = nearr * (proj[2][1] - 1.0f) / proj[1][1];

	float fLeft = farr * (proj[2][0] - 1.0f) / proj[0][0];
	float fRight = farr * (1.0f + proj[2][0]) / proj[0][0];
	float fTop = farr * (1.0f + proj[2][1]) / proj[1][1];
	float fBottom = farr * (proj[2][1] - 1.0f) / proj[1][1];

	auto a = cameraRotation * glm::vec3{ nLeft, nTop, 0 };
	auto b = cameraRotation * glm::vec3{ nRight, nTop, 0 };
	auto c = cameraRotation * glm::vec3{ nLeft, nBottom, 0 };
	auto d = cameraRotation * glm::vec3{ nRight, nBottom, 0 };
	auto e = cameraRotation * glm::vec3{ fLeft, fTop, 0 };
	auto f = cameraRotation * glm::vec3{ fRight, fTop, 0 };
	auto g = cameraRotation * glm::vec3{ fLeft, fBottom, 0 };
	auto h = cameraRotation * glm::vec3{ fRight, fBottom, 0 };

	DrawFrustumLines(*m_context.shapeDrawer, cameraPos, cameraForward, nearr, farr, a, b, c, d, e, f, g, h);
}

void AmberEngine::Core::EditorRenderer::RenderCameraOrthographicFrustum(std::pair<uint16_t, uint16_t>& p_size,
	ECS::Components::CCamera& p_camera)
{
	auto& owner = p_camera.Owner;
	auto& camera = p_camera.GetCamera();
	const auto ratio = p_size.first / static_cast<float>(p_size.second);

	const auto& cameraPos = owner.transform.GetWorldPosition();
	const auto& cameraRotation = owner.transform.GetWorldRotation();
	const auto& cameraForward = owner.transform.GetWorldForward();

	const auto nearr = camera.GetNear();
	const auto farr = camera.GetFar();
	const auto size = p_camera.GetSize();

	const auto right = ratio * size;
	const auto left = -right;
	const auto top = size;
	const auto bottom = -top;

	const auto a = cameraRotation * glm::vec3{ left, top, 0 };
	const auto b = cameraRotation * glm::vec3{ right, top, 0 };
	const auto c = cameraRotation * glm::vec3{ left, bottom, 0 };
	const auto d = cameraRotation * glm::vec3{ right, bottom, 0 };

	DrawFrustumLines(*m_context.shapeDrawer, cameraPos, cameraForward, nearr, farr, a, b, c, d, a, b, c, d);
}

void AmberEngine::Core::EditorRenderer::RenderCameraFrustum(ECS::Components::CCamera& p_camera)
{
	auto& gameView = EDITOR_PANEL(UI::Panels::GameView, "Game View");
	auto gameViewSize = gameView.GetSafeSize();
	
	if (gameViewSize.first == 0 || gameViewSize.second == 0)
	{
		gameViewSize = { 16, 9 };
	}

	switch (p_camera.GetProjectionMode())
	{
	case Rendering::Settings::EProjectionMode::ORTHOGRAPHIC:
		RenderCameraOrthographicFrustum(gameViewSize, p_camera);
		break;
	
	case Rendering::Settings::EProjectionMode::PERSPECTIVE:
		RenderCameraPerspectiveFrustum(gameViewSize, p_camera);
		break;
	}
}

void AmberEngine::Core::EditorRenderer::RenderAmbientBoxVolume(ECS::Components::CAmbientBoxLight& p_ambientBoxLight)
{
	bool depthTestBackup = m_context.renderer->GetDriver().GetCapability(Rendering::Settings::ERenderingCapability::DEPTH_TEST);
	m_context.renderer->GetDriver().SetCapability(Rendering::Settings::ERenderingCapability::DEPTH_TEST, false);

	const auto& lightData = p_ambientBoxLight.GetData();

	glm::vec3 center = p_ambientBoxLight.Owner.transform.GetWorldPosition();
	auto rotation = p_ambientBoxLight.Owner.transform.GetWorldRotation();
	glm::vec3 size = { lightData.Constant * 2.0f, lightData.Linear * 2.0f, lightData.Quadratic * 2.0f };
	glm::vec3 halfSize = size * 0.5f;

	m_context.shapeDrawer->DrawLine(center + glm::vec3{ -halfSize.x, -halfSize.y, -halfSize.z } *rotation, center + glm::vec3{ -halfSize.x, -halfSize.y, +halfSize.z }*rotation, LIGHT_VOLUME_COLOR, 1.f);
	m_context.shapeDrawer->DrawLine(center + glm::vec3{ -halfSize.x, halfSize.y, -halfSize.z } *rotation, center + glm::vec3{ -halfSize.x, +halfSize.y, +halfSize.z }*rotation, LIGHT_VOLUME_COLOR, 1.f);
	m_context.shapeDrawer->DrawLine(center + glm::vec3{ -halfSize.x, -halfSize.y, -halfSize.z } *rotation, center + glm::vec3{ -halfSize.x, +halfSize.y, -halfSize.z }*rotation, LIGHT_VOLUME_COLOR, 1.f);
	m_context.shapeDrawer->DrawLine(center + glm::vec3{ -halfSize.x, -halfSize.y, +halfSize.z } *rotation, center + glm::vec3{ -halfSize.x, +halfSize.y, +halfSize.z }*rotation, LIGHT_VOLUME_COLOR, 1.f);
	m_context.shapeDrawer->DrawLine(center + glm::vec3{ +halfSize.x, -halfSize.y, -halfSize.z } *rotation, center + glm::vec3{ +halfSize.x, -halfSize.y, +halfSize.z }*rotation, LIGHT_VOLUME_COLOR, 1.f);
	m_context.shapeDrawer->DrawLine(center + glm::vec3{ +halfSize.x, halfSize.y, -halfSize.z } *rotation, center + glm::vec3{ +halfSize.x, +halfSize.y, +halfSize.z }*rotation, LIGHT_VOLUME_COLOR, 1.f);
	m_context.shapeDrawer->DrawLine(center + glm::vec3{ +halfSize.x, -halfSize.y, -halfSize.z } *rotation, center + glm::vec3{ +halfSize.x, +halfSize.y, -halfSize.z }*rotation, LIGHT_VOLUME_COLOR, 1.f);
	m_context.shapeDrawer->DrawLine(center + glm::vec3{ +halfSize.x, -halfSize.y, +halfSize.z } *rotation, center + glm::vec3{ +halfSize.x, +halfSize.y, +halfSize.z }*rotation, LIGHT_VOLUME_COLOR, 1.f);
	m_context.shapeDrawer->DrawLine(center + glm::vec3{ -halfSize.x, -halfSize.y, -halfSize.z } *rotation, center + glm::vec3{ +halfSize.x, -halfSize.y, -halfSize.z }*rotation, LIGHT_VOLUME_COLOR, 1.f);
	m_context.shapeDrawer->DrawLine(center + glm::vec3{ -halfSize.x, +halfSize.y, -halfSize.z } *rotation, center + glm::vec3{ +halfSize.x, +halfSize.y, -halfSize.z }*rotation, LIGHT_VOLUME_COLOR, 1.f);
	m_context.shapeDrawer->DrawLine(center + glm::vec3{ -halfSize.x, -halfSize.y, +halfSize.z } *rotation, center + glm::vec3{ +halfSize.x, -halfSize.y, +halfSize.z }*rotation, LIGHT_VOLUME_COLOR, 1.f);
	m_context.shapeDrawer->DrawLine(center + glm::vec3{ -halfSize.x, +halfSize.y, +halfSize.z } *rotation, center + glm::vec3{ +halfSize.x, +halfSize.y, +halfSize.z }*rotation, LIGHT_VOLUME_COLOR, 1.f);

	m_context.renderer->GetDriver().SetCapability(Rendering::Settings::ERenderingCapability::DEPTH_TEST, depthTestBackup);
}

void AmberEngine::Core::EditorRenderer::RenderAmbientSphereVolume(
	ECS::Components::CAmbientSphereLight& p_ambientSphereLight)
{
	bool depthTestBackup = m_context.driver->GetCapability(Rendering::Settings::ERenderingCapability::DEPTH_TEST);
	m_context.driver->SetCapability(Rendering::Settings::ERenderingCapability::DEPTH_TEST, false);

	auto& data = p_ambientSphereLight.GetData();

	glm::quat rotation = p_ambientSphereLight.Owner.transform.GetWorldRotation();
	glm::vec3 center = p_ambientSphereLight.Owner.transform.GetWorldPosition();
	float radius = data.Constant;

	for (float i = 0; i <= 360.0f; i += 10.0f)
	{
		m_context.shapeDrawer->DrawLine(center + rotation * (glm::vec3{ cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f } *radius), center + rotation * (glm::vec3{ cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f } *radius), LIGHT_VOLUME_COLOR, 1.f);
		m_context.shapeDrawer->DrawLine(center + rotation * (glm::vec3{ 0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f)) } *radius), center + rotation * (glm::vec3{ 0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f)) } *radius), LIGHT_VOLUME_COLOR, 1.f);
		m_context.shapeDrawer->DrawLine(center + rotation * (glm::vec3{ cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f)) } *radius), center + rotation * (glm::vec3{ cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f)) } *radius), LIGHT_VOLUME_COLOR, 1.f);
	}

	m_context.driver->SetCapability(Rendering::Settings::ERenderingCapability::DEPTH_TEST, depthTestBackup);
}

glm::mat4 AmberEngine::Core::EditorRenderer::CalculateCameraModelMatrix(ECS::Actor& p_actor)
{
	auto translation = glm::translate(glm::mat4(1.0f), p_actor.transform.GetWorldPosition());
	auto rotation = glm::mat4(p_actor.transform.GetWorldRotation());

	return translation * rotation;
}

void AmberEngine::Core::EditorRenderer::RenderModelAsset(Resources::Model& p_model)
{
	auto model = glm::scale(glm::mat4(1.0f), { 3.f, 3.f, 3.f });
	m_context.renderer->DrawModelWithSingleMaterial(p_model, m_defaultMaterial, &model);
}

void AmberEngine::Core::EditorRenderer::RenderTextureAsset(Resources::Texture& p_texture)
{
	auto model = glm::rotate(glm::scale(glm::mat4(1.0f), { 5.f, 5.f, 5.f }), 90.f * 0.0174f, glm::vec3(1.0f, 0.0f, 0.0f));

	m_textureMaterial.SetUniform<Resources::Texture*>("u_DiffuseMap", &p_texture);
	m_context.renderer->DrawModelWithSingleMaterial(*m_context.editorResources->GetModel("Plane"), m_textureMaterial, &model);
}

void AmberEngine::Core::EditorRenderer::RenderMaterialAsset(Resources::Material& p_material)
{
	auto model = glm::scale(glm::mat4(1.0f) , { 3.f, 3.f, 3.f });
	m_context.renderer->DrawModelWithSingleMaterial(*m_context.editorResources->GetModel("Sphere"), p_material, &model, &m_emptyMaterial);
}

void AmberEngine::Core::EditorRenderer::RenderModelToStencil(const glm::mat4& p_worldMatrix, Resources::Model& p_model)
{
	m_context.renderer->GetDriver().SetStencilMask(0xFF);
	m_context.renderer->DrawModelWithSingleMaterial(p_model, m_stencilFillMaterial, &p_worldMatrix);
	m_context.renderer->GetDriver().SetStencilMask(0x00);
}

void AmberEngine::Core::EditorRenderer::RenderModelOutline(const glm::mat4& p_worldMatrix, Resources::Model& p_model,
                                                           float p_width)
{
	m_context.renderer->GetDriver().SetStencilAlgorithm(Rendering::Settings::EComparisonAlgorithm::NOTEQUAL, 1, 0xFF);
	m_context.renderer->GetDriver().SetRasterizationMode(Rendering::Settings::ERasterizationMode::LINE);
	m_context.renderer->GetDriver().SetRasterizationLinesWidth(p_width);
	m_context.renderer->DrawModelWithSingleMaterial(p_model, m_outlineMaterial, &p_worldMatrix);
	m_context.renderer->GetDriver().SetRasterizationLinesWidth(1.f);
	m_context.renderer->GetDriver().SetRasterizationMode(Rendering::Settings::ERasterizationMode::FILL);
	m_context.renderer->GetDriver().SetStencilAlgorithm(Rendering::Settings::EComparisonAlgorithm::ALWAYS, 1, 0xFF);
}

void AmberEngine::Core::EditorRenderer::RenderActorOutlinePass(ECS::Actor& p_actor, bool p_toStencil, bool p_isSelected)
{
	float outlineWidth = p_isSelected ? 5.0f : 2.5f;

	m_outlineMaterial.SetUniform("u_Diffuse", p_isSelected ? glm::vec4(1.f, 0.7f, 0.f, 1.0f) : glm::vec4(1.f, 1.f, 0.f, 1.0f));

	if (p_actor.IsActive())
	{
		/* Render static mesh outline and bounding spheres */
		if (auto modelRenderer = p_actor.GetComponent<ECS::Components::CModelRenderer>(); modelRenderer && modelRenderer->GetModel())
		{
			if (p_toStencil)
				RenderModelToStencil(p_actor.transform.GetWorldMatrix(), *modelRenderer->GetModel());
			else
				RenderModelOutline(p_actor.transform.GetWorldMatrix(), *modelRenderer->GetModel(), outlineWidth);

			//TODO:
			//if (p_isSelected && Settings::EditorSettings::ShowGeometryBounds && !p_toStencil)
			//{
			//	RenderBoundingSpheres(*modelRenderer);
			//}
		}

		/* Render camera component outline */
		if (auto cameraComponent = p_actor.GetComponent<ECS::Components::CCamera>(); cameraComponent)
		{
			auto model = CalculateCameraModelMatrix(p_actor);

			if (p_toStencil)
				RenderModelToStencil(model, *m_context.editorResources->GetModel("Camera"));
			else
				RenderModelOutline(model, *m_context.editorResources->GetModel("Camera"), outlineWidth);

			if (p_isSelected && !p_toStencil)
			{
				RenderCameraFrustum(*cameraComponent);
			}
		}

		if (p_isSelected && !p_toStencil)
		{
			if (p_actor.GetComponent<ECS::Components::CPhysicalObject>())
			{
				RenderActorCollider(p_actor);
			}

			if (auto ambientBoxComp = p_actor.GetComponent<ECS::Components::CAmbientBoxLight>())
			{
				RenderAmbientBoxVolume(*ambientBoxComp);
			}

			if (auto ambientSphereComp = p_actor.GetComponent<ECS::Components::CAmbientSphereLight>())
			{
				RenderAmbientSphereVolume(*ambientSphereComp);
			}

			//TODO:
			//if (Settings::EditorSettings::ShowLightBounds)
			//{
				if (auto light = p_actor.GetComponent<ECS::Components::CLight>())
				{
					RenderLightBounds(*light);
				}
			//}
		}

		for (auto& child : p_actor.GetChildren())
		{
			RenderActorOutlinePass(*child, p_toStencil, p_isSelected);
		}
	}
}

void AmberEngine::Core::EditorRenderer::RenderActorOutlinePass(const std::vector<ECS::Actor*>& p_actor, bool p_toStencil, bool p_isSelected)
{
	float outlineWidth = p_isSelected ? 5.0f : 2.5f;

	m_outlineMaterial.SetUniform("u_Diffuse", p_isSelected ? glm::vec4(1.f, 0.7f, 0.f, 1.0f) : glm::vec4(1.f, 1.f, 0.f, 1.0f));

	for (auto actor : p_actor)
	{
		if (actor->IsActive())
		{
			/* Render static mesh outline and bounding spheres */
			if (auto modelRenderer = actor->GetComponent<ECS::Components::CModelRenderer>(); modelRenderer && modelRenderer->GetModel())
			{
				if (p_toStencil)
					RenderModelToStencil(actor->transform.GetWorldMatrix(), *modelRenderer->GetModel());
				else
					RenderModelOutline(actor->transform.GetWorldMatrix(), *modelRenderer->GetModel(), outlineWidth);

				//TODO:
				//if (p_isSelected && Settings::EditorSettings::ShowGeometryBounds && !p_toStencil)
				//{
				//	RenderBoundingSpheres(*modelRenderer);
				//}
			}

			/* Render camera component outline */
			if (auto cameraComponent = actor->GetComponent<ECS::Components::CCamera>(); cameraComponent)
			{
				auto model = CalculateCameraModelMatrix(*actor);

				if (p_toStencil)
					RenderModelToStencil(model, *m_context.editorResources->GetModel("Camera"));
				else
					RenderModelOutline(model, *m_context.editorResources->GetModel("Camera"), outlineWidth);

				if (p_isSelected && !p_toStencil)
				{
					RenderCameraFrustum(*cameraComponent);
				}
			}

			if (p_isSelected && !p_toStencil)
			{
				if (actor->GetComponent<ECS::Components::CPhysicalObject>())
				{
					RenderActorCollider(*actor);
				}

				if (auto ambientBoxComp = actor->GetComponent<ECS::Components::CAmbientBoxLight>())
				{
					RenderAmbientBoxVolume(*ambientBoxComp);
				}

				if (auto ambientSphereComp = actor->GetComponent<ECS::Components::CAmbientSphereLight>())
				{
					RenderAmbientSphereVolume(*ambientSphereComp);
				}

				//TODO:
				//if (Settings::EditorSettings::ShowLightBounds)
				//{
				if (auto light = actor->GetComponent<ECS::Components::CLight>())
				{
					RenderLightBounds(*light);
				}
				//}
			}

			for (auto& child : actor->GetChildren())
			{
				RenderActorOutlinePass(*child, p_toStencil, p_isSelected);
			}
		}
	}
	
}

void AmberEngine::Core::EditorRenderer::RenderActorCollider(ECS::Actor& p_actor)
{
	bool depthTestBackup = m_context.renderer->GetDriver().GetCapability(Rendering::Settings::ERenderingCapability::DEPTH_TEST);
	m_context.renderer->GetDriver().SetCapability(Rendering::Settings::ERenderingCapability::DEPTH_TEST, false);

	/* Draw the box collider if any */
	if (auto boxColliderComponent = p_actor.GetComponent<ECS::Components::CPhysicalBox>(); boxColliderComponent)
	{
		glm::quat rotation = p_actor.transform.GetWorldRotation();
		glm::vec3 center = p_actor.transform.GetWorldPosition();
		glm::vec3 colliderSize = boxColliderComponent->GetSize();
		glm::vec3 actorScale = p_actor.transform.GetWorldScale();
		glm::vec3 halfSize = { colliderSize.x * actorScale.x, colliderSize.y * actorScale.y, colliderSize.z * actorScale.z };
		glm::vec3 size = halfSize * 2.f;

		m_context.shapeDrawer->DrawLine(center + rotation * glm::vec3{ -halfSize.x, -halfSize.y, -halfSize.z }, center + rotation * glm::vec3{ -halfSize.x, -halfSize.y, +halfSize.z }, glm::vec3{ 0.f, 1.f, 0.f }, 1.f);
		m_context.shapeDrawer->DrawLine(center + rotation * glm::vec3{ -halfSize.x, halfSize.y, -halfSize.z }, center + rotation *  glm::vec3{ -halfSize.x, +halfSize.y, +halfSize.z }, glm::vec3{ 0.f, 1.f, 0.f }, 1.f);
		m_context.shapeDrawer->DrawLine(center + rotation * glm::vec3{ -halfSize.x, -halfSize.y, -halfSize.z }, center + rotation * glm::vec3{ -halfSize.x, +halfSize.y, -halfSize.z }, glm::vec3{ 0.f, 1.f, 0.f }, 1.f);
		m_context.shapeDrawer->DrawLine(center + rotation * glm::vec3{ -halfSize.x, -halfSize.y, +halfSize.z }, center + rotation * glm::vec3{ -halfSize.x, +halfSize.y, +halfSize.z }, glm::vec3{ 0.f, 1.f, 0.f }, 1.f);
		m_context.shapeDrawer->DrawLine(center + rotation * glm::vec3{ +halfSize.x, -halfSize.y, -halfSize.z }, center + rotation * glm::vec3{ +halfSize.x, -halfSize.y, +halfSize.z }, glm::vec3{ 0.f, 1.f, 0.f }, 1.f);
		m_context.shapeDrawer->DrawLine(center + rotation * glm::vec3{ +halfSize.x, halfSize.y, -halfSize.z }, center + rotation *  glm::vec3{ +halfSize.x, +halfSize.y, +halfSize.z }, glm::vec3{ 0.f, 1.f, 0.f }, 1.f);
		m_context.shapeDrawer->DrawLine(center + rotation * glm::vec3{ +halfSize.x, -halfSize.y, -halfSize.z }, center + rotation * glm::vec3{ +halfSize.x, +halfSize.y, -halfSize.z }, glm::vec3{ 0.f, 1.f, 0.f }, 1.f);
		m_context.shapeDrawer->DrawLine(center + rotation * glm::vec3{ +halfSize.x, -halfSize.y, +halfSize.z }, center + rotation * glm::vec3{ +halfSize.x, +halfSize.y, +halfSize.z }, glm::vec3{ 0.f, 1.f, 0.f }, 1.f);
		m_context.shapeDrawer->DrawLine(center + rotation * glm::vec3{ -halfSize.x, -halfSize.y, -halfSize.z }, center + rotation * glm::vec3{ +halfSize.x, -halfSize.y, -halfSize.z }, glm::vec3{ 0.f, 1.f, 0.f }, 1.f);
		m_context.shapeDrawer->DrawLine(center + rotation * glm::vec3{ -halfSize.x, +halfSize.y, -halfSize.z }, center + rotation * glm::vec3{ +halfSize.x, +halfSize.y, -halfSize.z }, glm::vec3{ 0.f, 1.f, 0.f }, 1.f);
		m_context.shapeDrawer->DrawLine(center + rotation * glm::vec3{ -halfSize.x, -halfSize.y, +halfSize.z }, center + rotation * glm::vec3{ +halfSize.x, -halfSize.y, +halfSize.z }, glm::vec3{ 0.f, 1.f, 0.f }, 1.f);
		m_context.shapeDrawer->DrawLine(center + rotation * glm::vec3{ -halfSize.x, +halfSize.y, +halfSize.z }, center + rotation * glm::vec3{ +halfSize.x, +halfSize.y, +halfSize.z }, glm::vec3{ 0.f, 1.f, 0.f }, 1.f);
	}

	if (auto sphereColliderComponent = p_actor.GetComponent<ECS::Components::CPhysicalSphere>(); sphereColliderComponent)
	{
		glm::vec3 actorScale = p_actor.transform.GetWorldScale();
		glm::quat rotation = p_actor.transform.GetWorldRotation();
		glm::vec3 center = p_actor.transform.GetWorldPosition();
		float radius = sphereColliderComponent->GetRadius() * std::max(std::max(std::max(actorScale.x, actorScale.y), actorScale.z), 0.0f);

		for (float i = 0; i <= 360.0f; i += 10.0f)
		{
			m_context.shapeDrawer->DrawLine(center + rotation * (glm::vec3{ cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f } *radius), center + rotation * (glm::vec3{ cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f } *radius), glm::vec3{ 0.f, 1.f, 0.f }, 1.f);
			m_context.shapeDrawer->DrawLine(center + rotation * (glm::vec3{ 0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f)) } *radius), center + rotation * (glm::vec3{ 0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f)) } *radius), glm::vec3{ 0.f, 1.f, 0.f }, 1.f);
			m_context.shapeDrawer->DrawLine(center + rotation * (glm::vec3{ cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f)) } *radius), center + rotation * (glm::vec3{ cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f)) } *radius), glm::vec3{ 0.f, 1.f, 0.f }, 1.f);
		}
	}

	/* Draw the capsule collider if any */
	if (auto capsuleColliderComponent = p_actor.GetComponent<ECS::Components::CPhysicalCapsule>(); capsuleColliderComponent)
	{
		float radius = abs(capsuleColliderComponent->GetRadius() * std::max(std::max(p_actor.transform.GetWorldScale().x, p_actor.transform.GetWorldScale().z), 0.f));
		float height = abs(capsuleColliderComponent->GetHeight() * p_actor.transform.GetWorldScale().y);
		float halfHeight = height / 2;

		glm::vec3 actorScale = p_actor.transform.GetWorldScale();
		glm::quat rotation = p_actor.transform.GetWorldRotation();
		glm::vec3 center = p_actor.transform.GetWorldPosition();

		glm::vec3 hVec = { 0.0f, halfHeight, 0.0f };
		for (float i = 0; i < 360.0f; i += 10.0f)
		{
			m_context.shapeDrawer->DrawLine(center + rotation * (hVec + glm::vec3{ cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f)) } *radius), center + rotation * (hVec + glm::vec3{ cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f)) } *radius), glm::vec3{ 0.f, 1.f, 0.f }, 1.f);
			m_context.shapeDrawer->DrawLine(center + rotation * (-hVec + glm::vec3{ cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f)) } *radius), center + rotation * (-hVec + glm::vec3{ cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f)) } *radius), glm::vec3{ 0.f, 1.f, 0.f }, 1.f);

			if (i < 180.f)
			{
				m_context.shapeDrawer->DrawLine(center + rotation * (hVec + glm::vec3{ cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f } *radius), center + rotation * (hVec + glm::vec3{ cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f } *radius), glm::vec3{ 0.f, 1.f, 0.f }, 1.f);
				m_context.shapeDrawer->DrawLine(center + rotation * (hVec + glm::vec3{ 0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f)) } *radius), center + rotation * (hVec + glm::vec3{ 0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f)) } *radius), glm::vec3{ 0.f, 1.f, 0.f }, 1.f);
			}
			else
			{
				m_context.shapeDrawer->DrawLine(center + rotation * (-hVec + glm::vec3{ cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f } *radius), center + rotation * (-hVec + glm::vec3{ cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f } *radius), glm::vec3{ 0.f, 1.f, 0.f }, 1.f);
				m_context.shapeDrawer->DrawLine(center + rotation * (-hVec + glm::vec3{ 0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f)) } *radius), center + rotation * (-hVec + glm::vec3{ 0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f)) } *radius), glm::vec3{ 0.f, 1.f, 0.f }, 1.f);
			}
		}

		m_context.shapeDrawer->DrawLine(center + rotation * (glm::vec3{ -radius, -halfHeight, 0.f }), center + rotation * (glm::vec3{ -radius, +halfHeight, 0.f }), glm::vec3{ 0.f, 1.f, 0.f }, 1.f);
		m_context.shapeDrawer->DrawLine(center + rotation * (glm::vec3{ radius, -halfHeight, 0.f }), center + rotation * (glm::vec3{ radius, +halfHeight, 0.f }), glm::vec3{ 0.f, 1.f, 0.f }, 1.f);
		m_context.shapeDrawer->DrawLine(center + rotation * (glm::vec3{ 0.f, -halfHeight, -radius }), center + rotation * (glm::vec3{ 0.f, +halfHeight, -radius }), glm::vec3{ 0.f, 1.f, 0.f }, 1.f);
		m_context.shapeDrawer->DrawLine(center + rotation * (glm::vec3{ 0.f, -halfHeight, radius }), center + rotation * (glm::vec3{ 0.f, +halfHeight, radius }), glm::vec3{ 0.f, 1.f, 0.f }, 1.f);
	}

	m_context.renderer->GetDriver().SetCapability(Rendering::Settings::ERenderingCapability::DEPTH_TEST, depthTestBackup);
	m_context.renderer->GetDriver().SetRasterizationLinesWidth(1.0f);
}

void AmberEngine::Core::EditorRenderer::RenderLightBounds(ECS::Components::CLight& p_light)
{
	bool depthTestBackup = m_context.renderer->GetDriver().GetCapability(Rendering::Settings::ERenderingCapability::DEPTH_TEST);
	m_context.renderer->GetDriver().SetCapability(Rendering::Settings::ERenderingCapability::DEPTH_TEST, false);

	auto& data = p_light.GetData();

	glm::quat rotation = data.GetTransform().GetWorldRotation();
	glm::vec3 center = data.GetTransform().GetWorldPosition();
	float radius = data.GetEffectRange();

	if (!std::isinf(radius))
	{
		for (float i = 0; i <= 360.0f; i += 10.0f)
		{
			m_context.shapeDrawer->DrawLine(center + rotation * ( glm::vec3{ cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f } *radius), center + rotation * ( glm::vec3{ cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f } *radius), DEBUG_BOUNDS_COLOR, 1.f);
			m_context.shapeDrawer->DrawLine(center + rotation * ( glm::vec3{ 0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f)) } *radius), center + rotation * ( glm::vec3{ 0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f)) } *radius), DEBUG_BOUNDS_COLOR, 1.f);
			m_context.shapeDrawer->DrawLine(center + rotation * ( glm::vec3{ cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f)) } *radius), center + rotation * ( glm::vec3{ cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f)) } *radius), DEBUG_BOUNDS_COLOR, 1.f);
		}
	}

	m_context.renderer->GetDriver().SetCapability(Rendering::Settings::ERenderingCapability::DEPTH_TEST, depthTestBackup);
}

void AmberEngine::Core::EditorRenderer::RenderGizmo(const glm::vec3& p_position, const glm::quat& p_rotation, Core::EGizmoOperation p_operation, bool p_pickable, int p_highlightedAxis)
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f), p_position) * glm::mat4_cast(glm::normalize(p_rotation));

	Resources::Model* arrowModel = nullptr;

	if (!p_pickable)
	{
		glm::mat4 sphereModel = model * glm::scale(glm::mat4(1.0f), { 0.1f, 0.1f, 0.1f });
		m_context.renderer->DrawModelWithSingleMaterial(*m_context.editorResources->GetModel("Sphere"), m_gizmoBallMaterial, &sphereModel);
		m_gizmoArrowMaterial.SetUniform("u_HighlightedAxis", p_highlightedAxis);

		switch (p_operation)
		{
		case EGizmoOperation::TRANSLATE:
			arrowModel = m_context.editorResources->GetModel("Arrow_Translate");
			break;
		case EGizmoOperation::ROTATE:
			arrowModel = m_context.editorResources->GetModel("Arrow_Rotate");
			break;
		case EGizmoOperation::SCALE:
			arrowModel = m_context.editorResources->GetModel("Arrow_Scale");
			break;
		}
	}
	else
	{
		arrowModel = m_context.editorResources->GetModel("Arrow_Picking");
	}

	if (arrowModel)
	{
		m_context.renderer->DrawModelWithSingleMaterial(*arrowModel, p_pickable ? m_gizmoPickingMaterial : m_gizmoArrowMaterial, &model);
	}
}


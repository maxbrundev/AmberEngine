#include "Amberpch.h"

#include "AmberCore/Rendering/ECSRenderer.h"

#include "AmberRendering/Resources/Model.h"
#include "AmberRendering/Resources/ETextureFilteringMode.h"
#include "AmberRendering/Resources/Loaders/TextureLoader.h"

#include "AmberCore/SceneSystem/Scene.h"
#include "AmberCore/ECS/Actor.h"
#include "AmberCore/ECS/Components/CAmbientBoxLight.h"
#include "AmberCore/ECS/Components/CLight.h"
#include "AmberCore/ECS/Components/CMaterialRenderer.h"
#include "AmberCore/ECS/Components/CModelRenderer.h"

AmberCore::Rendering::ECSRenderer::ECSRenderer(AmberRendering::Context::Driver& p_driver) : Renderer(p_driver),
m_emptyTexture(AmberRendering::Resources::Loaders::TextureLoader::CreateColor
(
 (255 << 24) | (255 << 16) | (255 << 8) | 255,
 AmberRendering::Resources::ETextureFilteringMode::NEAREST,
 AmberRendering::Resources::ETextureFilteringMode::NEAREST,
 false
))
{
}

AmberCore::Rendering::ECSRenderer::~ECSRenderer()
{
	AmberRendering::Resources::Loaders::TextureLoader::Destroy(m_emptyTexture);
}

std::pair<AmberCore::Rendering::ECSRenderer::OpaqueDrawables, AmberCore::Rendering::ECSRenderer::TransparentDrawables>
AmberCore::Rendering::ECSRenderer::FindAndSortDrawables(const SceneSystem::Scene& p_scene, const glm::vec3& p_cameraPosition, AmberCore::Resources::Material* p_defaultMaterial)
{
	OpaqueDrawables opaqueDrawables;
	TransparentDrawables transparentDrawables;

	for (const ECS::Components::CModelRenderer* modelRenderer : p_scene.GetFastAccessComponents().modelRenderers)
	{
		if (modelRenderer->Owner.IsActive())
		{
			if (const auto model = modelRenderer->GetModel())
			{
				float distanceToActor = glm::distance(modelRenderer->Owner.transform.GetWorldPosition(), p_cameraPosition);

				if (const auto materialRenderer = modelRenderer->Owner.GetComponent<ECS::Components::CMaterialRenderer>())
				{
					const auto& transform = modelRenderer->Owner.transform.GetTransform();

					const ECS::Components::CMaterialRenderer::MaterialList& materials = materialRenderer->GetMaterials();

					for (auto mesh : model->GetMeshes())
					{
						AmberCore::Resources::Material* material = nullptr;

						if (mesh->GetMaterialIndex() < MAX_MATERIAL_COUNT)
						{
							material = materials.at(mesh->GetMaterialIndex());
							if (!material || !material->GetShader())
								material = p_defaultMaterial;
						}

						if (material)
						{
							Drawable element = { transform.GetWorldMatrix(), mesh, material, materialRenderer->GetUserMatrix() };

							if (material->IsBlendable())
								transparentDrawables.emplace(distanceToActor, element);
							else
								opaqueDrawables.emplace(distanceToActor, element);
						}
					}
				}
			}
		}
	}

	return { opaqueDrawables, transparentDrawables };
}

void AmberCore::Rendering::ECSRenderer::DrawDrawable(const Drawable& p_toDraw)
{
	m_userMatrixSender(std::get<3>(p_toDraw));
	DrawMesh(*std::get<1>(p_toDraw), *std::get<2>(p_toDraw), &std::get<0>(p_toDraw));
}

void AmberCore::Rendering::ECSRenderer::DrawModelWithSingleMaterial(AmberRendering::Resources::Model& p_model, AmberCore::Resources::Material& p_material, glm::mat4 const* p_modelMatrix, AmberCore::Resources::Material* p_defaultMaterial)
{
	if (p_modelMatrix)
		m_modelMatrixSender(*p_modelMatrix);

	for (auto mesh : p_model.GetMeshes())
	{
		if (AmberCore::Resources::Material* material = p_material.GetShader() ? &p_material : p_defaultMaterial)
			DrawMesh(*mesh, *material, nullptr);
	}
}

void AmberCore::Rendering::ECSRenderer::RegisterModelMatrixSender(std::function<void(glm::mat4)> p_modelMatrixSender)
{
	m_modelMatrixSender = std::move(p_modelMatrixSender);
}

void AmberCore::Rendering::ECSRenderer::RegisterUserMatrixSender(std::function<void(glm::mat4)> p_userMatrixSender)
{
	m_userMatrixSender = p_userMatrixSender;
}

void AmberCore::Rendering::ECSRenderer::DrawMesh(AmberRendering::Resources::Mesh& p_mesh, const AmberCore::Resources::Material& p_material, glm::mat4 const* p_modelMatrix)
{
	if (p_material.HasShader() && p_material.GetGPUInstances() > 0)
	{
		if (p_modelMatrix)
			m_modelMatrixSender(*p_modelMatrix);

		const uint8_t stateMask = p_material.GenerateStateMask();
		ApplyStateMask(stateMask);

		/* Draw the mesh */
		p_material.Bind(m_emptyTexture);
		Draw(p_mesh, AmberRendering::Settings::EPrimitiveMode::TRIANGLES, p_material.GetGPUInstances());
		p_material.Unbind();
	}
}

std::vector<glm::mat4> AmberCore::Rendering::ECSRenderer::FindLightMatrices(const SceneSystem::Scene& p_scene)
{
	std::vector<glm::mat4> result;

	const auto& fastAccessComponents = p_scene.GetFastAccessComponents();

	for (const auto light : fastAccessComponents.lights)
	{
		if (light->Owner.IsActive())
		{
			if(dynamic_cast<ECS::Components::CAmbientBoxLight*>(light))
			{
				result.push_back(light->GetData().GenerateMatrix(true));
			}
			else
			{
				result.push_back(light->GetData().GenerateMatrix());
			}
		}
	}

	return result;
}

void AmberCore::Rendering::ECSRenderer::RenderScene(const SceneSystem::Scene& p_scene, const glm::vec3& p_cameraPosition, const  AmberRendering::Entities::Camera& p_camera, AmberCore::Resources::Material* p_defaultMaterial)
{
	const auto& [opaqueMeshes, transparentMeshes] = FindAndSortDrawables(p_scene, p_cameraPosition, p_defaultMaterial);

	for (const auto&[distance, drawable] : opaqueMeshes)
		DrawDrawable(drawable);

	for (const auto&[distance, drawable] : transparentMeshes)
		DrawDrawable(drawable);
}

AmberCore::ECS::Components::CCamera* AmberCore::Rendering::ECSRenderer::FindMainCamera(const SceneSystem::Scene& p_scene)
{
	for (ECS::Components::CCamera* camera : p_scene.GetFastAccessComponents().cameras)
		if (camera->Owner.IsActive())
			return camera;

	return nullptr;
}

#include "Amberpch.h"

#include "AmberEngine/Core/ECSRenderer.h"

#include "AmberEngine/Resources/Model.h"
#include "AmberEngine/Resources/Mesh.h"

#include "AmberEngine/Resources/ETextureFilteringMode.h"
#include "AmberEngine/Resources/Loaders/TextureLoader.h"

#include "AmberEngine/Core/SceneSystem/Scene.h"

#include "AmberEngine/Core/ECS/Actor.h"
#include "AmberEngine/Core/ECS/Components/CLight.h"
#include "AmberEngine/Core/ECS/Components/CMaterialRenderer.h"
#include "AmberEngine/Core/ECS/Components/CModelRenderer.h"

AmberEngine::Core::ECSRenderer::ECSRenderer(Context::Driver& p_driver) : Renderer(p_driver),
m_emptyTexture(Resources::Loaders::TextureLoader::CreateColor
(
 (255 << 24) | (255 << 16) | (255 << 8) | 255,
 Resources::ETextureFilteringMode::NEAREST,
 Resources::ETextureFilteringMode::NEAREST,
 false
))
{
}

AmberEngine::Core::ECSRenderer::~ECSRenderer()
{
	Resources::Loaders::TextureLoader::Destroy(m_emptyTexture);
}

std::pair<AmberEngine::Core::ECSRenderer::OpaqueDrawables, AmberEngine::Core::ECSRenderer::TransparentDrawables>
AmberEngine::Core::ECSRenderer::FindAndSortDrawables(const SceneSystem::Scene& p_scene, const glm::vec3& p_cameraPosition, Resources::Material* p_defaultMaterial)
{
	OpaqueDrawables opaqueDrawables;
	TransparentDrawables transparentDrawables;

	for (const ECS::Components::CModelRenderer* modelRenderer : p_scene.GetFastAccessComponents().modelRenderers)
	{
		if (modelRenderer->owner.IsActive())
		{
			if (const auto model = modelRenderer->GetModel())
			{
				float distanceToActor = glm::distance(modelRenderer->owner.transform.GetWorldPosition(), p_cameraPosition);

				if (const auto materialRenderer = modelRenderer->owner.GetComponent<ECS::Components::CMaterialRenderer>())
				{
					const auto& transform = modelRenderer->owner.transform.GetTransform();

					const ECS::Components::CMaterialRenderer::MaterialList& materials = materialRenderer->GetMaterials();

					for (auto mesh : model->GetMeshes())
					{
						Resources::Material* material = nullptr;

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

void AmberEngine::Core::ECSRenderer::DrawDrawable(const Drawable& p_toDraw)
{
	DrawMesh(*std::get<1>(p_toDraw), *std::get<2>(p_toDraw), &std::get<0>(p_toDraw));
}

void AmberEngine::Core::ECSRenderer::RegisterModelMatrixSender(std::function<void(glm::mat4)> p_modelMatrixSender)
{
	m_modelMatrixSender = std::move(p_modelMatrixSender);
}

void AmberEngine::Core::ECSRenderer::DrawMesh(Resources::Mesh& p_mesh, const Resources::Material& p_material, glm::mat4 const* p_modelMatrix)
{
	if (p_material.HasShader() && p_material.GetGPUInstances() > 0)
	{
		if (p_modelMatrix)
			m_modelMatrixSender(*p_modelMatrix);

		const uint8_t stateMask = p_material.GenerateStateMask();
		ApplyStateMask(stateMask);

		/* Draw the mesh */
		p_material.Bind(m_emptyTexture);
		Draw(p_mesh, Rendering::Settings::EPrimitiveMode::TRIANGLES, p_material.GetGPUInstances());
		p_material.Unbind();
	}
}

std::vector<glm::mat4> AmberEngine::Core::ECSRenderer::FindLightMatrices(const SceneSystem::Scene& p_scene)
{
	std::vector<glm::mat4> result;

	const auto& fastAccessComponents = p_scene.GetFastAccessComponents();

	for (const auto light : fastAccessComponents.lights)
	{
		if (light->owner.IsActive())
		{
			result.push_back(light->GetData().GenerateMatrix());
		}
	}

	return result;
}

void AmberEngine::Core::ECSRenderer::RenderScene(const SceneSystem::Scene& p_scene, const glm::vec3& p_cameraPosition, const  Rendering::Entities::Camera& p_camera, Resources::Material* p_defaultMaterial)
{
	const auto& [opaqueMeshes, transparentMeshes] = FindAndSortDrawables(p_scene, p_cameraPosition, p_defaultMaterial);

	for (const auto&[distance, drawable] : opaqueMeshes)
		DrawDrawable(drawable);

	for (const auto&[distance, drawable] : transparentMeshes)
		DrawDrawable(drawable);

	//for (const ECS::Components::CModelRenderer* modelRenderer : p_scene.GetFastAccessComponents().modelRenderers)
	//{
	//	if (modelRenderer->owner.IsActive())
	//	{
	//		if (const auto model = modelRenderer->GetModel())
	//		{
	//			const auto& transform = modelRenderer->owner.transform.GetTransform();
	//
	//			m_modelMatrixSender(transform.GetWorldMatrix());
	//
	//			for (const auto mesh : model->GetMeshes())
	//			{
	//				auto material = model->GetMaterials()[mesh->GetMaterialIndex()];
	//
	//				if (!material || !material->GetShader())
	//				{
	//					material = p_defaultMaterial;
	//				}
	//
	//				DrawMesh(*mesh, *material, &transform.GetWorldMatrix());
	//			}
	//		}
	//	}
	//}
}

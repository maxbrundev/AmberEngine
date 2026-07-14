#pragma once


#include "AmberEditor/Core/Context.h"
#include "AmberEditor/Core/GizmoBehaviour.h"
#include "AmberCore/ECS/Components/CAmbientBoxLight.h"
#include "AmberCore/ECS/Components/CAmbientSphereLight.h"
#include "AmberCore/ECS/Components/CCamera.h"
#include "AmberCore/ECS/Components/CModelRenderer.h"

namespace AmberEditor::Core
{
	class EditorRenderer
	{
	public:
		EditorRenderer(Context& p_context);
		void InitMaterials();
		void PreparePickingMaterial(AmberCore::ECS::Actor& p_actor, AmberCore::Resources::Material& p_material);
		void RenderSceneForActorPicking();
		void RenderScene(const glm::vec3& p_cameraPosition, const AmberRendering::Entities::Camera& p_camera);
		void RenderUI() const;
		void RenderCameras();
		void RenderGrid(const glm::vec3& p_viewPos, const glm::vec3& p_color, float p_div = 10.0f, float p_bias = 0.5f, float p_LineWidth = 1.0f, float p_MajorLineWidth = 2.0f);
		void UpdateLights(AmberCore::SceneSystem::Scene& p_scene) const;

		void RenderCameraPerspectiveFrustum(std::pair<uint16_t, uint16_t>& p_size, AmberCore::ECS::Components::CCamera& p_camera);
		void RenderCameraOrthographicFrustum(std::pair<uint16_t, uint16_t>& p_size, AmberCore::ECS::Components::CCamera& p_camera);
		void RenderCameraFrustum(AmberCore::ECS::Components::CCamera& p_camera);
		void RenderAmbientBoxVolume(AmberCore::ECS::Components::CAmbientBoxLight& p_ambientBoxLight);
		void RenderAmbientSphereVolume(AmberCore::ECS::Components::CAmbientSphereLight& p_ambientSphereLight);
		glm::mat4 CalculateCameraModelMatrix(AmberCore::ECS::Actor& p_actor);
		void RenderModelAsset(AmberRendering::Resources::Model& p_model);
		void RenderTextureAsset(AmberRendering::Resources::Texture& p_texture);
		void RenderMaterialAsset(AmberCore::Resources::Material& p_material);
		void RenderModelToStencil(const glm::mat4& p_worldMatrix, AmberRendering::Resources::Model& p_model);
		void RenderModelOutline(const glm::mat4& p_worldMatrix, AmberRendering::Resources::Model& p_model, float p_width);
		void RenderActorOutlinePass(AmberCore::ECS::Actor& p_actor, bool p_toStencil, bool p_isSelected = false);
		void RenderActorOutlinePass(const std::vector<AmberCore::ECS::Actor*>& p_actor, bool p_toStencil, bool p_isSelected = false);
		void RenderActorCollider(AmberCore::ECS::Actor& p_actor);
		void RenderBoundingSpheres(AmberCore::ECS::Components::CModelRenderer& p_modelRenderer);
		void RenderLightBounds(AmberCore::ECS::Components::CLight& p_light);

		void RenderGizmo(const glm::vec3& p_position, const glm::quat& p_rotation, Core::EGizmoOperation p_operation, bool p_pickable, int p_highlightedAxis = -1);
	private:
		Context& m_context;

		AmberCore::Resources::Material m_emptyMaterial;
		AmberCore::Resources::Material m_defaultMaterial;
		AmberCore::Resources::Material m_gridMaterial;
		AmberCore::Resources::Material m_cameraMaterial;
		AmberCore::Resources::Material m_outlineMaterial;
		AmberCore::Resources::Material m_stencilFillMaterial;
		AmberCore::Resources::Material m_textureMaterial;
		AmberCore::Resources::Material m_actorPickingMaterial;
		AmberCore::Resources::Material m_gizmoArrowMaterial;
		AmberCore::Resources::Material m_gizmoBallMaterial;
		AmberCore::Resources::Material m_gizmoPickingMaterial;
	};
}

#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Core/Context.h"
#include "AmberEngine/Core/GizmoBehaviour.h"
#include "AmberEngine/Core/ECS/Components/CAmbientBoxLight.h"
#include "AmberEngine/Core/ECS/Components/CAmbientSphereLight.h"
#include "AmberEngine/Core/ECS/Components/CCamera.h"

namespace AmberEngine::Core
{
	class API_AMBERENGINE EditorRenderer
	{
	public:
		EditorRenderer(Context& p_context);
		void InitMaterials();
		void PreparePickingMaterial(ECS::Actor& p_actor, Resources::Material& p_material);
		void RenderSceneForActorPicking();
		void RenderScene(const glm::vec3& p_cameraPosition, const Rendering::Entities::Camera& p_camera);
		void RenderUI() const;
		void RenderCameras();
		void RenderGrid(const glm::vec3& p_viewPos, const glm::vec3& p_color, float p_div = 10.0f, float p_bias = 0.5f, float p_LineWidth = 1.0f, float p_MajorLineWidth = 2.0f);
		void UpdateLights(SceneSystem::Scene& p_scene) const;

		void RenderCameraPerspectiveFrustum(std::pair<uint16_t, uint16_t>& p_size, ECS::Components::CCamera& p_camera);
		void RenderCameraOrthographicFrustum(std::pair<uint16_t, uint16_t>& p_size, ECS::Components::CCamera& p_camera);
		void RenderCameraFrustum(ECS::Components::CCamera& p_camera);
		void RenderAmbientBoxVolume(ECS::Components::CAmbientBoxLight& p_ambientBoxLight);
		void RenderAmbientSphereVolume(ECS::Components::CAmbientSphereLight& p_ambientSphereLight);
		glm::mat4 CalculateCameraModelMatrix(ECS::Actor& p_actor);
		void RenderModelAsset(Resources::Model& p_model);
		void RenderTextureAsset(Resources::Texture& p_texture);
		void RenderMaterialAsset(Resources::Material& p_material);
		void RenderModelToStencil(const glm::mat4& p_worldMatrix, Resources::Model& p_model);
		void RenderModelOutline(const glm::mat4& p_worldMatrix, Resources::Model& p_model, float p_width);
		void RenderActorOutlinePass(ECS::Actor& p_actor, bool p_toStencil, bool p_isSelected = false);
		void RenderActorOutlinePass(const std::vector<ECS::Actor*>& p_actor, bool p_toStencil, bool p_isSelected = false);
		void RenderActorCollider(ECS::Actor& p_actor);
		void RenderLightBounds(ECS::Components::CLight& p_light);

		void RenderGizmo(const glm::vec3& p_position, const glm::quat& p_rotation, Core::EGizmoOperation p_operation, bool p_pickable, int p_highlightedAxis = -1);
	private:
		Context& m_context;

		Resources::Material m_emptyMaterial;
		Resources::Material m_defaultMaterial;
		Resources::Material m_gridMaterial;
		Resources::Material m_cameraMaterial;
		Resources::Material m_outlineMaterial;
		Resources::Material m_stencilFillMaterial;
		Resources::Material m_textureMaterial;
		Resources::Material m_actorPickingMaterial;
		Resources::Material m_gizmoArrowMaterial;
		Resources::Material m_gizmoBallMaterial;
		Resources::Material m_gizmoPickingMaterial;
	};
}

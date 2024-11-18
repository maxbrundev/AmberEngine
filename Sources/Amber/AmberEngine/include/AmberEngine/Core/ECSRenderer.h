#pragma once

#include <map>

#include <glm/glm.hpp>

#include "AmberEngine/Core/ECS/Components/CCamera.h"
#include "AmberEngine/Rendering/Core/Renderer.h"

namespace AmberEngine::Core
{
	class API_AMBERENGINE ECSRenderer : public Rendering::Core::Renderer
	{
	public:
		using Drawable             = std::tuple<glm::mat4, Resources::Mesh*, Resources::Material*, glm::mat4>;
		using OpaqueDrawables      = std::multimap<float, Drawable, std::less<float>>;
		using TransparentDrawables = std::multimap<float, Drawable, std::greater<float>>;

		ECSRenderer(Context::Driver& p_driver);
		~ECSRenderer();

		std::pair<OpaqueDrawables, TransparentDrawables> FindAndSortDrawables
		(
			const SceneSystem::Scene& p_scene,
			const glm::vec3& p_cameraPosition,
			Resources::Material* p_defaultMaterial
		);

		void DrawDrawable(const Drawable& p_toDraw);
		void DrawModelWithSingleMaterial(Resources::Model& p_model, Resources::Material& p_material, glm::mat4 const* p_modelMatrix, Resources::Material* p_defaultMaterial = nullptr);

		void RegisterModelMatrixSender(std::function<void(glm::mat4)> p_modelMatrixSender);
		void RegisterUserMatrixSender(std::function<void(glm::mat4)> p_userMatrixSender);
		void DrawMesh(Resources::Mesh& p_mesh, const Resources::Material& p_material, glm::mat4 const* p_modelMatrix);

		std::vector<glm::mat4> FindLightMatrices(const SceneSystem::Scene& p_scene);

		void RenderScene(const SceneSystem::Scene& p_scene, const glm::vec3& p_cameraPosition, const  Rendering::Entities::Camera& p_camera, Resources::Material* p_defaultMaterial);

		ECS::Components::CCamera* FindMainCamera(const SceneSystem::Scene& p_scene);

	private:
		std::function<void(glm::mat4)> m_modelMatrixSender;
		std::function<void(glm::mat4)> m_userMatrixSender;
		Resources::Texture* m_emptyTexture = nullptr;
	};
}

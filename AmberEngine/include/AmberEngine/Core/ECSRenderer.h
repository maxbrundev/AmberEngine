#pragma once

#include <map>

#include <glm/glm.hpp>

#include "AmberEngine/Rendering/Core/Renderer.h"

namespace AmberEngine::Core
{
	class API_AMBERENGINE ECSRenderer : public Renderer
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

		void RegisterModelMatrixSender(std::function<void(glm::mat4)> p_modelMatrixSender);
		void DrawMesh(Resources::Mesh& p_mesh, const Resources::Material& p_material, glm::mat4 const* p_modelMatrix);

		std::vector<glm::mat4> FindLightMatrices(const SceneSystem::Scene& p_scene);

		void RenderScene(const SceneSystem::Scene& p_scene, const glm::vec3& p_cameraPosition, const  Rendering::Entities::Camera& p_camera, Resources::Material* p_defaultMaterial);

	private:
		std::function<void(glm::mat4)> m_modelMatrixSender;
		Resources::Texture* m_emptyTexture = nullptr;
	};
}

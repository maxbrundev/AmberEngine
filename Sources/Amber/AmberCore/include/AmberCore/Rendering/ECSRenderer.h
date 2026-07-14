#pragma once

#include <map>

#include <glm/glm.hpp>

#include "AmberCore/ECS/Components/CCamera.h"
#include "AmberRendering/Core/Renderer.h"

namespace AmberRendering::Resources
{
	class Texture;
}

namespace AmberCore::Resources
{
	class Material;
}

namespace AmberCore::SceneSystem
{
	class Scene;
}

namespace AmberCore::Rendering
{
	class ECSRenderer : public AmberRendering::Core::Renderer
	{
	public:
		using Drawable             = std::tuple<glm::mat4, AmberRendering::Resources::Mesh*, AmberCore::Resources::Material*, glm::mat4>;
		using OpaqueDrawables      = std::multimap<float, Drawable, std::less<float>>;
		using TransparentDrawables = std::multimap<float, Drawable, std::greater<float>>;

		ECSRenderer(AmberRendering::Context::Driver& p_driver);
		~ECSRenderer();

		std::pair<OpaqueDrawables, TransparentDrawables> FindAndSortDrawables
		(
			const AmberCore::SceneSystem::Scene& p_scene,
			const glm::vec3& p_cameraPosition,
			AmberCore::Resources::Material* p_defaultMaterial
		);

		void DrawDrawable(const Drawable& p_toDraw);
		void DrawModelWithSingleMaterial(AmberRendering::Resources::Model& p_model, AmberCore::Resources::Material& p_material, glm::mat4 const* p_modelMatrix, AmberCore::Resources::Material* p_defaultMaterial = nullptr);

		void RegisterModelMatrixSender(std::function<void(glm::mat4)> p_modelMatrixSender);
		void RegisterUserMatrixSender(std::function<void(glm::mat4)> p_userMatrixSender);
		void DrawMesh(AmberRendering::Resources::Mesh& p_mesh, const AmberCore::Resources::Material& p_material, glm::mat4 const* p_modelMatrix);

		std::vector<glm::mat4> FindLightMatrices(const AmberCore::SceneSystem::Scene& p_scene);

		void RenderScene(const AmberCore::SceneSystem::Scene& p_scene, const glm::vec3& p_cameraPosition, const  AmberRendering::Entities::Camera& p_camera, AmberCore::Resources::Material* p_defaultMaterial);

		ECS::Components::CCamera* FindMainCamera(const AmberCore::SceneSystem::Scene& p_scene);

	private:
		std::function<void(glm::mat4)> m_modelMatrixSender;
		std::function<void(glm::mat4)> m_userMatrixSender;
		AmberRendering::Resources::Texture* m_emptyTexture = nullptr;
	};
}

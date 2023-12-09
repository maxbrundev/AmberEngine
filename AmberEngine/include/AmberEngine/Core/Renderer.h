#pragma once

#include <glm/glm.hpp>

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Context/Driver.h"

namespace AmberEngine::Resources
{
	class Model;
	class Mesh;
	class Texture;
	class Material;
}

namespace AmberEngine::LowRenderer
{
	class Camera;
}

namespace AmberEngine::Core::SceneSystem
{
	class Scene;
}

namespace AmberEngine::Core
{
	class API_AMBERENGINE Renderer
	{
	public:
		struct FrameInfo
		{
			uint64_t batchCount = 0;
			uint64_t instanceCount = 0;
			uint64_t polyCount = 0;
			uint64_t vertexCount = 0;
		};

		Renderer(Context::Driver& p_driver);
		~Renderer();

		void RenderScene(SceneSystem::Scene& p_scene, Resources::Material* p_defaultMaterial = nullptr);

		void Draw(Resources::Model& p_model, glm::mat4 const* p_modelMatrix, Resources::Material* p_defaultMaterial);
		void SetClearColor(float p_red, float p_green, float p_blue, float p_alpha = 1.0f) const;
		void Clear(bool p_colorBuffer = true, bool p_depthBuffer = true, bool p_stencilBuffer = true) const;
		void Clear(LowRenderer::Camera& p_camera, bool p_colorBuffer = true, bool p_depthBuffer = true, bool p_stencilBuffer = true) const;
		std::vector<glm::mat4> FindLightMatrices(SceneSystem::Scene& p_scene);
		void RegisterModelMatrixSender(std::function<void(glm::mat4)> p_modelMatrixSender);

		void PolygonModeLine() const;
		void PolygonModeFill() const;

		void ClearFrameInfo();
		const FrameInfo& GetFrameInfo() const;

	private:
		std::function<void(glm::mat4)> m_modelMatrixSender;
		void DrawMesh(const Resources::Mesh& p_mesh, Resources::Material& p_material);

	private:
		Context::Driver& m_driver;
		FrameInfo m_frameInfo;
		Resources::Texture* m_emptyTexture = nullptr;
	};
}

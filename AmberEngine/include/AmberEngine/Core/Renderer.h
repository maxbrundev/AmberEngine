#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Context/Driver.h"

#include "AmberEngine/LowRenderer/CameraController.h"

#include "AmberEngine/Resources/Model.h"
#include "AmberEngine/Resources/Texture.h"

namespace AmberEngine::Core
{
	class API_AMBERENGINE Renderer
	{
	public:
		Renderer(Context::Driver& p_driver);
		~Renderer();

		void Draw(Resources::Model& p_model, glm::mat4 const* p_modelMatrix) const;
		void SetClearColor(float p_red, float p_green, float p_blue, float p_alpha = 1.0f) const;
		void Clear(bool p_colorBuffer = true, bool p_depthBuffer = true, bool p_stencilBuffer = true) const;
		void Clear(LowRenderer::Camera& p_camera, bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer) const;

		void RegisterModelMatrixSender(std::function<void(glm::mat4)> p_modelMatrixSender);

		void PolygonModeLine() const;
		void PolygonModeFill() const;

	private:
		std::function<void(glm::mat4)> m_modelMatrixSender;
		void DrawMesh(const Resources::Mesh& p_mesh, Resources::Material& p_material) const;

	private:
		Context::Driver& m_driver;

		Resources::Texture* m_emptyTexture = nullptr;
	};
}

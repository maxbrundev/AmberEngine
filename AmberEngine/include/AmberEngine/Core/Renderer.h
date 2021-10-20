#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Context/Driver.h"

#include "AmberEngine/LowRenderer/CameraController.h"
#include "AmberEngine/Resources/Model.h"

namespace AmberEngine::Core
{
	class API_AMBERENGINE Renderer
	{
	public:
		Renderer(Context::Driver& p_driver);
		~Renderer() = default;

		void Draw(Resources::Model& p_model);
		void SetClearColor(float p_red, float p_green, float p_blue, float p_alpha = 1.0f);
		void Clear(bool p_colorBuffer = true, bool p_depthBuffer = true, bool p_stencilBuffer = true);
		void Clear(LowRenderer::Camera& p_camera, bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer);

		void UpdateRenderMode();

		void PolygonModeLine();
		void PolygonModeFill();

		void ToggleWireFrame();

	private:
		void DrawMesh(Resources::Mesh& p_mesh);
	private:
		Context::Driver& m_driver;
		bool isWireFrame;
	};
}

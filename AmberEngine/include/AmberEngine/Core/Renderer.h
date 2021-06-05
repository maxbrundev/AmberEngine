#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Context/Driver.h"

#include "AmberEngine/LowRenderer/CameraController.h"
#include "AmberEngine/Resources/AssimpModel.h"
#include "AmberEngine/Resources/AssimpMesh.h"

namespace AmberEngine::Core
{
	class API_AMBERENGINE Renderer
	{
	public:
		Renderer(Context::Driver& p_driver);
		~Renderer() = default;

		// Quick Draw method, Going to be reworked.
		void DrawModelWithShader(Resources::AssimpModel& p_model, Resources::Shader& p_shader);
		void DrawMesh(Resources::AssimpMesh& p_mesh, Resources::Shader& p_shader);
		void SetClearColor(float p_red, float p_green, float p_blue, float p_alpha = 1.0f);
		void Clear(bool p_colorBuffer = true, bool p_depthBuffer = true, bool p_stencilBuffer = true);
		void Clear(LowRenderer::Camera& p_camera, bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer);

		void UpdateRenderMode();

		void PolygonModeLine();
		void PolygonModeFill();

		void ToggleWireFrame();
		
	private:
		Context::Driver m_driver;
		bool isWireFrame;
	};
}

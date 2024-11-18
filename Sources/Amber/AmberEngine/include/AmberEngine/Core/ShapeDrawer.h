#pragma once

#include "AmberEngine/Rendering/Core/Renderer.h"

#include "AmberEngine/Resources/Shader.h"

namespace AmberEngine::Core
{
	class ShapeDrawer
	{
	public:
		ShapeDrawer(Rendering::Core::Renderer& p_renderer);

		~ShapeDrawer();

		void SetViewProjection(const glm::mat4& p_viewProjection);

		void DrawLine(const glm::vec3& p_start, const glm::vec3& p_end, const glm::vec3& p_color, float p_lineWidth = 1.0f);
		void DrawGrid(const glm::vec3& p_viewPos, const glm::vec3& p_color, int32_t p_gridSize = 50, float p_linear = 0.0f, float p_quadratic = 0.0f, float p_fadeThreshold = 0.0f, float p_lineWidth = 1.0f);

	private:
		Resources::Shader* m_lineShader = nullptr;
		Resources::Shader* m_gridShader = nullptr;
		Resources::Mesh* m_lineMesh     = nullptr;

		Rendering::Core::Renderer& m_renderer;
	};
}

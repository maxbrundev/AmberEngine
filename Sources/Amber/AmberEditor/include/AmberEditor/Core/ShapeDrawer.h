#pragma once

#include "AmberRendering/Core/Renderer.h"

#include "AmberRendering/Resources/Shader.h"

namespace AmberEditor::Core
{
	class ShapeDrawer
	{
	public:
		ShapeDrawer(AmberRendering::Core::Renderer& p_renderer);

		~ShapeDrawer();

		void SetViewProjection(const glm::mat4& p_viewProjection);

		void DrawLine(const glm::vec3& p_start, const glm::vec3& p_end, const glm::vec3& p_color, float p_lineWidth = 1.0f);
		void DrawGrid(const glm::vec3& p_viewPos, const glm::vec3& p_color, int32_t p_gridSize = 50, float p_linear = 0.0f, float p_quadratic = 0.0f, float p_fadeThreshold = 0.0f, float p_lineWidth = 1.0f);

	private:
		AmberRendering::Resources::Shader* m_lineShader = nullptr;
		AmberRendering::Resources::Shader* m_gridShader = nullptr;
		AmberRendering::Resources::Mesh* m_lineMesh     = nullptr;

		AmberRendering::Core::Renderer& m_renderer;
	};
}

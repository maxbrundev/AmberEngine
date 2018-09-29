#pragma once

#include "Shader.h"
#include "Texture.h"

namespace RenderEngine
{
	namespace Resources
	{
		class Material
		{
		private:
			glm::vec3 m_ambient;
			glm::vec3 m_diffuse;
			glm::vec3 m_specular;
			float m_shininess;

			RenderEngine::Resources::Shader* m_shader;
			RenderEngine::Resources::Texture* m_texture;
		public:
			Material(Resources::Shader* p_shader, Resources::Texture* p_texture = nullptr);
			~Material() = default;
		};
	}
}
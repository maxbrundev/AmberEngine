#pragma once

namespace RenderEngine
{
	namespace Resources
	{
		class Material
		{
		private:
			glm::vec3 m_ambient{};
			glm::vec3 m_diffuse{};
			glm::vec3 m_specular{};
			GLuint m_diffuseMap{};
			GLuint m_specularMap{};
			GLuint m_emissiveMap{};
			float m_shininess;

		public:
			Material(glm::vec3& p_ambient, glm::vec3& p_diffuse, glm::vec3& p_specular, float p_shininess, GLuint p_diffuseMap, GLuint p_specularMap, GLuint p_emissive);
			~Material() = default;

			void SendToShader(class Shader& p_shader);
		};
	}
}
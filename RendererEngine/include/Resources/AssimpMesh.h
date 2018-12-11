#pragma once

namespace RenderEngine
{
	namespace Resources
	{
		struct Vertex 
		{
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 texCoords;
			glm::vec3 tangent;
			glm::vec3 bitangent;
		};

		struct TextureData 
		{
			unsigned int id;
			std::string type;
			std::string path;
		};

		class AssimpMesh
		{
		private:
			std::vector<Vertex> m_vertices;
			std::vector<TextureData> m_textures;
			std::vector<unsigned int> m_indices;

			unsigned int m_vao;
			unsigned int m_vbo;
			unsigned int m_ebo;

		public:
			AssimpMesh(std::vector<Vertex> p_vertices, std::vector<unsigned int> p_indices, std::vector<TextureData> p_textures);
			~AssimpMesh() = default;
			void BindBuffers();
			void DeleteBuffers();

		private:
			void InitBuffers();
		};
	}
}
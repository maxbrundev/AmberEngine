#pragma once

#include "Resources/Shader.h"

namespace RenderEngine
{
	namespace Resources
	{
		struct Vertex 
		{
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TexCoords;
			glm::vec3 Tangent;
			glm::vec3 Bitangent;
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
			unsigned int VBO;
			unsigned int EBO;

		public:
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			std::vector<TextureData> textures;
			unsigned int VAO;

		public:
			AssimpMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureData> textures);
			void Draw(Shader& shader);
			~AssimpMesh() = default;

		private:
			void setupMesh();
		};
	}
}
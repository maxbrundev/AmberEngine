#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Resources/Shader.h"
#include "AmberEngine/Resources/Texture.h"

//TODO: Refactor.

namespace AmberEngine::Resources
{
	struct API_AMBERENGINE AssimpVertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
		glm::vec3 tangent;
		glm::vec3 bitangent;
	};

	class API_AMBERENGINE AssimpMesh
	{
	public:
		std::vector<AssimpVertex> m_vertices;
		std::vector<Texture*> m_textures;
		std::vector<unsigned int> m_indices;

		unsigned int m_vao;
		unsigned int m_vbo;
		unsigned int m_ebo;

	public:
		AssimpMesh(std::vector<AssimpVertex> p_vertices, std::vector<unsigned int> p_indices, std::vector<Texture*> p_textures);
		~AssimpMesh() = default;

		void BindBuffers(Shader& p_shader);
		void DeleteBuffers();

	private:
		void InitBuffers();
	};
}
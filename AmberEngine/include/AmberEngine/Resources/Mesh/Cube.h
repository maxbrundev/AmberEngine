#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "AmberEngine/API/Export.h"

namespace AmberEngine::PrimitiveMesh
{
	struct API_AMBERENGINE Vertex
	{
		Vertex() = default;
		Vertex(glm::vec3 p_position, glm::vec2 p_textureCoord, glm::vec3 p_normals)
		{
			position = p_position;
			textureCoord = p_textureCoord;
			normals = p_normals;
		}

		glm::vec3 position;
		glm::vec2 textureCoord;
		glm::vec3 normals;
	};

	class API_AMBERENGINE Cube
	{
	private:
		static std::vector<PrimitiveMesh::Vertex> m_vertices;
		static glm::vec3 m_positions[8];
		static glm::vec3 m_normals[6];
		static glm::vec2 m_textureCoordinates[4];
		static bool m_initialized;

	public:
		Cube() = default;
		~Cube() = default;

		static void Setup();
		static void InitCubePositions();
		static void InitCubeTextureCoordinates();
		static void InitCubeNormals();
		static void InitCubeVertices();
		static void CreateTriangle(uint8_t pos1, uint8_t pos2, uint8_t pos3, uint8_t text1, uint8_t text2, uint8_t text3, uint8_t p_normals);

		static std::vector<PrimitiveMesh::Vertex>& GetVertices();
	};
}
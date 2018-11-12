#include "pch.h"

#include "Resources/Mesh/Cube.h"

std::vector<RenderEngine::PrimitiveMesh::Vertex> RenderEngine::PrimitiveMesh::Cube::m_vertices;
glm::vec3 RenderEngine::PrimitiveMesh::Cube::m_positions[8];
glm::vec3 RenderEngine::PrimitiveMesh::Cube::m_normals[6];
glm::vec2 RenderEngine::PrimitiveMesh::Cube::m_textureCoordinates[4];
bool RenderEngine::PrimitiveMesh::Cube::m_initialized = false;

void RenderEngine::PrimitiveMesh::Cube::Setup()
{
	if (!m_initialized)
	{
		m_initialized = true;
		InitCubePositions();
		InitCubeNormals();
		InitCubeTextureCoordinates();
		InitCubeVertices();
	}
}

void RenderEngine::PrimitiveMesh::Cube::InitCubePositions()
{
	m_positions[0] = glm::vec3(-0.5, +0.5, -0.5); // A,V,O
	m_positions[1] = glm::vec3(-0.5, +0.5, +0.5); // C,X,E
	m_positions[2] = glm::vec3(+0.5, +0.5, +0.5); // D,S,F
	m_positions[3] = glm::vec3(+0.5, +0.5, -0.5); // B,Q,P
	m_positions[4] = glm::vec3(-0.5, -0.5, -0.5); // U,K,M
	m_positions[5] = glm::vec3(-0.5, -0.5, +0.5); // W,G,I
	m_positions[6] = glm::vec3(+0.5, -0.5, +0.5); // T,H,J
	m_positions[7] = glm::vec3(+0.5, -0.5, -0.5); // R,L,N
}

void RenderEngine::PrimitiveMesh::Cube::InitCubeTextureCoordinates()
{
	m_textureCoordinates[0] = glm::vec2(0, 0);
	m_textureCoordinates[1] = glm::vec2(0, 1);
	m_textureCoordinates[2] = glm::vec2(1, 0);
	m_textureCoordinates[3] = glm::vec2(1, 1);
}

void RenderEngine::PrimitiveMesh::Cube::InitCubeNormals()
{
	m_normals[0] = glm::vec3(-1, 0, 0);
	m_normals[1] = glm::vec3(1, 0, 0);
	m_normals[2] = glm::vec3(0, -1, 0);
	m_normals[3] = glm::vec3(0, 1, 0);
	m_normals[4] = glm::vec3(0, 0, -1);
	m_normals[5] = glm::vec3(0, 0, 1);
}

void RenderEngine::PrimitiveMesh::Cube::InitCubeVertices()
{
	// Top
	CreateTriangle(3, 1, 2, 3, 0, 2, 3);
	CreateTriangle(3, 0, 1, 3, 1, 0, 3);

	// Front
	CreateTriangle(2, 5, 6, 3, 0, 2, 5);
	CreateTriangle(2, 1, 5, 3, 1, 0, 5);

	// Bottom
	CreateTriangle(4, 6, 5, 3, 0, 2, 2);
	CreateTriangle(4, 7, 6, 3, 1, 0, 2);

	// Back
	CreateTriangle(0, 7, 4, 3, 0, 2, 4);
	CreateTriangle(0, 3, 7, 3, 1, 0, 4);

	// Left
	CreateTriangle(1, 4, 5, 3, 0, 2, 0);
	CreateTriangle(1, 0, 4, 3, 1, 0, 0);

	// Right
	CreateTriangle(3, 6, 7, 3, 0, 2, 1);
	CreateTriangle(3, 2, 6, 3, 1, 0, 1);
}

void RenderEngine::PrimitiveMesh::Cube::CreateTriangle(uint8_t pos1, uint8_t pos2, uint8_t pos3, uint8_t text1,
	uint8_t text2, uint8_t text3, uint8_t p_normals)
{
	m_vertices.emplace_back(m_positions[pos1], m_textureCoordinates[text1], m_normals[p_normals]);
	m_vertices.emplace_back(m_positions[pos2], m_textureCoordinates[text2], m_normals[p_normals]);
	m_vertices.emplace_back(m_positions[pos3], m_textureCoordinates[text3], m_normals[p_normals]);
}

std::vector<RenderEngine::PrimitiveMesh::Vertex>& RenderEngine::PrimitiveMesh::Cube::GetVertices()
{
	return m_vertices;
}
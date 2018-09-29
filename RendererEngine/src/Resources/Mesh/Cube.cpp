#include "pch.h"

#include "Resources/Mesh/Cube.h"

RenderEngine::PrimitiveMesh::Cube::Cube() : m_vbo(m_vertices.cubeVertices, sizeof(m_vertices.cubeVertices))
{
}

void RenderEngine::PrimitiveMesh::Cube::Init()
{
	m_layout.PushFloat(3);
	m_layout.PushFloat(3);
	m_vao.AddBuffer(m_vbo, m_layout);

	m_vao.Unbind();
	m_vbo.Unbind();
}

void RenderEngine::PrimitiveMesh::Cube::Bind() const
{
	m_vao.Bind();
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
}
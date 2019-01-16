#pragma once

#include <string>

#include <GL/glew.h>

#include "AmberEngine/Resources/ObjLoader.h"

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Resources
{
	class API_AMBERENGINE Mesh
	{
	private:
		GLuint m_vao;
		GLuint m_vbo[4];
		GLsizei m_numIndices;
		std::string m_sourceFile;

	public:
		Mesh(const std::string& p_filePath);
		~Mesh();

		void Draw();
		void InitBuffers(const Model& p_model);
	};
}
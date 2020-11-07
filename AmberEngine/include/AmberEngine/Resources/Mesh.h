#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Resources/ObjLoader.h"

namespace AmberEngine::Resources
{
	class API_AMBERENGINE Mesh
	{
	private:
		GLuint m_vao;
		GLuint m_vbo[4];
		unsigned int m_numIndices;
		std::string m_sourceFile;

	public:
		Mesh(const std::string& p_filePath);
		~Mesh();

		void Draw();
		void InitBuffers(const ObjModel& p_model);
	};
}
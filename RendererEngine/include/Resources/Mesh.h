#pragma once

#include "Resources/ObjLoader.h"
namespace RenderEngine
{
	namespace Resources
	{
		class Mesh
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
}
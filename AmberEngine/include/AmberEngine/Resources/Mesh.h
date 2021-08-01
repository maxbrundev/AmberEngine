#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Geometry/Vertex.h"
#include "AmberEngine/Resources/Texture.h"

namespace AmberEngine::Resources
{
	//class Model;
	//typedef void(Model::*SetTextureUnifromCallback)(std::string, int);
	
	class API_AMBERENGINE Mesh
	{
	public:
		Mesh(const std::vector<Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices, std::vector<Texture*> p_textures);
		~Mesh();

		void Bind();
		void Unbind();
		void BindMaterialTextures();
		void DeleteBuffers();
		std::function<void(std::string, int)> callback;
		//SetTextureUnifromCallback callback;

	private:
		void InitBuffers(const std::vector<Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices);

	public:
		const uint32_t m_vertexCount;
		const uint32_t m_indicesCount;
		std::vector<Texture*> m_textures;

		unsigned int m_vao;
		unsigned int m_vbo;
		unsigned int m_ebo;
	};
}
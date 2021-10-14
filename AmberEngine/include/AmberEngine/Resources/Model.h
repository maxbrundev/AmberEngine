#pragma once

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Resources
{
	class Mesh;
	class Shader;
	
	class API_AMBERENGINE Model
	{
	public:
		Model(std::string_view p_filePath);
		~Model();

		void SetShader(Shader& p_shader);
		
		void Bind();
		
		std::vector<Mesh*>& GetMeshes();
		std::vector<std::string>& GetMaterialNames();
		void SetTextureUniform(std::string uniformName, int id);
		
	public:
		const std::string_view path;

		std::vector<Mesh*> m_meshes;
		std::vector<std::string> m_materialNames;
		Shader* m_shader;
	};
}
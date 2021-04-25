#pragma once

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Resources
{
	class AssimpMesh;
	class Shader;

	class API_AMBERENGINE AssimpModel
	{
	public:
		AssimpModel(std::string_view p_filePath);
		~AssimpModel();
		
		void Draw(Shader& p_shader);

	public:
		std::vector<AssimpMesh*> m_meshes;
		
		std::vector<std::string> m_materialNames;
		const std::string_view path;
	};
}
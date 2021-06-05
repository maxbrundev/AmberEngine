#pragma once

#include "AmberEngine/API/Export.h"
#include "AmberEngine/Resources/Shader.h"

namespace AmberEngine::Resources
{
	class AssimpMesh;
	class Shader;

	class API_AMBERENGINE AssimpModel
	{
	public:
		AssimpModel(std::string_view p_filePath);
		~AssimpModel();

		std::vector<AssimpMesh*>& GetMeshes();
		std::vector<std::string>& GetMaterialNames();
		
	public:
		std::vector<AssimpMesh*> m_meshes;
		
		std::vector<std::string> m_materialNames;
		const std::string_view path;
	};
}
#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Resources/Mesh.h"
#include "AmberEngine/Resources/Material.h"

namespace AmberEngine::Resources
{
	namespace Loaders { class ModelLoader; }
	
	class API_AMBERENGINE Model
	{
		friend class Loaders::ModelLoader;

	public:
		Model(const std::string& p_filePath);
		~Model();

		void SetShader(Shader& p_shader) const;

		std::vector<std::string>& GetMaterialNames();
		std::vector<Mesh*>& GetMeshes();
		std::array<Material*, 255>& GetMaterials();
		
	private:
		const std::string path;

		std::vector<std::string> m_materialNames;
		std::vector<Mesh*> m_meshes;

		std::array<Material*, 255> m_materials;
	};
}
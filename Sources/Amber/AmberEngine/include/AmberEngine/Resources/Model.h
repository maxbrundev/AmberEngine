#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Resources/Mesh.h"
#include "AmberEngine/Resources/Material.h"

namespace  AmberEngine::Resources::Loaders
{
	class ModelLoader;
}

namespace AmberEngine::Resources
{
	class API_AMBERENGINE Model
	{
		friend class Loaders::ModelLoader;

	public:
		std::vector<std::string>& GetMaterialNames();
		std::vector<Mesh*>& GetMeshes();

	private:
		Model(const std::string& p_filePath);
		~Model();

	public:
		const std::string Path;
		std::map<int, std::vector<std::tuple<ETextureType, std::string>>> LoadedTextureData;

	private:
		std::vector<std::string> m_materialNames;
		std::vector<Mesh*> m_meshes;
	};
}
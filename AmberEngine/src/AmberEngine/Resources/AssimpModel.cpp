#include "Amberpch.h"

#include "AmberEngine/Resources/AssimpModel.h"

#include "AmberEngine/Resources/AssimpMesh.h"

AmberEngine::Resources::AssimpModel::AssimpModel(std::string_view p_filePath) : path(p_filePath)
{
}

AmberEngine::Resources::AssimpModel::~AssimpModel()
{
	for (auto mesh : m_meshes)
	{
		delete mesh;
		mesh = nullptr;
	}
}

std::vector<AmberEngine::Resources::AssimpMesh*>& AmberEngine::Resources::AssimpModel::GetMeshes()
{
	return m_meshes;
}

std::vector<std::string>& AmberEngine::Resources::AssimpModel::GetMaterialNames()
{
	return m_materialNames;
}

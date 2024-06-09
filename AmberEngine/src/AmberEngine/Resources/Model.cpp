#include "Amberpch.h"

#include "AmberEngine/Resources/Model.h"

#include "AmberEngine/Eventing/Event.h"

AmberEngine::Resources::Model::Model(const std::string& p_filePath) : Path(p_filePath)
{
}

AmberEngine::Resources::Model::~Model()
{
	for (auto& mesh : m_meshes)
	{
		delete mesh;
		mesh = nullptr;
	}
}

std::vector<std::string>& AmberEngine::Resources::Model::GetMaterialNames()
{
	return m_materialNames;
}

std::vector<AmberEngine::Resources::Mesh*>& AmberEngine::Resources::Model::GetMeshes()
{
	return m_meshes;
}
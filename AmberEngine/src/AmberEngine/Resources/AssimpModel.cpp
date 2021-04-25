#include "Amberpch.h"

#include "AmberEngine/Resources/AssimpModel.h"

#include "AmberEngine/Resources/AssimpMesh.h"

AmberEngine::Resources::AssimpModel::AssimpModel(std::string_view p_filePath) : path(p_filePath)
{
}

void AmberEngine::Resources::AssimpModel::Draw(Shader& p_shader)
{
	for (int i = 0; i < m_meshes.size(); i++)
		m_meshes[i]->BindBuffers(p_shader);
}

AmberEngine::Resources::AssimpModel::~AssimpModel()
{
	for (auto mesh : m_meshes)
		delete mesh;
}
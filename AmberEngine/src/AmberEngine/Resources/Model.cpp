#include "Amberpch.h"

#include "AmberEngine/Resources/Model.h"

AmberEngine::Resources::Model::Model(const std::string& p_filePath) : path(p_filePath)
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

void AmberEngine::Resources::Model::SetShader(Shader* p_shader)
{
	for (const auto mesh : m_meshes)
	{
		mesh->GetMaterial().SetShader(p_shader);
	}
}

std::vector<AmberEngine::Resources::Mesh*>& AmberEngine::Resources::Model::GetMeshes()
{
	return m_meshes;
}

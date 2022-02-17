#include "Amberpch.h"

#include "AmberEngine/Resources/Model.h"

#include "AmberEngine/Eventing/Event.h"

AmberEngine::Resources::Model::Model(const std::string& p_filePath) : path(p_filePath)
{
	m_materials.fill(nullptr);
}

AmberEngine::Resources::Model::~Model()
{
	for (auto& mesh : m_meshes)
	{
		delete mesh;
		mesh = nullptr;
	}

	for (auto& material : m_materials)
	{
		delete material;
		material = nullptr;
	}

}

void AmberEngine::Resources::Model::SetShader(Shader& p_shader) const
{
	for(const auto material : m_materials)
	{
		if(material)
		{
			material->SetShader(p_shader);
		}
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

std::array<AmberEngine::Resources::Material*, 255>& AmberEngine::Resources::Model::GetMaterials()
{
	return m_materials;
}

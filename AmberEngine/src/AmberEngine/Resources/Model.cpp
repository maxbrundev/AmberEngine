#include "Amberpch.h"

#include "AmberEngine/Resources/Model.h"

#include "AmberEngine/Resources/Mesh.h"
#include "AmberEngine/Resources/Shader.h"

AmberEngine::Resources::Model::Model(std::string_view p_filePath) : path(p_filePath)
{
}

AmberEngine::Resources::Model::~Model()
{
	for (auto mesh : m_meshes)
	{
		delete mesh;
		mesh = nullptr;
	}
}

void AmberEngine::Resources::Model::Bind()
{
	for (auto mesh : m_meshes)
	{
		mesh->callback = [this](std::string uniform, int id) { SetTextureUniform(uniform, id); };
	}
}

void AmberEngine::Resources::Model::SetShader(Shader& shader)
{
	m_shader = &shader;
}

std::vector<AmberEngine::Resources::Mesh*>& AmberEngine::Resources::Model::GetMeshes()
{
	return m_meshes;
}

std::vector<std::string>& AmberEngine::Resources::Model::GetMaterialNames()
{
	return m_materialNames;
}

void AmberEngine::Resources::Model::SetTextureUniform(std::string uniformName, int id)
{
	m_shader->Bind();
	m_shader->SetUniform1i(uniformName, id);
}

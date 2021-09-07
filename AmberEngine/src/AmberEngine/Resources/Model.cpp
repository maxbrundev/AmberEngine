#include "Amberpch.h"

#include "AmberEngine/Resources/Model.h"

#include <chrono>

#include "AmberEngine/Resources/Mesh.h"
#include "AmberEngine/Resources/Shader.h"

#include "AmberEngine/Eventing/Event.h"

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
	m_shader->Bind();

	for (auto mesh : m_meshes)
	{
		// Template arguments deduction in case of curiosity: <void(__thiscall AmberEngine::Resources::Model::*)(class std::basic_string<char, struct std::char_traits<char>, class std::allocator<char> >, int), class AmberEngine::Resources::Model>
		mesh->SetTextureUniformCallback = Eventing::QuickBind(&Model::SetTextureUniform, this);
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
	m_shader->SetUniform1i(uniformName, id);
}

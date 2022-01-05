#include "Amberpch.h"

#include "AmberEngine/Resources/Model.h"

#include "AmberEngine/Buffers/UniformBuffer.h"

#include "AmberEngine/Resources/Mesh.h"
#include "AmberEngine/Resources/Shader.h"

#include "AmberEngine/Eventing/Event.h"

AmberEngine::Resources::Model::Model(const std::string& p_filePath) : path(p_filePath), m_shader(nullptr)
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

void AmberEngine::Resources::Model::Bind()
{
	m_shader->Bind();
}

void AmberEngine::Resources::Model::BindTextureCallback()
{
	for (const auto& mesh : m_meshes)
	{
		// Template arguments deduction in case of curiosity: <void(__thiscall AmberEngine::Resources::Model::*)(class std::basic_string<char, struct std::char_traits<char>, class std::allocator<char> >, int), class AmberEngine::Resources::Model>
		mesh->SetTextureUniformCallback = Eventing::QuickBind(&Model::SetTextureUniform, this);
	}
}

void AmberEngine::Resources::Model::SetTextureUniform(const std::string_view p_uniformName, int p_id) const
{
	m_shader->SetUniform1i(p_uniformName, p_id);
}

void AmberEngine::Resources::Model::SetShader(Shader& p_shader)
{
	m_shader = &p_shader;

	Buffers::UniformBuffer::BindBlockToShader(*m_shader, "EngineUBO");
}

AmberEngine::Resources::Shader* AmberEngine::Resources::Model::GetShader() const
{
	return m_shader;
}

std::vector<AmberEngine::Resources::Mesh*>& AmberEngine::Resources::Model::GetMeshes()
{
	return m_meshes;
}

std::vector<std::string>& AmberEngine::Resources::Model::GetMaterialNames()
{
	return m_materialNames;
}

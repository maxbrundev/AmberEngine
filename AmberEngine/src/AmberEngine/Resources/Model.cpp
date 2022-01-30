#include "Amberpch.h"

#include "AmberEngine/Resources/Model.h"

#include "AmberEngine/Buffers/UniformBuffer.h"

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

	m_shader = nullptr;

}

void AmberEngine::Resources::Model::Bind() const
{
	m_shader->Bind();
}

void AmberEngine::Resources::Model::Unbind() const
{
	m_shader->Unbind();
}

void AmberEngine::Resources::Model::BindTextureCallback()
{
	for (const auto mesh : m_meshes)
	{
		// Template arguments deduction in case of curiosity: <void(__thiscall AmberEngine::Resources::Model::*)(class std::basic_string<char, struct std::char_traits<char>, class std::allocator<char> >, int), class AmberEngine::Resources::Model>
		mesh->SetTextureUniformCallback = Eventing::QuickBind(&Model::SetTextureUniform, this);
	}
}

void AmberEngine::Resources::Model::SetTextureUniform(const std::string_view p_uniformName, uint32_t p_id) const
{
	m_shader->SetUniform1i(p_uniformName, p_id);
}

void AmberEngine::Resources::Model::SetShader(Shader& p_shader)
{
	m_shader = &p_shader;

	Buffers::UniformBuffer::BindBlockToShader(*m_shader, "EngineUBO");
}

void AmberEngine::Resources::Model::SetTexture(Texture& p_texture) const
{
	for(const auto mesh : m_meshes)
	{
		for(auto texture : mesh->GetTextures())
		{
			if(texture->type == p_texture.type)
			{
				texture = std::make_shared<Texture>(p_texture);
			}
		}
	}
}

AmberEngine::Resources::Shader* AmberEngine::Resources::Model::GetShader() const
{
	return m_shader;
}

std::vector<std::string>& AmberEngine::Resources::Model::GetMaterialNames()
{
	return m_materialNames;
}

std::vector<AmberEngine::Resources::Mesh*>& AmberEngine::Resources::Model::GetMeshes()
{
	return m_meshes;
}

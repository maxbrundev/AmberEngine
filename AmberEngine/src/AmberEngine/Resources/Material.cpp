#include "Amberpch.h"

#include "AmberEngine/Resources/Material.h"

#include "AmberEngine/Buffers/UniformBuffer.h"
#include "AmberEngine/Resources/Loaders/TextureLoader.h"

AmberEngine::Resources::Material::~Material()
{
	for (auto& texture : m_textures)
	{
		Loaders::TextureLoader::Delete(texture.get());
	}

	m_textures.clear();

	m_shader = nullptr;
	m_previousShader = nullptr;
}

void AmberEngine::Resources::Material::Bind(const Texture* p_emptyTexture) const
{
	if (HasShader())
	{
		m_shader->Bind();

		uint32_t textureSlot = 0;

		if (!m_textures.empty())
		{
			for (const auto& texture : m_textures)
			{
				switch (texture->type)
				{
				case Settings::ETextureType::DIFFUSE_MAP:
					texture->Bind(textureSlot);
					m_shader->SetUniform1i("u_DiffuseMap", textureSlot++);
					break;
				case Settings::ETextureType::SPECULAR_MAP:
					texture->Bind(textureSlot);
					m_shader->SetUniform1i("u_SpecularMap", textureSlot++);
					break;
				}
			}
		}
		else if (p_emptyTexture)
		{
			p_emptyTexture->Bind(textureSlot);
			m_shader->SetUniform1i("u_DiffuseMap", textureSlot);
			m_shader->SetUniform1i("u_SpecularMap", textureSlot);
		}
	}
}

void AmberEngine::Resources::Material::Unbind() const
{
	if (HasShader())
	{
		m_shader->Unbind();
	}

	for(const auto& texture : m_textures)
	{
		texture->Unbind();
	}
}

void AmberEngine::Resources::Material::FillTextures(const std::vector<std::shared_ptr<Texture>>& p_textures)
{
	m_textures = p_textures;
}

void AmberEngine::Resources::Material::ResetToPreviousShader()
{
	m_shader = m_previousShader;

	if (m_shader)
	{
		Buffers::UniformBuffer::BindBlockToShader(*m_shader, "EngineUBO");
	}
}

void AmberEngine::Resources::Material::SetShader(Shader& p_shader)
{
	if(m_shader == &p_shader)
		return;

	m_previousShader = m_shader;
	m_shader = &p_shader;

	if (m_shader)
	{
		Buffers::UniformBuffer::BindBlockToShader(*m_shader, "EngineUBO");
	}
}

void AmberEngine::Resources::Material::SetTexture(Texture* p_texture)
{
	for(auto& texture : m_textures)
	{
		if(texture->type == p_texture->type)
		{
			texture = std::make_shared<Texture>(*p_texture);
		}
	}
}

void AmberEngine::Resources::Material::SetBlendable(bool p_blendable)
{
	m_blendable = p_blendable;
}

void AmberEngine::Resources::Material::SetBackfaceCulling(bool p_backfaceCulling)
{
	m_backfaceCulling = p_backfaceCulling;
}

void AmberEngine::Resources::Material::SetFrontfaceCulling(bool p_frontfaceCulling)
{
	m_frontfaceCulling = p_frontfaceCulling;
}

void AmberEngine::Resources::Material::SetDepthTest(bool p_depthTest)
{
	m_depthTest = p_depthTest;
}

void AmberEngine::Resources::Material::SetDepthWriting(bool p_depthWriting)
{
	m_depthWriting = p_depthWriting;
}

void AmberEngine::Resources::Material::SetColorWriting(bool p_colorWriting)
{
	m_colorWriting = p_colorWriting;
}

void AmberEngine::Resources::Material::SetGPUInstances(uint64_t p_instances)
{
	m_gpuInstances = p_instances;
}

AmberEngine::Resources::Shader* AmberEngine::Resources::Material::GetShader()
{
	return m_shader;
}

bool AmberEngine::Resources::Material::HasShader() const
{
	return m_shader;
}

std::vector<std::shared_ptr<AmberEngine::Resources::Texture>>& AmberEngine::Resources::Material::GetTextures()
{
	return m_textures;
}

std::vector<std::string>& AmberEngine::Resources::Material::GetMaterialNames()
{
	return m_materialNames;
}

bool AmberEngine::Resources::Material::IsBlendable() const
{
	return m_blendable;
}

bool AmberEngine::Resources::Material::HasBackfaceCulling() const
{
	return m_backfaceCulling;
}

bool AmberEngine::Resources::Material::HasFrontfaceCulling() const
{
	return m_frontfaceCulling;
}

bool AmberEngine::Resources::Material::HasDepthTest() const
{
	return m_depthTest;
}

bool AmberEngine::Resources::Material::HasDepthWriting() const
{
	return m_depthWriting;
}

bool AmberEngine::Resources::Material::HasColorWriting() const
{
	return m_colorWriting;
}

uint64_t AmberEngine::Resources::Material::GetGPUInstances() const
{
	return m_gpuInstances;
}

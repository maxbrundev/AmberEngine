#include "Amberpch.h"

#include "AmberEngine/Resources/Material.h"

#include "AmberEngine/Buffers/UniformBuffer.h"
#include "AmberEngine/Resources/Loaders/TextureLoader.h"

AmberEngine::Resources::Material::Material()
{
	m_textures[ETextureType::DIFFUSE_MAP]  = nullptr;
	m_textures[ETextureType::SPECULAR_MAP] = nullptr;
	m_textures[ETextureType::HEIGHT_MAP]   = nullptr;
	m_textures[ETextureType::NORMAL_MAP]   = nullptr;
	m_textures[ETextureType::MASK_MAP]     = nullptr;
}

AmberEngine::Resources::Material::~Material()
{
	for (auto& [type, texture] : m_textures)
	{
		Loaders::TextureLoader::Delete(texture.get());
	}

	m_textures.clear();
	m_uniformsData.clear();

	m_shader = nullptr;
	m_previousShader = nullptr;
}

void AmberEngine::Resources::Material::Bind(const Texture* p_emptyTexture) const
{
	if (HasShader())
	{
		m_shader->Bind();
	
		int textureSlot = 0;
	
		for (auto& [name, value] : m_uniformsData)
		{
			if (const auto uniformData = m_shader->GetUniformInfo(name))
			{
				switch (uniformData->type)
				{
				case EUniformType::UNIFORM_BOOL:       if (value.type() == typeid(bool))      m_shader->SetUniformInt(name, std::any_cast<bool>(value));      break;
				case EUniformType::UNIFORM_INT:        if (value.type() == typeid(int))       m_shader->SetUniformInt(name, std::any_cast<int>(value));       break;
				case EUniformType::UNIFORM_FLOAT:      if (value.type() == typeid(float))     m_shader->SetUniformFloat(name, std::any_cast<float>(value));   break;
				case EUniformType::UNIFORM_FLOAT_VEC2: if (value.type() == typeid(glm::vec2)) m_shader->SetUniformVec2(name, std::any_cast<glm::vec2>(value)); break;
				case EUniformType::UNIFORM_FLOAT_VEC3: if (value.type() == typeid(glm::vec3)) m_shader->SetUniformVec3(name, std::any_cast<glm::vec3>(value)); break;
				case EUniformType::UNIFORM_FLOAT_VEC4: if (value.type() == typeid(glm::vec4)) m_shader->SetUniformVec4(name, std::any_cast<glm::vec4>(value)); break;
				case EUniformType::UNIFORM_SAMPLER_2D:
					{
						if (value.type() == typeid(Texture*))
						{
							if (auto tex = std::any_cast<Texture*>(value); tex)
							{
								tex->Bind(textureSlot);
								m_shader->SetUniformInt(uniformData->name, textureSlot++);
							}
							else if (p_emptyTexture)
							{
								if (!hasSpecularMap && uniformData->name == "u_SpecularMap")
									continue;

								p_emptyTexture->Bind(textureSlot);
								m_shader->SetUniformInt(uniformData->name, textureSlot++);
							}
						}
					}
				}
			}
		}
	}
}

void AmberEngine::Resources::Material::Unbind() const
{
	if (HasShader())
	{
		m_shader->Unbind();
	}

	for(const auto& [type, texture] : m_textures)
	{
		texture->Unbind();
	}
}

void AmberEngine::Resources::Material::FillTextures(std::unordered_map<ETextureType, std::shared_ptr<Texture>> p_textures)
{
	m_textures = std::move(p_textures);
}

void AmberEngine::Resources::Material::ResetToPreviousShader()
{
	m_shader = m_previousShader;

	if (m_shader)
	{
		Buffers::UniformBuffer::BindBlockToShader(*m_shader, "EngineUBO");
		FillUniform();
	}
}

void AmberEngine::Resources::Material::SetShader(Shader* p_shader)
{
	if (m_shader == p_shader)
		return;

	m_previousShader = m_shader;
	m_shader = p_shader;

	if (m_shader)
	{
		Buffers::UniformBuffer::BindBlockToShader(*m_shader, "EngineUBO");
		FillUniform();
	}
	else
	{
		m_uniformsData.clear();
	}
}

void AmberEngine::Resources::Material::SetTexture(Texture* p_texture)
{
	for(auto& [type, texture] : m_textures)
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

void AmberEngine::Resources::Material::SetName(std::string p_name)
{
	m_name = std::move(p_name);
}

const AmberEngine::Resources::Shader* AmberEngine::Resources::Material::GetShader() const
{
	return m_shader;
}

bool AmberEngine::Resources::Material::HasShader() const
{
	return m_shader != nullptr;
}

void AmberEngine::Resources::Material::FillUniform()
{
	m_uniformsData.clear();

	for(uint32_t i = 0; i < m_shader->uniforms.size(); i++)
	{
		std::any uniformDefaultValue = m_shader->uniforms[i].defaultValue;

		if(m_shader->uniforms[i].type == EUniformType::UNIFORM_SAMPLER_2D && !m_textures.empty())
		{
			ETextureType textureType;

			if(m_shader->uniforms[i].name == "u_DiffuseMap")
			{
				textureType = ETextureType::DIFFUSE_MAP;
			}
			else if(m_shader->uniforms[i].name == "u_SpecularMap")
			{
				textureType = ETextureType::SPECULAR_MAP;
			}

			auto predicate = [&](const std::pair<ETextureType, std::shared_ptr<Texture>>& p_texture)
			{
				return p_texture.first == textureType;
			};

			switch (textureType)
			{
			case ETextureType::DIFFUSE_MAP:
				{
					const auto& iterator = std::find_if(std::begin(m_textures), std::end(m_textures), predicate);

					if (iterator != m_textures.end() && iterator->second)
						uniformDefaultValue = &*iterator->second;
				}
				break;

			case ETextureType::SPECULAR_MAP:
				{
					const auto& iterator = std::find_if(std::begin(m_textures), std::end(m_textures), predicate);

					if (iterator != m_textures.end() && iterator->second)
						uniformDefaultValue = &*iterator->second;
					else
						hasSpecularMap = false;
				}
				break;
			}
		}

		m_uniformsData.emplace(m_shader->uniforms[i].name, uniformDefaultValue);
	}
}

const std::unordered_map<AmberEngine::Resources::ETextureType, std::shared_ptr<AmberEngine::Resources::Texture>>& AmberEngine::Resources::Material::GetTextures()
{
	return m_textures;
}

const std::string& AmberEngine::Resources::Material::GetName()
{
	return m_name;
}

const std::map<std::string, std::any>& AmberEngine::Resources::Material::GetUniformsData()
{
	return m_uniformsData;
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

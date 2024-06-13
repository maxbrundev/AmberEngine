#include "Amberpch.h"

#include "AmberEngine/Resources/Material.h"

#include "AmberEngine/Buffers/UniformBuffer.h"
#include "AmberEngine/Resources/Loaders/TextureLoader.h"

AmberEngine::Resources::Material::Material()
{
}

AmberEngine::Resources::Material::~Material()
{
	m_uniformsData.clear();

	m_shader = nullptr;
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
								if (uniformData->name == "u_SpecularMap")
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
}

void AmberEngine::Resources::Material::SetShader(Shader* p_shader)
{
	if (m_shader == p_shader)
		return;

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

void AmberEngine::Resources::Material::SetBlendable(bool p_blendable)
{
	m_blendable = p_blendable;
}

void AmberEngine::Resources::Material::SetBackFaceCulling(bool p_backFaceCulling)
{
	m_backFaceCulling = p_backFaceCulling;
}

void AmberEngine::Resources::Material::SetFrontFaceCulling(bool p_frontFaceCulling)
{
	m_frontFaceCulling = p_frontFaceCulling;
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

	for (const UniformInfo& element : m_shader->Uniforms)
		m_uniformsData.emplace(element.name, element.defaultValue);
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

bool AmberEngine::Resources::Material::HasBackFaceCulling() const
{
	return m_backFaceCulling;
}

bool AmberEngine::Resources::Material::HasFrontFaceCulling() const
{
	return m_frontFaceCulling;
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

int AmberEngine::Resources::Material::GetGPUInstances() const
{
	return m_gpuInstances;
}

uint8_t AmberEngine::Resources::Material::GenerateStateMask() const
{
	uint8_t result = 0;

	if (m_depthWriting)								result |= 0b0000'0001;
	if (m_colorWriting)								result |= 0b0000'0010;
	if (m_blendable)								result |= 0b0000'0100;
	if (m_backFaceCulling || m_frontFaceCulling)	result |= 0b0000'1000;
	if (m_depthTest)								result |= 0b0001'0000;
	if (m_backFaceCulling)							result |= 0b0010'0000;
	if (m_frontFaceCulling)							result |= 0b0100'0000;

	return result;
}

void AmberEngine::Resources::Material::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
}

void AmberEngine::Resources::Material::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
}

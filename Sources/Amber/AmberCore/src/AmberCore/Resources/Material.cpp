#include "Amberpch.h"

#include "AmberCore/Resources/Material.h"

#include "AmberRendering/Buffers/UniformBuffer.h"
#include "AmberCore/Helpers/Serializer.h"
#include "AmberRendering/Resources/Loaders/TextureLoader.h"

AmberCore::Resources::Material::Material()
{
}

AmberCore::Resources::Material::~Material()
{
	m_uniformsData.clear();

	m_shader = nullptr;
}

void AmberCore::Resources::Material::Bind(const AmberRendering::Resources::Texture* p_emptyTexture) const
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
				case AmberRendering::Resources::EUniformType::UNIFORM_BOOL:       if (value.type() == typeid(bool))      m_shader->SetUniformInt(name, std::any_cast<bool>(value));      break;
				case AmberRendering::Resources::EUniformType::UNIFORM_INT:        if (value.type() == typeid(int))       m_shader->SetUniformInt(name, std::any_cast<int>(value));       break;
				case AmberRendering::Resources::EUniformType::UNIFORM_FLOAT:      if (value.type() == typeid(float))     m_shader->SetUniformFloat(name, std::any_cast<float>(value));   break;
				case AmberRendering::Resources::EUniformType::UNIFORM_FLOAT_VEC2: if (value.type() == typeid(glm::vec2)) m_shader->SetUniformVec2(name, std::any_cast<glm::vec2>(value)); break;
				case AmberRendering::Resources::EUniformType::UNIFORM_FLOAT_VEC3: if (value.type() == typeid(glm::vec3)) m_shader->SetUniformVec3(name, std::any_cast<glm::vec3>(value)); break;
				case AmberRendering::Resources::EUniformType::UNIFORM_FLOAT_VEC4: if (value.type() == typeid(glm::vec4)) m_shader->SetUniformVec4(name, std::any_cast<glm::vec4>(value)); break;
				case AmberRendering::Resources::EUniformType::UNIFORM_SAMPLER_2D:
					{
						if (value.type() == typeid(AmberRendering::Resources::Texture*))
						{
							if (auto tex = std::any_cast<AmberRendering::Resources::Texture*>(value); tex)
							{
								tex->Bind(textureSlot);
								m_shader->SetUniformInt(uniformData->name, textureSlot++);
							}
							else if (p_emptyTexture && uniformData->name == "u_DiffuseMap")
							{
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

void AmberCore::Resources::Material::Unbind() const
{
	if (HasShader())
	{
		int textureSlot = 0;

		for (auto&[name, value] : m_uniformsData)
		{
			if (const auto uniformData = m_shader->GetUniformInfo(name))
			{
				switch (uniformData->type)
				{
				case AmberRendering::Resources::EUniformType::UNIFORM_SAMPLER_2D:
					{
						if (value.type() == typeid(AmberRendering::Resources::Texture*))
						{
							if (auto tex = std::any_cast<AmberRendering::Resources::Texture*>(value); tex)
							{
								tex->Unbind();
							}
						}
					}
				}
			}
		}

		m_shader->Unbind();
	}
}

void AmberCore::Resources::Material::SetShader(AmberRendering::Resources::Shader* p_shader)
{
	if (m_shader == p_shader)
		return;

	m_shader = p_shader;

	if (m_shader)
	{
		AmberRendering::Buffers::UniformBuffer::BindBlockToShader(*m_shader, "EngineUBO");
		FillUniform();
	}
	else
	{
		m_uniformsData.clear();
	}
}

void AmberCore::Resources::Material::SetBlendable(bool p_blendable)
{
	m_blendable = p_blendable;
}

void AmberCore::Resources::Material::SetBackFaceCulling(bool p_backFaceCulling)
{
	m_backFaceCulling = p_backFaceCulling;
}

void AmberCore::Resources::Material::SetFrontFaceCulling(bool p_frontFaceCulling)
{
	m_frontFaceCulling = p_frontFaceCulling;
}

void AmberCore::Resources::Material::SetDepthTest(bool p_depthTest)
{
	m_depthTest = p_depthTest;
}

void AmberCore::Resources::Material::SetDepthWriting(bool p_depthWriting)
{
	m_depthWriting = p_depthWriting;
}

void AmberCore::Resources::Material::SetColorWriting(bool p_colorWriting)
{
	m_colorWriting = p_colorWriting;
}

void AmberCore::Resources::Material::SetGPUInstances(uint64_t p_instances)
{
	m_gpuInstances = p_instances;
}

void AmberCore::Resources::Material::SetName(std::string p_name)
{
	m_name = std::move(p_name);
}

AmberRendering::Resources::Shader* AmberCore::Resources::Material::GetShader() const
{
	return m_shader;
}

bool AmberCore::Resources::Material::HasShader() const
{
	return m_shader != nullptr;
}

void AmberCore::Resources::Material::FillUniform()
{
	m_uniformsData.clear();

	for (const AmberRendering::Resources::UniformInfo& element : m_shader->Uniforms)
		m_uniformsData.emplace(element.name, element.defaultValue);
}


const std::string& AmberCore::Resources::Material::GetName()
{
	return m_name;
}

std::map<std::string, std::any>& AmberCore::Resources::Material::GetUniformsData()
{
	return m_uniformsData;
}

bool AmberCore::Resources::Material::IsBlendable() const
{
	return m_blendable;
}

bool AmberCore::Resources::Material::HasBackFaceCulling() const
{
	return m_backFaceCulling;
}

bool AmberCore::Resources::Material::HasFrontFaceCulling() const
{
	return m_frontFaceCulling;
}

bool AmberCore::Resources::Material::HasDepthTest() const
{
	return m_depthTest;
}

bool AmberCore::Resources::Material::HasDepthWriting() const
{
	return m_depthWriting;
}

bool AmberCore::Resources::Material::HasColorWriting() const
{
	return m_colorWriting;
}

int AmberCore::Resources::Material::GetGPUInstances() const
{
	return m_gpuInstances;
}

uint8_t AmberCore::Resources::Material::GenerateStateMask() const
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

void AmberCore::Resources::Material::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	AmberCore::Helpers::Serializer::SerializeShader(p_doc, p_node, "shader", m_shader);

	tinyxml2::XMLNode* settingsNode = p_doc.NewElement("settings");
	p_node->InsertEndChild(settingsNode);

	AmberCore::Helpers::Serializer::SerializeBoolean(p_doc, settingsNode, "blendable", m_blendable);
	AmberCore::Helpers::Serializer::SerializeBoolean(p_doc, settingsNode, "backface_culling", m_backFaceCulling);
	AmberCore::Helpers::Serializer::SerializeBoolean(p_doc, settingsNode, "frontface_culling", m_frontFaceCulling);
	AmberCore::Helpers::Serializer::SerializeBoolean(p_doc, settingsNode, "depth_test", m_depthTest);
	AmberCore::Helpers::Serializer::SerializeBoolean(p_doc, settingsNode, "depth_writing", m_depthWriting);
	AmberCore::Helpers::Serializer::SerializeBoolean(p_doc, settingsNode, "color_writing", m_colorWriting);
	AmberCore::Helpers::Serializer::SerializeInt(p_doc, settingsNode, "gpu_instances", m_gpuInstances);

	// Create "Uniforms" (Every uniform will be attached to "Uniforms")
	tinyxml2::XMLNode* uniformsNode = p_doc.NewElement("uniforms");
	p_node->InsertEndChild(uniformsNode);

	for (const auto&[uniformName, uniformValue] : m_uniformsData)
	{
		tinyxml2::XMLNode* uniform = p_doc.NewElement("uniform");
		uniformsNode->InsertEndChild(uniform); // Instead of p_node, use uniformNode (To create)

		const AmberRendering::Resources::UniformInfo* uniformInfo = m_shader->GetUniformInfo(uniformName);

		AmberCore::Helpers::Serializer::SerializeString(p_doc, uniform, "name", uniformName);

		if (uniformInfo && uniformValue.has_value())
		{
			switch (uniformInfo->type)
			{
			case AmberRendering::Resources::EUniformType::UNIFORM_BOOL:
				if (uniformValue.type() == typeid(bool)) AmberCore::Helpers::Serializer::SerializeInt(p_doc, uniform, "value", std::any_cast<bool>(uniformValue));
				break;

			case AmberRendering::Resources::EUniformType::UNIFORM_INT:
				if (uniformValue.type() == typeid(int)) AmberCore::Helpers::Serializer::SerializeInt(p_doc, uniform, "value", std::any_cast<int>(uniformValue));
				break;

			case AmberRendering::Resources::EUniformType::UNIFORM_FLOAT:
				if (uniformValue.type() == typeid(float)) AmberCore::Helpers::Serializer::SerializeFloat(p_doc, uniform, "value", std::any_cast<float>(uniformValue));
				break;

			case AmberRendering::Resources::EUniformType::UNIFORM_FLOAT_VEC2:
				if (uniformValue.type() == typeid(glm::vec2)) AmberCore::Helpers::Serializer::SerializeVec2(p_doc, uniform, "value", std::any_cast<glm::vec2>(uniformValue));
				break;

			case AmberRendering::Resources::EUniformType::UNIFORM_FLOAT_VEC3:
				if (uniformValue.type() == typeid(glm::vec3)) AmberCore::Helpers::Serializer::SerializeVec3(p_doc, uniform, "value", std::any_cast<glm::vec3>(uniformValue));
				break;

			case AmberRendering::Resources::EUniformType::UNIFORM_FLOAT_VEC4:
				if (uniformValue.type() == typeid(glm::vec4)) AmberCore::Helpers::Serializer::SerializeVec4(p_doc, uniform, "value", std::any_cast<glm::vec4>(uniformValue));
				break;

			case AmberRendering::Resources::EUniformType::UNIFORM_SAMPLER_2D:
				if (uniformValue.type() == typeid(AmberRendering::Resources::Texture*)) AmberCore::Helpers::Serializer::SerializeTexture(p_doc, uniform, "value", std::any_cast<AmberRendering::Resources::Texture*>(uniformValue));
				break;
			}
		}
	}
}

void AmberCore::Resources::Material::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	tinyxml2::XMLNode* settingsNode = p_node->FirstChildElement("settings");

	if (settingsNode)
	{
		AmberCore::Helpers::Serializer::DeserializeBoolean(p_doc, settingsNode, "blendable", m_blendable);
		AmberCore::Helpers::Serializer::DeserializeBoolean(p_doc, settingsNode, "backface_culling", m_backFaceCulling);
		AmberCore::Helpers::Serializer::DeserializeBoolean(p_doc, settingsNode, "frontface_culling", m_frontFaceCulling);
		AmberCore::Helpers::Serializer::DeserializeBoolean(p_doc, settingsNode, "depth_test", m_depthTest);
		AmberCore::Helpers::Serializer::DeserializeBoolean(p_doc, settingsNode, "depth_writing", m_depthWriting);
		AmberCore::Helpers::Serializer::DeserializeBoolean(p_doc, settingsNode, "color_writing", m_colorWriting);
		AmberCore::Helpers::Serializer::DeserializeInt(p_doc, settingsNode, "gpu_instances", m_gpuInstances);
	}

	/* We get the shader with Deserialize method */
	AmberRendering::Resources::Shader* deserializedShader = AmberCore::Helpers::Serializer::DeserializeShader(p_doc, p_node, "shader");

	/* We verify that the shader is valid (Not null) */
	if (deserializedShader)
	{
		/* If the shader is valid, we set it to the material (Modify m_shader pointer + Query + FillUniforms) */
		SetShader(deserializedShader);

		tinyxml2::XMLNode* uniformsNode = p_node->FirstChildElement("uniforms"); // Access to "Uniforms" (Every uniform will be attached to "Uniforms")		

		if (uniformsNode)
		{
			/* We iterate over every <uniform>...</uniform> */
			for (auto uniform = uniformsNode->FirstChildElement("uniform"); uniform; uniform = uniform->NextSiblingElement("uniform"))
			{
				/* Verify that the uniform node contains a "name" element */
				if (auto uniformNameElement = uniform->FirstChildElement("name"); uniformNameElement)
				{
					const std::string uniformName = uniformNameElement->GetText();

					/* We collect information about the uniform (The uniform is identified in the shader by its name) */
					const AmberRendering::Resources::UniformInfo* uniformInfo = m_shader->GetUniformInfo(uniformName);

					/* We verify that the uniform is existant is the current shader */
					if (uniformInfo)
					{
						/* Deserialization of the uniform value depending on the uniform type (Deserialization result to std::any) */
						switch (uniformInfo->type)
						{
						case AmberRendering::Resources::EUniformType::UNIFORM_BOOL:
							m_uniformsData[uniformInfo->name] = AmberCore::Helpers::Serializer::DeserializeBoolean(p_doc, uniform, "value");
							break;

						case AmberRendering::Resources::EUniformType::UNIFORM_INT:
							m_uniformsData[uniformInfo->name] = AmberCore::Helpers::Serializer::DeserializeInt(p_doc, uniform, "value");
							break;

						case AmberRendering::Resources::EUniformType::UNIFORM_FLOAT:
							m_uniformsData[uniformInfo->name] = AmberCore::Helpers::Serializer::DeserializeFloat(p_doc, uniform, "value");
							break;

						case AmberRendering::Resources::EUniformType::UNIFORM_FLOAT_VEC2:
							m_uniformsData[uniformInfo->name] = AmberCore::Helpers::Serializer::DeserializeVec2(p_doc, uniform, "value");
							break;

						case AmberRendering::Resources::EUniformType::UNIFORM_FLOAT_VEC3:
							m_uniformsData[uniformInfo->name] = AmberCore::Helpers::Serializer::DeserializeVec3(p_doc, uniform, "value");
							break;

						case AmberRendering::Resources::EUniformType::UNIFORM_FLOAT_VEC4:
							m_uniformsData[uniformInfo->name] = AmberCore::Helpers::Serializer::DeserializeVec4(p_doc, uniform, "value");
							break;

						case AmberRendering::Resources::EUniformType::UNIFORM_SAMPLER_2D:
							m_uniformsData[uniformInfo->name] = AmberCore::Helpers::Serializer::DeserializeTexture(p_doc, uniform, "value");
							break;
						}
					}
				}
			}
		}
	}
}

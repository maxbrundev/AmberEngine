#include "Amberpch.h"

#include "AmberEngine/Resources/Material.h"

#include "AmberEngine/Buffers/UniformBuffer.h"
#include "AmberEngine/Core/Helpers/Serializer.h"
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

void AmberEngine::Resources::Material::Unbind() const
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
				case EUniformType::UNIFORM_SAMPLER_2D:
					{
						if (value.type() == typeid(Texture*))
						{
							if (auto tex = std::any_cast<Texture*>(value); tex)
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

AmberEngine::Resources::Shader* AmberEngine::Resources::Material::GetShader() const
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

std::map<std::string, std::any>& AmberEngine::Resources::Material::GetUniformsData()
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
	Core::Helpers::Serializer::SerializeShader(p_doc, p_node, "shader", m_shader);

	tinyxml2::XMLNode* settingsNode = p_doc.NewElement("settings");
	p_node->InsertEndChild(settingsNode);

	Core::Helpers::Serializer::SerializeBoolean(p_doc, settingsNode, "blendable", m_blendable);
	Core::Helpers::Serializer::SerializeBoolean(p_doc, settingsNode, "backface_culling", m_backFaceCulling);
	Core::Helpers::Serializer::SerializeBoolean(p_doc, settingsNode, "frontface_culling", m_frontFaceCulling);
	Core::Helpers::Serializer::SerializeBoolean(p_doc, settingsNode, "depth_test", m_depthTest);
	Core::Helpers::Serializer::SerializeBoolean(p_doc, settingsNode, "depth_writing", m_depthWriting);
	Core::Helpers::Serializer::SerializeBoolean(p_doc, settingsNode, "color_writing", m_colorWriting);
	Core::Helpers::Serializer::SerializeInt(p_doc, settingsNode, "gpu_instances", m_gpuInstances);

	// Create "Uniforms" (Every uniform will be attached to "Uniforms")
	tinyxml2::XMLNode* uniformsNode = p_doc.NewElement("uniforms");
	p_node->InsertEndChild(uniformsNode);

	for (const auto&[uniformName, uniformValue] : m_uniformsData)
	{
		tinyxml2::XMLNode* uniform = p_doc.NewElement("uniform");
		uniformsNode->InsertEndChild(uniform); // Instead of p_node, use uniformNode (To create)

		const Resources::UniformInfo* uniformInfo = m_shader->GetUniformInfo(uniformName);

		Core::Helpers::Serializer::SerializeString(p_doc, uniform, "name", uniformName);

		if (uniformInfo && uniformValue.has_value())
		{
			switch (uniformInfo->type)
			{
			case EUniformType::UNIFORM_BOOL:
				if (uniformValue.type() == typeid(bool)) Core::Helpers::Serializer::SerializeInt(p_doc, uniform, "value", std::any_cast<bool>(uniformValue));
				break;

			case EUniformType::UNIFORM_INT:
				if (uniformValue.type() == typeid(int)) Core::Helpers::Serializer::SerializeInt(p_doc, uniform, "value", std::any_cast<int>(uniformValue));
				break;

			case EUniformType::UNIFORM_FLOAT:
				if (uniformValue.type() == typeid(float)) Core::Helpers::Serializer::SerializeFloat(p_doc, uniform, "value", std::any_cast<float>(uniformValue));
				break;

			case EUniformType::UNIFORM_FLOAT_VEC2:
				if (uniformValue.type() == typeid(glm::vec2)) Core::Helpers::Serializer::SerializeVec2(p_doc, uniform, "value", std::any_cast<glm::vec2>(uniformValue));
				break;

			case EUniformType::UNIFORM_FLOAT_VEC3:
				if (uniformValue.type() == typeid(glm::vec3)) Core::Helpers::Serializer::SerializeVec3(p_doc, uniform, "value", std::any_cast<glm::vec3>(uniformValue));
				break;

			case EUniformType::UNIFORM_FLOAT_VEC4:
				if (uniformValue.type() == typeid(glm::vec4)) Core::Helpers::Serializer::SerializeVec4(p_doc, uniform, "value", std::any_cast<glm::vec4>(uniformValue));
				break;

			case EUniformType::UNIFORM_SAMPLER_2D:
				if (uniformValue.type() == typeid(Texture*)) Core::Helpers::Serializer::SerializeTexture(p_doc, uniform, "value", std::any_cast<Texture*>(uniformValue));
				break;
			}
		}
	}
}

void AmberEngine::Resources::Material::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	tinyxml2::XMLNode* settingsNode = p_node->FirstChildElement("settings");

	if (settingsNode)
	{
		Core::Helpers::Serializer::DeserializeBoolean(p_doc, settingsNode, "blendable", m_blendable);
		Core::Helpers::Serializer::DeserializeBoolean(p_doc, settingsNode, "backface_culling", m_backFaceCulling);
		Core::Helpers::Serializer::DeserializeBoolean(p_doc, settingsNode, "frontface_culling", m_frontFaceCulling);
		Core::Helpers::Serializer::DeserializeBoolean(p_doc, settingsNode, "depth_test", m_depthTest);
		Core::Helpers::Serializer::DeserializeBoolean(p_doc, settingsNode, "depth_writing", m_depthWriting);
		Core::Helpers::Serializer::DeserializeBoolean(p_doc, settingsNode, "color_writing", m_colorWriting);
		Core::Helpers::Serializer::DeserializeInt(p_doc, settingsNode, "gpu_instances", m_gpuInstances);
	}

	/* We get the shader with Deserialize method */
	Resources::Shader* deserializedShader = Core::Helpers::Serializer::DeserializeShader(p_doc, p_node, "shader");

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
					const Resources::UniformInfo* uniformInfo = m_shader->GetUniformInfo(uniformName);

					/* We verify that the uniform is existant is the current shader */
					if (uniformInfo)
					{
						/* Deserialization of the uniform value depending on the uniform type (Deserialization result to std::any) */
						switch (uniformInfo->type)
						{
						case Resources::EUniformType::UNIFORM_BOOL:
							m_uniformsData[uniformInfo->name] = Core::Helpers::Serializer::DeserializeBoolean(p_doc, uniform, "value");
							break;

						case Resources::EUniformType::UNIFORM_INT:
							m_uniformsData[uniformInfo->name] = Core::Helpers::Serializer::DeserializeInt(p_doc, uniform, "value");
							break;

						case Resources::EUniformType::UNIFORM_FLOAT:
							m_uniformsData[uniformInfo->name] = Core::Helpers::Serializer::DeserializeFloat(p_doc, uniform, "value");
							break;

						case Resources::EUniformType::UNIFORM_FLOAT_VEC2:
							m_uniformsData[uniformInfo->name] = Core::Helpers::Serializer::DeserializeVec2(p_doc, uniform, "value");
							break;

						case Resources::EUniformType::UNIFORM_FLOAT_VEC3:
							m_uniformsData[uniformInfo->name] = Core::Helpers::Serializer::DeserializeVec3(p_doc, uniform, "value");
							break;

						case Resources::EUniformType::UNIFORM_FLOAT_VEC4:
							m_uniformsData[uniformInfo->name] = Core::Helpers::Serializer::DeserializeVec4(p_doc, uniform, "value");
							break;

						case Resources::EUniformType::UNIFORM_SAMPLER_2D:
							m_uniformsData[uniformInfo->name] = Core::Helpers::Serializer::DeserializeTexture(p_doc, uniform, "value");
							break;
						}
					}
				}
			}
		}
	}
}

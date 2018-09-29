#include "pch.h"

#include "Resources/Material.h"

RenderEngine::Resources::Material::Material(Resources::Shader* p_shader, Resources::Texture* p_texture) : m_shader(p_shader), m_texture(p_texture),
m_ambient(1.0f, 1.0f, 1.0f), m_diffuse(1.0f, 1.0f, 1.0f), m_specular(1.0f, 1.0f, 1.0f), m_shininess(32.0f)
{
	m_shader->SetUniformVec3("material.ambient", m_ambient);
	m_shader->SetUniformVec3("material.diffuse", m_diffuse);
	m_shader->SetUniformVec3("material.specular", m_specular);
	m_shader->SetUniform1f("material.shininess", m_shininess);

	if (m_texture)
	{
		//TODO
	}
}
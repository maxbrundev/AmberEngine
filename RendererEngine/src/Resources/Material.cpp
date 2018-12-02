#include "pch.h"

#include "Resources/Material.h"
#include "Resources/Shader.h"

RenderEngine::Resources::Material::Material(glm::vec3& p_ambient, glm::vec3& p_diffuse, glm::vec3& p_specular,
	float p_shininess, GLuint p_diffuseMap, GLuint p_specularMap, GLuint p_emissive) : m_ambient(p_ambient),
	                                                                                   m_diffuse(p_diffuse), 
																					   m_specular(p_specular),
																					   m_diffuseMap(p_diffuseMap),
	                                                                                   m_specularMap(p_specularMap),
	                                                                                   m_emissiveMap(p_emissive),
	                                                                                   m_shininess(p_shininess)
{
	//TODO
}

void RenderEngine::Resources::Material::SendToShader(Shader& p_shader)
{
	p_shader.Bind();
}
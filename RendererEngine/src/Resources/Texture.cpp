#include "pch.h"

#include "Resources/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


RenderEngine::Resources::Texture::Texture(const std::string& path) : m_RendererID(0), m_Width(0), m_Height(0), m_BPP(0), m_LocalBuffer(nullptr), m_FilePath(path)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

RenderEngine::Resources::Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void RenderEngine::Resources::Texture::Bind(unsigned slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void RenderEngine::Resources::Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
#include "Amberpch.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "AmberEngine/Resources/Texture.h"

#include "AmberEngine/Debug/GLDebug.h"
#include "AmberEngine/Tools/Utils/String.h"

AmberEngine::Resources::Texture::Texture(const std::string& p_path, bool p_flipVertically) : m_id(0), m_width(0), m_height(0), m_bitsPerPixel(0), m_path(p_path)
{
	GLuint textureID;
	int textureWidth;
	int textureHeight;
	int bitsPerPixel;
	
	GLCall(glGenTextures(1, &textureID));
	
	stbi_set_flip_vertically_on_load(p_flipVertically);
	unsigned char* dataBuffer = stbi_load(m_path.data(), &textureWidth, &textureHeight, &bitsPerPixel, 4);

	m_id           = textureID;
	m_width        = textureWidth;
	m_height       = textureHeight;
	m_bitsPerPixel = bitsPerPixel;

	m_name = Utils::String::ExtractFileNameFromPath(m_path);
	
	if (dataBuffer)
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, m_id));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataBuffer));
		glGenerateMipmap(GL_TEXTURE_2D);

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		stbi_image_free(dataBuffer);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << m_path << std::endl;
		stbi_image_free(dataBuffer);
	}
	
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

AmberEngine::Resources::Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_id));
}

void AmberEngine::Resources::Texture::Bind(uint32_t p_slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + p_slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_id));
}

void AmberEngine::Resources::Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

#include "Amberpch.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "AmberEngine/Resources/Texture.h"

#include "AmberEngine/Debug/GLDebug.h"

AmberEngine::Resources::Texture::Texture(const std::string& p_filePath) : m_textureId(0), m_path(p_filePath), m_width(0), m_height(0), m_bitsPerPixel(0)
{
	GLCall(glGenTextures(1, &m_textureId));

	stbi_set_flip_vertically_on_load(true);
	unsigned char* dataBuffer = stbi_load(p_filePath.c_str(), &m_width, &m_height, &m_bitsPerPixel, 0);

	if (dataBuffer)
	{
		GLenum format;
		if (m_bitsPerPixel == 1)
			format = GL_RED;
		else if (m_bitsPerPixel == 3)
			format = GL_RGB;
		else if (m_bitsPerPixel == 4)
			format = GL_RGBA;

		GLCall(glBindTexture(GL_TEXTURE_2D, m_textureId));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, dataBuffer));
		glGenerateMipmap(GL_TEXTURE_2D);

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		stbi_image_free(dataBuffer);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << p_filePath << std::endl;
		stbi_image_free(dataBuffer);
	}

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

AmberEngine::Resources::Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_textureId));
}

void AmberEngine::Resources::Texture::Bind(const unsigned int& p_slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + p_slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_textureId));
}

void AmberEngine::Resources::Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

GLuint AmberEngine::Resources::Texture::GetTextureId()
{
	return m_textureId;
}

int AmberEngine::Resources::Texture::GetTextureWidth()
{
	return m_width;
}

int AmberEngine::Resources::Texture::GetTextureHeight()
{
	return m_height;
}
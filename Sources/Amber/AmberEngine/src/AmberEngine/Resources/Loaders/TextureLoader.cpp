#include "Amberpch.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "AmberEngine/Resources/Loaders/TextureLoader.h"

#include "AmberEngine/Tools/Utils/String.h"

std::string AmberEngine::Resources::Loaders::TextureLoader::FILE_TRACE;

AmberEngine::Resources::Texture* AmberEngine::Resources::Loaders::TextureLoader::Create(std::string p_filePath, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, ETextureType p_textureType, bool p_flipVertically, bool p_generateMipmap)
{
	FILE_TRACE = p_filePath;

	GLuint textureID;
	int textureWidth;
	int textureHeight;
	int bitsPerPixel;

	glGenTextures(1, &textureID);

	stbi_set_flip_vertically_on_load(p_flipVertically);

	if (unsigned char* dataBuffer = stbi_load(p_filePath.c_str(), &textureWidth, &textureHeight, &bitsPerPixel, 4); dataBuffer)
	{
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataBuffer);

		if(p_generateMipmap)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(p_firstFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(p_secondFilter));

		stbi_image_free(dataBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);

		std::string name = Tools::Utils::String::ExtractFileNameFromPath(p_filePath);

		return new Texture(p_filePath, std::move(name), textureID, textureWidth, textureHeight, bitsPerPixel, p_firstFilter, p_secondFilter, p_textureType, p_generateMipmap);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << FILE_TRACE << std::endl;

		stbi_image_free(dataBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	return nullptr;
}

AmberEngine::Resources::Texture* AmberEngine::Resources::Loaders::TextureLoader::CreateColor(uint32_t p_data, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, bool p_generateMipmap)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &p_data);

	if (p_generateMipmap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(p_firstFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(p_secondFilter));

	glBindTexture(GL_TEXTURE_2D, 0);

	return new Texture("", "ColorTexture", textureID, 1, 1, 32, p_firstFilter, p_secondFilter, ETextureType::DIFFUSE_MAP, p_generateMipmap);
}

AmberEngine::Resources::Texture* AmberEngine::Resources::Loaders::TextureLoader::CreateFromMemory(uint8_t* p_data, uint32_t p_width, uint32_t p_height, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, bool p_generateMipmap)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, p_width, p_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, p_data);

	if (p_generateMipmap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(p_firstFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(p_secondFilter));

	glBindTexture(GL_TEXTURE_2D, 0);

	return new Texture("", "", textureID, 1, 1, 32, p_firstFilter, p_secondFilter, ETextureType::DIFFUSE_MAP, p_generateMipmap);
}

void AmberEngine::Resources::Loaders::TextureLoader::Reload(Texture& p_texture, const std::string& p_filePath, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, ETextureType p_textureType, bool p_flipVertically, bool p_generateMipmap)
{
	Texture* newTexture = Create(p_filePath, p_firstFilter, p_secondFilter, p_textureType, p_flipVertically, p_generateMipmap);

	if (newTexture)
	{
		glDeleteTextures(1, &p_texture.ID);

		*const_cast<uint32_t*>(&p_texture.ID)                                  = newTexture->ID;
		*const_cast<uint32_t*>(&p_texture.Width)                               = newTexture->Width;
		*const_cast<uint32_t*>(&p_texture.Height)                              = newTexture->Height;
		*const_cast<uint32_t*>(&p_texture.BitsPerPixel)                        = newTexture->BitsPerPixel;
		*const_cast<ETextureFilteringMode*>(&p_texture.FirstFilter)            = newTexture->FirstFilter;
		*const_cast<ETextureFilteringMode*>(&p_texture.SecondFilter)           = newTexture->SecondFilter;
		*const_cast<bool*>(&p_texture.HasMipmaps)                              = newTexture->HasMipmaps;

		delete newTexture;
	}
}

bool AmberEngine::Resources::Loaders::TextureLoader::Destroy(Texture*& p_textureInstance)
{
	if (p_textureInstance)
	{
		glDeleteTextures(1, &p_textureInstance->ID);

		delete p_textureInstance;
		p_textureInstance = nullptr;

		return true;
	}

	return false;
}

bool AmberEngine::Resources::Loaders::TextureLoader::Delete(Texture* p_textureInstance)
{
	if (p_textureInstance)
	{
		glDeleteTextures(1, &p_textureInstance->ID);

		return true;
	}

	return false;
}

#include "Amberpch.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "AmberEngine/Resources/Loaders/TextureLoader.h"

#include "AmberEngine/Debug/GLDebug.h"
#include "AmberEngine/Tools/Utils/String.h"

AmberEngine::Resources::Texture* AmberEngine::Resources::TextureLoader::Create(std::string p_filePath,
	AmberEngine::Settings::ETextureFilteringMode p_firstFilter,
	AmberEngine::Settings::ETextureFilteringMode p_secondFilter,
	AmberEngine::Settings::ETextureType p_textureType,
	bool p_flipVertically, bool p_generateMipmap)
{
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

		std::string name = Utils::String::ExtractFileNameFromPath(p_filePath);

		return new Texture(std::move(p_filePath), std::move(name), textureID, textureWidth, textureHeight, bitsPerPixel, p_firstFilter, p_secondFilter, p_textureType, p_generateMipmap);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << p_filePath << std::endl;

		stbi_image_free(dataBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	return nullptr;
}

bool AmberEngine::Resources::TextureLoader::Destroy(Texture*& p_textureInstance)
{
	if (p_textureInstance)
	{
		glDeleteTextures(1, &p_textureInstance->id);

		delete p_textureInstance;
		p_textureInstance = nullptr;

		return true;
	}

	return false;
}

bool AmberEngine::Resources::TextureLoader::Delete(Texture* p_textureInstance)
{
	if (p_textureInstance)
	{
		glDeleteTextures(1, &p_textureInstance->id);

		return true;
	}

	return false;
}

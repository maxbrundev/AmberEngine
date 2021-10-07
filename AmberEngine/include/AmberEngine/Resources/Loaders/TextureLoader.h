#pragma once


#include <GL/glew.h>

#include "AmberEngine/Resources/Texture.h"

namespace AmberEngine::Resources
{
	class TextureLoader
	{
	public:
		TextureLoader() = delete;

		static Texture* Create(const std::string& p_filePath, AmberEngine::Settings::ETextureFilteringMode p_firstFilter, AmberEngine::Settings::ETextureFilteringMode p_secondFilter, AmberEngine::Settings::ETextureType p_textureType, bool p_flipVertically, bool p_generateMipmap);
		static bool Destroy(Texture*& p_textureInstance);

	private:

	};
}

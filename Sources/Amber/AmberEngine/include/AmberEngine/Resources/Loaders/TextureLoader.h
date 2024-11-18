#pragma once

#include <GL/glew.h>

#include "AmberEngine/Resources/Texture.h"

namespace AmberEngine::Resources::Loaders
{
	class TextureLoader
	{
	public:
		TextureLoader() = delete;

		static Texture* Create(std::string p_filePath, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, ETextureType p_textureType, bool p_flipVertically, bool p_generateMipmap);
		static Texture* CreateColor(uint32_t p_data, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, bool p_generateMipmap);
		static Texture* CreateFromMemory(uint8_t* p_data, uint32_t p_width, uint32_t p_height, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, bool p_generateMipmap);
		static void Reload(Texture& p_texture, const std::string& p_filePath, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, ETextureType p_textureType, bool p_flipVertically, bool p_generateMipmap);
		static bool Destroy(Texture*& p_textureInstance);
		static bool Delete(Texture* p_textureInstance);

	private:
		static std::string FILE_TRACE;
	};
}

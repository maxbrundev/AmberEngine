#pragma once

#include <GL/glew.h>

#include "AmberEngine/Resources/Texture.h"

namespace AmberEngine::Resources::Loaders
{
	class TextureLoader
	{
	public:
		TextureLoader() = delete;

		static Texture* Create(std::string p_filePath, Settings::ETextureFilteringMode p_firstFilter, Settings::ETextureFilteringMode p_secondFilter, Settings::ETextureType p_textureType, bool p_flipVertically, bool p_generateMipmap);
		static Texture* CreateColor(uint32_t p_data, Settings::ETextureFilteringMode p_firstFilter, Settings::ETextureFilteringMode p_secondFilter, bool p_generateMipmap);
		static void Reload(const Texture& p_texture, const std::string& p_filePath, Settings::ETextureFilteringMode p_firstFilter, Settings::ETextureFilteringMode p_secondFilter, Settings::ETextureType p_textureType, bool p_flipVertically, bool p_generateMipmap);
		static bool Destroy(Texture*& p_textureInstance);
		static bool Delete(Texture* p_textureInstance);

	private:
		static std::string __FILE_TRACE;
	};
}

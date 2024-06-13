#pragma once

#include "AmberEngine/Resources/ETextureFilteringMode.h"
#include "AmberEngine/Resources/ETextureType.h"

namespace  AmberEngine::Resources::Loaders
{
	class TextureLoader;
}

namespace AmberEngine::Resources
{
	class Texture
	{
		friend class Loaders::TextureLoader;

	public:
		Texture(std::string p_filePath, std::string p_name, uint32_t p_id, uint32_t p_width, uint32_t p_height, uint32_t p_bitsPerPixel, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, ETextureType p_textureType, bool p_generateMipmap);
		~Texture();
		void Bind(uint32_t p_slot = 0) const;
		void Unbind() const;

	public:
		const uint32_t ID;
		const uint32_t Width;
		const uint32_t Height;
		const uint32_t BitsPerPixel;

		const std::string Path;
		const std::string Name;

		const ETextureFilteringMode FirstFilter;
		const ETextureFilteringMode SecondFilter;

		const ETextureType Type;

		const bool HasMipmaps;
	};
}
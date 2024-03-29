#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Resources/ETextureFilteringMode.h"
#include "AmberEngine/Resources/ETextureType.h"

namespace AmberEngine::Resources
{
	class API_AMBERENGINE Texture
	{
	public:
		Texture(std::string p_filePath, std::string p_name, uint32_t p_id, uint32_t p_width, uint32_t p_height, uint32_t p_bitsPerPixel, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, ETextureType p_textureType, bool p_generateMipmap);
		~Texture();
		
		void Bind(uint32_t p_slot = 0) const;
		void Unbind() const;
		
	public:
		const std::string path;
		const std::string name;
		const uint32_t id;
		const uint32_t width;
		const uint32_t height;
		const uint32_t bitsPerPixel;
		const ETextureFilteringMode firstFilter;
		const ETextureFilteringMode secondFilter;
		const ETextureType type;
		const bool isMimapped;
	};
}
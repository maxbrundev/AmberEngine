#pragma once



#include "AmberEngine/API/Export.h"

#include "ETextureFilteringMode.h"
#include "ETextureType.h"

namespace AmberEngine::Resources
{
	class API_AMBERENGINE Texture
	{
	public:
		Texture(const std::string& p_filePath, const std::string& p_name, uint32_t p_id, uint32_t p_width, uint32_t p_height, uint32_t p_bitsPerPixel, Settings::ETextureFilteringMode p_firstFilter, Settings::ETextureFilteringMode p_secondFilter, Settings::ETextureType p_textureType, bool p_generateMipmap);
		~Texture() = default;
		
		void Bind(uint32_t p_slot = 0) const;
		void Unbind() const;
		
	public:
		const std::string path;
		const std::string name;
		const uint32_t id;
		const uint32_t width;
		const uint32_t height;
		const uint32_t bitsPerPixel;
		const Settings::ETextureFilteringMode firstFilter;
		const Settings::ETextureFilteringMode secondFilter;
		const Settings::ETextureType type;
		const bool isMimapped;
	};
}
#include "Amberpch.h"

#include <GL/glew.h>

#include "AmberEngine/Resources/Texture.h"

AmberEngine::Resources::Texture::Texture(const std::string& p_filePath, const std::string& p_name, uint32_t p_id, uint32_t p_width, uint32_t p_height, uint32_t p_bitsPerPixel, Settings::ETextureFilteringMode p_firstFilter, Settings::ETextureFilteringMode p_secondFilter, Settings::ETextureType p_textureType, bool p_generateMipmap) :
	path(p_filePath),
	name(p_name),
	id(p_id),
	width(p_width),
	height(p_width),
	bitsPerPixel(p_bitsPerPixel),
	firstFilter(p_firstFilter),
	secondFilter(p_secondFilter),
	type(p_textureType),
	isMimapped(p_generateMipmap)
{
}

void AmberEngine::Resources::Texture::Bind(uint32_t p_slot) const
{
	glActiveTexture(GL_TEXTURE0 + p_slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void AmberEngine::Resources::Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

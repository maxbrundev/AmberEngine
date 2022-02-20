#include "Amberpch.h"

#include <GL/glew.h>

#include "AmberEngine/Resources/Texture.h"

AmberEngine::Resources::Texture::Texture(std::string p_filePath, std::string p_name, uint32_t p_id, uint32_t p_width, uint32_t p_height, uint32_t p_bitsPerPixel, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, ETextureType p_textureType, bool p_generateMipmap) :
	path(std::move(p_filePath)),
	name(std::move(p_name)),
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

AmberEngine::Resources::Texture::~Texture()
{
	glDeleteTextures(1, &id);
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

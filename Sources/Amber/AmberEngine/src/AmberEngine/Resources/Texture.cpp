#include "Amberpch.h"

#include <GL/glew.h>

#include "AmberEngine/Resources/Texture.h"

AmberEngine::Resources::Texture::Texture(std::string p_filePath, std::string p_name, uint32_t p_id, uint32_t p_width, uint32_t p_height, uint32_t p_bitsPerPixel, ETextureFilteringMode p_firstFilter, ETextureFilteringMode p_secondFilter, ETextureType p_textureType, bool p_generateMipmap) :
	Path(std::move(p_filePath)),
	Name(std::move(p_name)),
	ID(p_id),
	Width(p_width),
	Height(p_width),
	BitsPerPixel(p_bitsPerPixel),
	FirstFilter(p_firstFilter),
	SecondFilter(p_secondFilter),
	Type(p_textureType),
	HasMipmaps(p_generateMipmap)
{
}

AmberEngine::Resources::Texture::~Texture()
{
	glDeleteTextures(1, &ID);
}

void AmberEngine::Resources::Texture::Bind(uint32_t p_slot) const
{
	glActiveTexture(GL_TEXTURE0 + p_slot);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void AmberEngine::Resources::Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

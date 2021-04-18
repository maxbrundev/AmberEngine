#pragma once

#include <GL/glew.h>

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Resources
{
	class API_AMBERENGINE Texture
	{
	public:
		Texture(const std::string_view p_path, bool p_flipVertically = false);
		~Texture();

		void Bind(uint32_t p_slot = 0) const;
		void Unbind() const;
		
	public:
		uint32_t m_id;
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_bitsPerPixel;
		std::string_view m_type;
		std::string_view m_path;

	};
}
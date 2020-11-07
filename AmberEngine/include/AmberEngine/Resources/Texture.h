#pragma once

#include <GL/glew.h>

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Resources
{
	class API_AMBERENGINE Texture
	{
	private:
		std::string m_type;
		std::string m_path;

		GLuint m_textureId;
		
		int m_width;
		int m_height;
		int m_bitsPerPixel;

	public:
		Texture() = default;
		Texture(const std::string& p_filePath);
		~Texture();

		void Bind(const unsigned int& p_slot = 0) const;
		void Unbind() const;

		unsigned int GetTextureId();
		int GetTextureWidth();
		int GetTextureHeight();

	};
}
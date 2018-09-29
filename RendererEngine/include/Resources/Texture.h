#pragma once

namespace RenderEngine
{
	namespace Resources
	{
		class Texture
		{
		private:
			unsigned int m_RendererID;

			int m_Width;
			int m_Height; 
			int m_BPP;

			unsigned char* m_LocalBuffer;

			std::string m_FilePath;

		public:
			Texture(const std::string& path);
			~Texture();

			void Bind(unsigned int slot = 0) const;
			void Unbind() const;

			inline int GetWidth() const { return m_Width; }
			inline int GetHeight() const { return m_Height; }
		};
	}
}
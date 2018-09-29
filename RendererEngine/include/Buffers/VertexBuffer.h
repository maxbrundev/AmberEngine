#pragma once

namespace RenderEngine
{
	namespace Buffers
	{
		class VertexBuffer
		{
		private:
			unsigned int m_rendererID;
		public:
			VertexBuffer(const void* p_data, unsigned int p_size);
			~VertexBuffer();

			void Bind() const;
			void Unbind() const;
		};
	}
}
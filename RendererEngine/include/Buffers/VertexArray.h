#pragma once

#include "VertexBuffer.h"
#include "Buffers/VertexBufferLayout.h"

namespace RenderEngine
{
	namespace Buffers
	{
		class VertexArray
		{
		private:
			unsigned int m_rendererID;
		public:
			VertexArray();
			~VertexArray();

			void AddBuffer(const VertexBuffer& p_vbo, const VertexBufferLayout& p_layout);

			void Bind() const;
			void Unbind() const;
		};
	}
}
#pragma once

#include "pch.h"

namespace RenderEngine
{
	namespace Buffers
	{
		struct VertexBufferElement
		{
			unsigned int type;
			unsigned int count;
			unsigned char normalized;
		};

		class VertexBufferLayout
		{
		private:
			std::vector<VertexBufferElement> m_elements;
			unsigned int m_stride;
		public:
			VertexBufferLayout();
			~VertexBufferLayout() = default;

			void PushFloat(unsigned int count);

			const std::vector<VertexBufferElement>& GetElements() const;
			unsigned int GetStride() const;
		};
	}
}
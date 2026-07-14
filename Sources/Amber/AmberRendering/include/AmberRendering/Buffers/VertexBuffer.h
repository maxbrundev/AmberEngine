#pragma once


#include "AmberRendering/Context/Driver.h"

namespace AmberRendering::Buffers
{
	class VertexBuffer
	{
	public:
		VertexBuffer(const void* p_data, size_t p_elements);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

		GLuint GetID();

	private:
		GLuint m_bufferID;
	};
}
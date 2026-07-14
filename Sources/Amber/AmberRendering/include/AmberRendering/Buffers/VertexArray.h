#pragma once


#include "AmberRendering/Context/Driver.h"
#include "AmberRendering/Buffers/VertexBuffer.h"

namespace AmberRendering::Buffers
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void BindAttribPointer(const unsigned int p_size, const unsigned int p_type, const unsigned int p_normalized, const unsigned int p_stride, void* p_pointer);

		void Bind() const;
		void Unbind() const;

		GLuint GetID() const;

	private:
		GLuint m_bufferID;
		uint32_t index;
	};
}
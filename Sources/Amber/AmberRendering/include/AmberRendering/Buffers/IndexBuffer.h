#pragma once


#include "AmberRendering/Context/Driver.h"

namespace AmberRendering::Buffers
{
	class IndexBuffer
	{
	public:
		IndexBuffer(const unsigned int* p_data, size_t p_elements);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		GLuint GetID() const;

	private:
		GLuint m_bufferID;
	};
}
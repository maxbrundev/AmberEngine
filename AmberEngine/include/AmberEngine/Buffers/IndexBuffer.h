#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Context/Driver.h"

namespace AmberEngine::Buffers
{
	class API_AMBERENGINE IndexBuffer
	{
	private:
		GLuint m_bufferID;

	public:
		IndexBuffer(const unsigned int* p_data, size_t p_elements);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		GLuint GetID();
	};
}
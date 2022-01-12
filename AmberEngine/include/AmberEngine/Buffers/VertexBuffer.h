#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Context/Driver.h"

namespace AmberEngine::Buffers
{
	class API_AMBERENGINE VertexBuffer
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
#pragma once

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Buffers
{
	class API_AMBERENGINE VertexBuffer
	{
	private:
		GLuint m_bufferID;
		
	public:
		VertexBuffer(const void* p_data, size_t p_elements);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

		GLuint GetID();
	};
}
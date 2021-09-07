#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Context/Driver.h"


namespace AmberEngine::Buffers
{
	class API_AMBERENGINE VertexArray
	{
	private:
		GLuint m_bufferID;
		unsigned int index;
		
	public:
		VertexArray();
		~VertexArray();

		void BindAttribPointer(const unsigned int p_size, const unsigned int p_type, const unsigned int p_normalized, const unsigned int p_stride, void* p_pointer);

		void Bind() const;
		void Unbind() const;


		GLuint GetID();
	};
}
#pragma once

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Buffers { class VertexBuffer; }
namespace AmberEngine::Buffers { class VertexBufferLayout; }

namespace AmberEngine::Buffers
{
	class API_AMBERENGINE VertexArray
	{
	private:
		GLuint m_bufferID;
		
	public:
		VertexArray();
		~VertexArray();

		void AddBuffer(const VertexBuffer& p_vbo, const VertexBufferLayout& p_layout);

		void Bind() const;
		void Unbind() const;


		GLuint GetID();
	};
}
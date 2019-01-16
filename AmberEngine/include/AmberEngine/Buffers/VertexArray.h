#pragma once

#include "AmberEngine/Buffers/VertexBuffer.h"
#include "AmberEngine/Buffers/VertexBufferLayout.h"

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Buffers
{
	class API_AMBERENGINE VertexArray
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
#pragma once

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Buffers
{
	class API_AMBERENGINE IndexBuffer
	{
	private:
		unsigned int m_rendererID;
		unsigned int m_count;
	public:
		IndexBuffer(const unsigned int* p_data, unsigned int p_count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		inline unsigned int GetCount() const { return m_count; }
	};
}
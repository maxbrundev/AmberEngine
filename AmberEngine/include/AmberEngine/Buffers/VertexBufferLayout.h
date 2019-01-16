#pragma once

#include <vector>

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Buffers
{
	struct API_AMBERENGINE VertexBufferElement
	{
		unsigned int type;
		unsigned int count;
		unsigned char normalized;
	};

	class API_AMBERENGINE VertexBufferLayout
	{
	private:
		std::vector<VertexBufferElement> m_elements;
		unsigned int m_stride;
	public:
		VertexBufferLayout();
		~VertexBufferLayout() = default;

		void PushFloat(unsigned int count);

		const std::vector<VertexBufferElement>& GetElements() const;
		unsigned int GetStride() const;
	};
}
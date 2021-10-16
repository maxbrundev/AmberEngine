#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Context/Driver.h"

namespace AmberEngine::Buffers
{
	class API_AMBERENGINE FrameBuffer
	{
	public:
		FrameBuffer(uint16_t p_width, uint16_t p_height);
		~FrameBuffer();

		void Bind();
		void Unbind();

		void Resize(uint16_t p_width, uint16_t p_height);

		uint32_t GetID();
		uint32_t GetTextureID();
		uint32_t GetRenderBufferID();
	private:
		uint32_t m_bufferID = 0;
		uint32_t m_renderTexture = 0;
		uint32_t m_depthStencilBuffer = 0;
	};
}
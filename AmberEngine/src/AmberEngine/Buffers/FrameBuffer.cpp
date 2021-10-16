#include "Amberpch.h"

#include "AmberEngine/Buffers/FrameBuffer.h"

AmberEngine::Buffers::FrameBuffer::FrameBuffer(uint16_t p_width, uint16_t p_height)
{
	glGenFramebuffers(1, &m_bufferID);
	glGenTextures(1, &m_renderTexture);
	glGenRenderbuffers(1, &m_depthStencilBuffer);

	/* Setup texture */
	glBindTexture(GL_TEXTURE_2D, m_renderTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	/* Setup framebuffer */
	Bind();
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_renderTexture, 0);
	Unbind();

	Resize(p_width, p_height);
}

AmberEngine::Buffers::FrameBuffer::~FrameBuffer()
{
	/* Destroy OpenGL objects */
	glDeleteBuffers(1, &m_bufferID);
	glDeleteTextures(1, &m_renderTexture);
	glDeleteRenderbuffers(1, &m_depthStencilBuffer);
}

void AmberEngine::Buffers::FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_bufferID);
}

void AmberEngine::Buffers::FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void AmberEngine::Buffers::FrameBuffer::Resize(uint16_t p_width, uint16_t p_height)
{
	/* Resize texture */
	glBindTexture(GL_TEXTURE_2D, m_renderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, p_width, p_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	/* Setup depth-stencil buffer (24 + 8 bits) */
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, p_width, p_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	/* Attach depth and stencil buffer to the framebuffer */
	Bind();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilBuffer);
	Unbind();
}

uint32_t AmberEngine::Buffers::FrameBuffer::GetID()
{
	return m_bufferID;
}

uint32_t AmberEngine::Buffers::FrameBuffer::GetTextureID()
{
	return m_renderTexture;
}

uint32_t AmberEngine::Buffers::FrameBuffer::GetRenderBufferID()
{
	return m_depthStencilBuffer;
}

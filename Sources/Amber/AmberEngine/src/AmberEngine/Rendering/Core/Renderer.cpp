#include "Amberpch.h"

#include "AmberEngine/Rendering/Core/Renderer.h"

#include "AmberEngine/Rendering/Entities/Camera.h"

AmberEngine::Rendering::Core::Renderer::Renderer(Context::Driver& p_driver) :
m_driver(p_driver),
m_state(0)
{
}

void AmberEngine::Rendering::Core::Renderer::SetClearColor(float p_red, float p_green, float p_blue, float p_alpha) const
{
	glClearColor(p_red, p_green, p_blue, p_alpha);
}

void AmberEngine::Rendering::Core::Renderer::Clear(bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer) const
{
	//TODO: Clean
	m_driver.SetStencilMask(0xFF);

	glClear
	(
		(p_colorBuffer   ? GL_COLOR_BUFFER_BIT   : 0) |
		(p_depthBuffer   ? GL_DEPTH_BUFFER_BIT   : 0) |
		(p_stencilBuffer ? GL_STENCIL_BUFFER_BIT : 0)
	);
}

void AmberEngine::Rendering::Core::Renderer::Clear(Rendering::Entities::Camera& p_camera, bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer) const
{
	/* Backup the previous OpenGL clear color */
	GLfloat previousClearColor[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, previousClearColor);

	/* Clear the screen using the camera clear color */
	const glm::vec3& cameraClearColor = p_camera.GetClearColor();
	SetClearColor(cameraClearColor.x, cameraClearColor.y, cameraClearColor.z, 1.0f);
	Clear(p_colorBuffer, p_depthBuffer, p_stencilBuffer);

	/* Reset the OpenGL clear color to the previous clear color (Backuped one) */
	SetClearColor(previousClearColor[0], previousClearColor[1], previousClearColor[2], previousClearColor[3]);
}

uint8_t AmberEngine::Rendering::Core::Renderer::FetchGLState() const
{
	uint8_t result = 0;

	GLboolean cMask[4];
	glGetBooleanv(GL_COLOR_WRITEMASK, cMask);

	if (m_driver.GetBool(GL_DEPTH_WRITEMASK))                     result |= 0b0000'0001;
	if (cMask[0])                                                           result |= 0b0000'0010;
	if (m_driver.GetCapability(Settings::ERenderingCapability::BLEND))      result |= 0b0000'0100;
	if (m_driver.GetCapability(Settings::ERenderingCapability::CULL_FACE))  result |= 0b0000'1000;
	if (m_driver.GetCapability(Settings::ERenderingCapability::DEPTH_TEST)) result |= 0b0001'0000;

	switch (static_cast<Settings::ECullFace>(m_driver.GetInt(GL_CULL_FACE)))
	{
	case Settings::ECullFace::BACK:           result |= 0b0010'0000; break;
	case Settings::ECullFace::FRONT:          result |= 0b0100'0000; break;
	case Settings::ECullFace::FRONT_AND_BACK: result |= 0b0110'0000; break;
	}

	return result;
}

void AmberEngine::Rendering::Core::Renderer::ApplyStateMask(uint8_t p_mask)
{
	if (p_mask != m_state)
	{
		using namespace Rendering::Settings;

		if ((p_mask & 0x01) != (m_state & 0x01)) m_driver.SetDepthWriting(p_mask & 0x01);
		if ((p_mask & 0x02) != (m_state & 0x02)) m_driver.SetColorWriting(p_mask & 0x02);
		if ((p_mask & 0x04) != (m_state & 0x04)) m_driver.SetCapability(ERenderingCapability::BLEND, p_mask & 0x04);
		if ((p_mask & 0x08) != (m_state & 0x08)) m_driver.SetCapability(ERenderingCapability::CULL_FACE, p_mask & 0x8);
		if ((p_mask & 0x10) != (m_state & 0x10)) m_driver.SetCapability(ERenderingCapability::DEPTH_TEST, p_mask & 0x10);

		if ((p_mask & 0x08) && ((p_mask & 0x20) != (m_state & 0x20) || (p_mask & 0x40) != (m_state & 0x40)))
		{
			const int backBit  = p_mask & 0x20;
			const int frontBit = p_mask & 0x40;
			m_driver.SetCullFace(backBit && frontBit ? ECullFace::FRONT_AND_BACK : (backBit ? ECullFace::BACK : ECullFace::FRONT));
		}

		m_state = p_mask;
	}
}

void AmberEngine::Rendering::Core::Renderer::SetState(uint8_t p_state)
{
	m_state = p_state;
}

void AmberEngine::Rendering::Core::Renderer::ClearFrameInfo()
{
	m_frameInfo.batchCount    = 0;
	m_frameInfo.instanceCount = 0;
	m_frameInfo.polyCount     = 0;
	m_frameInfo.vertexCount   = 0;
}

const AmberEngine::Rendering::Core::Renderer::FrameInfo& AmberEngine::Rendering::Core::Renderer::GetFrameInfo() const
{
	return m_frameInfo;
}

void AmberEngine::Rendering::Core::Renderer::Draw(Resources::IMesh& p_mesh, Settings::EPrimitiveMode p_primitiveMode, uint32_t p_instances)
{
	if (p_instances > 0)
	{
		++m_frameInfo.batchCount;
		m_frameInfo.instanceCount += p_instances;
		m_frameInfo.polyCount     += p_mesh.GetIndexCount() / 3 * p_instances;
		m_frameInfo.vertexCount   += p_mesh.GetVertexCount() * p_instances;

		p_mesh.Bind();

		if (p_mesh.GetIndexCount() > 0)
		{
			if (p_instances == 1)
				glDrawElements(static_cast<GLenum>(p_primitiveMode), p_mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr);
			else
				glDrawElementsInstanced(static_cast<GLenum>(p_primitiveMode), p_mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr, p_instances);
		}
		else
		{
			if (p_instances == 1)
				glDrawArrays(static_cast<GLenum>(p_primitiveMode), 0, p_mesh.GetVertexCount());
			else
				glDrawArraysInstanced(static_cast<GLenum>(p_primitiveMode), 0, p_mesh.GetVertexCount(), p_instances);
		}

		p_mesh.Unbind();
	}
}

AmberEngine::Context::Driver& AmberEngine::Rendering::Core::Renderer::GetDriver() const
{
	return m_driver;
}

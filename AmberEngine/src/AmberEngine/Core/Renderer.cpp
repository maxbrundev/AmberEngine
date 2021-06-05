#include "Amberpch.h"

#include "AmberEngine/Core/Renderer.h"

#include "AmberEngine/Resources/AssimpMesh.h"

AmberEngine::Core::Renderer::Renderer(Context::Driver& p_driver) : m_driver(p_driver), isWireFrame(false)
{
}

void AmberEngine::Core::Renderer::DrawModelWithShader(Resources::AssimpModel& p_model, Resources::Shader& p_shader)
{
	for (auto mesh : p_model.GetMeshes())
	{
		DrawMesh(*mesh, p_shader);
	}
}

void AmberEngine::Core::Renderer::DrawMesh(Resources::AssimpMesh& p_mesh, Resources::Shader& p_shader)
{
	p_mesh.BindMaterialTextures(p_shader);
	
	p_mesh.Bind();
	
	if(p_mesh.m_indicesCount > 0)
	{
		glDrawElements(GL_TRIANGLES, p_mesh.m_indicesCount, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, p_mesh.m_vertexCount);
	}

	p_mesh.Unbind();
}

void AmberEngine::Core::Renderer::SetClearColor(float p_red, float p_green, float p_blue, float p_alpha)
{
	glClearColor(p_red, p_green, p_blue, p_alpha);
}

void AmberEngine::Core::Renderer::Clear(bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer)
{
	glClear
	(
		(p_colorBuffer ? GL_COLOR_BUFFER_BIT : 0) |
		(p_depthBuffer ? GL_DEPTH_BUFFER_BIT : 0) |
		(p_stencilBuffer ? GL_STENCIL_BUFFER_BIT : 0)
	);
}

void AmberEngine::Core::Renderer::Clear(AmberEngine::LowRenderer::Camera& p_camera, bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer)
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

void AmberEngine::Core::Renderer::UpdateRenderMode()
{
	if (isWireFrame)
		PolygonModeLine();
	else
		PolygonModeFill();
}

void AmberEngine::Core::Renderer::PolygonModeLine()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void AmberEngine::Core::Renderer::PolygonModeFill()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void AmberEngine::Core::Renderer::ToggleWireFrame()
{
	isWireFrame = !isWireFrame;
}
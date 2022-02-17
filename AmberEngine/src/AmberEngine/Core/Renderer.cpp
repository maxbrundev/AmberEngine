#include "Amberpch.h"

#include <utility>

#include "AmberEngine/Core/Renderer.h"

#include "AmberEngine/Core/SceneSystem/Scene.h"
#include "AmberEngine/Resources/Mesh.h"
#include "AmberEngine/Resources/Loaders/TextureLoader.h"

AmberEngine::Core::Renderer::Renderer(Context::Driver& p_driver) :
	m_driver(p_driver),
	m_emptyTexture(Resources::Loaders::TextureLoader::CreateColor
	(
	(255 << 24) | (255 << 16) | (255 << 8) | 255,
	Settings::ETextureFilteringMode::NEAREST,
	Settings::ETextureFilteringMode::NEAREST,
	false
	))
{
}

AmberEngine::Core::Renderer::~Renderer()
{
	Resources::Loaders::TextureLoader::Destroy(m_emptyTexture);
}

void AmberEngine::Core::Renderer::Draw(Resources::Model& p_model, glm::mat4 const* p_modelMatrix) const
{
	m_modelMatrixSender(*p_modelMatrix);
	
	for (const auto mesh : p_model.GetMeshes())
	{
		const auto material = p_model.GetMaterials()[mesh->GetMaterialIndex()];
		DrawMesh(*mesh, material);
	}
}

void AmberEngine::Core::Renderer::DrawMesh(const Resources::Mesh& p_mesh, Resources::Material* p_material) const
{
	p_material->Bind(m_emptyTexture);

	p_mesh.Bind();

	if(p_mesh.GetIndexCount() > 0)
	{
		glDrawElements(GL_TRIANGLES, p_mesh.GetIndexCount(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, p_mesh.GetVertexCount());
	}

	p_mesh.Unbind();

	p_material->Unbind();
}

void AmberEngine::Core::Renderer::SetClearColor(float p_red, float p_green, float p_blue, float p_alpha) const
{
	glClearColor(p_red, p_green, p_blue, p_alpha);
}

void AmberEngine::Core::Renderer::Clear(bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer) const
{
	glClear
	(
		(p_colorBuffer ? GL_COLOR_BUFFER_BIT : 0) |
		(p_depthBuffer ? GL_DEPTH_BUFFER_BIT : 0) |
		(p_stencilBuffer ? GL_STENCIL_BUFFER_BIT : 0)
	);
}

void AmberEngine::Core::Renderer::Clear(AmberEngine::LowRenderer::Camera& p_camera, bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer) const
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

std::vector<glm::mat4> AmberEngine::Core::Renderer::FindLightMatrices(SceneSystem::Scene& p_scene)
{
	std::vector<glm::mat4> result;

	for (const auto light : p_scene.GetLights())
	{
		result.push_back(light->GetLightData().GenerateMatrix());
	}

	return result;
}

void AmberEngine::Core::Renderer::RegisterModelMatrixSender(std::function<void(glm::mat4)> p_modelMatrixSender)
{
	m_modelMatrixSender = std::move(p_modelMatrixSender);
}

void AmberEngine::Core::Renderer::PolygonModeLine() const
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void AmberEngine::Core::Renderer::PolygonModeFill() const
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
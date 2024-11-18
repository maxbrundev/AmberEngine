#include "Amberpch.h"

#include "AmberEngine/Core/ShapeDrawer.h"

#include "AmberEngine/Geometry/Vertex.h"
#include "AmberEngine/Resources/Mesh.h"
#include "AmberEngine/Resources/Loaders/ShaderLoader.h"

AmberEngine::Core::ShapeDrawer::ShapeDrawer(Rendering::Core::Renderer& p_renderer) : m_renderer(p_renderer)
{
	std::vector<Geometry::Vertex> vertices;
	vertices.push_back
	({
		0, 0, 0,
		0, 0,
		0, 0, 0,
		0, 0, 0,
		0, 0, 0
		});
	vertices.push_back
	({
		0, 0, 0,
		0, 0,
		0, 0, 0,
		0, 0, 0,
		0, 0, 0
		});

	m_lineMesh = new Resources::Mesh(vertices, { 0, 1 }, 0);

	std::string vertexShader = R"(
#version 430 core

uniform vec3 start;
uniform vec3 end;
uniform mat4 viewProjection;

void main()
{
	vec3 position = gl_VertexID == 0 ? start : end;
    gl_Position = viewProjection * vec4(position, 1.0);
}

)";

	std::string fragmentShader = R"(
#version 430 core

uniform vec3 color;

out vec4 FRAGMENT_COLOR;

void main()
{
	FRAGMENT_COLOR = vec4(color, 1.0);
}
)";

	m_lineShader = Resources::Loaders::ShaderLoader::CreateFromSource(vertexShader, fragmentShader);

	vertexShader = R"(
#version 430 core

uniform vec3 start;
uniform vec3 end;
uniform mat4 viewProjection;

out vec3 fragPos;

void main()
{
	vec3 position = gl_VertexID == 0 ? start : end;
	fragPos = position;
    gl_Position = viewProjection * vec4(position, 1.0);
}

)";

	fragmentShader = R"(
#version 430 core

uniform vec3 color;
uniform vec3 viewPos;
uniform float linear;
uniform float quadratic;
uniform float fadeThreshold;

out vec4 FRAGMENT_COLOR;

in vec3 fragPos;

float AlphaFromAttenuation()
{
	vec3 fakeViewPos = viewPos;
	fakeViewPos.y = 0;

    const float distanceToLight = max(max(length(viewPos - fragPos) - fadeThreshold, 0) - viewPos.y, 0);
    const float attenuation = (linear * distanceToLight + quadratic * (distanceToLight * distanceToLight));
    return 1.0 / attenuation;
}

void main()
{
	FRAGMENT_COLOR = vec4(color, AlphaFromAttenuation());
}
)";

	m_gridShader = Resources::Loaders::ShaderLoader::CreateFromSource(vertexShader, fragmentShader);
}

AmberEngine::Core::ShapeDrawer::~ShapeDrawer()
{
	Resources::Loaders::ShaderLoader::Destroy(m_lineShader);
}

void AmberEngine::Core::ShapeDrawer::SetViewProjection(const glm::mat4& p_viewProjection)
{
	m_lineShader->Bind();
	m_lineShader->SetUniformMat4("viewProjection", p_viewProjection);
	m_lineShader->Unbind();

	m_gridShader->Bind();
	m_gridShader->SetUniformMat4("viewProjection", p_viewProjection);
	m_gridShader->Unbind();
}

void AmberEngine::Core::ShapeDrawer::DrawLine(const glm::vec3& p_start, const glm::vec3& p_end, const glm::vec3& p_color, float p_lineWidth)
{
	m_lineShader->Bind();

	m_lineShader->SetUniformVec3("start", p_start);
	m_lineShader->SetUniformVec3("end", p_end);
	m_lineShader->SetUniformVec3("color", p_color);

	m_renderer.GetDriver().SetRasterizationMode(Rendering::Settings::ERasterizationMode::LINE);
	m_renderer.GetDriver().SetRasterizationLinesWidth(p_lineWidth);
	m_renderer.Draw(*m_lineMesh, Rendering::Settings::EPrimitiveMode::LINES);
	m_renderer.GetDriver().SetRasterizationLinesWidth(1.0f);
	m_renderer.GetDriver().SetRasterizationMode(Rendering::Settings::ERasterizationMode::FILL);

	m_lineShader->Unbind();
}

void AmberEngine::Core::ShapeDrawer::DrawGrid(const glm::vec3& p_viewPos, const glm::vec3& p_color, int32_t p_gridSize, float p_linear, float p_quadratic, float p_fadeThreshold, float p_lineWidth)
{
	m_gridShader->Bind();
	m_gridShader->SetUniformVec3("color", p_color);
	m_gridShader->SetUniformVec3("viewPos", p_viewPos);
	m_gridShader->SetUniformFloat("linear", p_linear);
	m_gridShader->SetUniformFloat("quadratic", p_quadratic);
	m_gridShader->SetUniformFloat("fadeThreshold", p_fadeThreshold);

	m_renderer.GetDriver().SetRasterizationMode(Rendering::Settings::ERasterizationMode::LINE);
	m_renderer.GetDriver().SetRasterizationLinesWidth(p_lineWidth);
	m_renderer.GetDriver().SetCapability(Rendering::Settings::ERenderingCapability::BLEND, true);

	for (int32_t i = -p_gridSize + 1; i < p_gridSize; ++i)
	{
		m_gridShader->SetUniformVec3("start", { -(float)p_gridSize + std::floor(p_viewPos.x), 0.f, (float)i + std::floor(p_viewPos.z) });
		m_gridShader->SetUniformVec3("end", { (float)p_gridSize + std::floor(p_viewPos.x), 0.f, (float)i + std::floor(p_viewPos.z) });
		m_renderer.Draw(*m_lineMesh, Rendering::Settings::EPrimitiveMode::LINES);

		m_gridShader->SetUniformVec3("start", { (float)i + std::floor(p_viewPos.x), 0.f, -(float)p_gridSize + std::floor(p_viewPos.z) });
		m_gridShader->SetUniformVec3("end", { (float)i + std::floor(p_viewPos.x), 0.f, (float)p_gridSize + std::floor(p_viewPos.z) });
		m_renderer.Draw(*m_lineMesh, Rendering::Settings::EPrimitiveMode::LINES);
	}

	m_renderer.GetDriver().SetCapability(Rendering::Settings::ERenderingCapability::BLEND, false);
	m_renderer.GetDriver().SetRasterizationLinesWidth(1.0f);
	m_renderer.GetDriver().SetRasterizationMode(Rendering::Settings::ERasterizationMode::FILL);
	m_gridShader->Unbind();
}

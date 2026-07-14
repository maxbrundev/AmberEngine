#include "Amberpch.h"

#include "AmberRendering/Resources/Model.h"

#include "AmberTools/Eventing/Event.h"

AmberRendering::Resources::Model::Model(const std::string& p_filePath) : Path(p_filePath)
{
}

AmberRendering::Resources::Model::~Model()
{
	for (auto& mesh : m_meshes)
	{
		delete mesh;
		mesh = nullptr;
	}
}

std::vector<std::string>& AmberRendering::Resources::Model::GetMaterialNames()
{
	return m_materialNames;
}

std::vector<AmberRendering::Resources::Mesh*>& AmberRendering::Resources::Model::GetMeshes()
{
	return m_meshes;
}

const AmberRendering::Geometry::BoundingSphere& AmberRendering::Resources::Model::GetBoundingSphere() const
{
	return m_boundingSphere;
}

void AmberRendering::Resources::Model::ComputeBoundingSphere()
{
	if (m_meshes.size() == 1)
	{
		m_boundingSphere = m_meshes[0]->GetBoundingSphere();
	}
	else
	{
		m_boundingSphere.Position = glm::vec3(0.0f);
		m_boundingSphere.Radius = 0.0f;

		if (!m_meshes.empty())
		{
			float minX = std::numeric_limits<float>::max();
			float minY = std::numeric_limits<float>::max();
			float minZ = std::numeric_limits<float>::max();

			float maxX = std::numeric_limits<float>::lowest();
			float maxY = std::numeric_limits<float>::lowest();
			float maxZ = std::numeric_limits<float>::lowest();

			for (const auto& mesh : m_meshes)
			{
				const auto& boundingSphere = mesh->GetBoundingSphere();
				minX = std::min(minX, boundingSphere.Position.x - boundingSphere.Radius);
				minY = std::min(minY, boundingSphere.Position.y - boundingSphere.Radius);
				minZ = std::min(minZ, boundingSphere.Position.z - boundingSphere.Radius);

				maxX = std::max(maxX, boundingSphere.Position.x + boundingSphere.Radius);
				maxY = std::max(maxY, boundingSphere.Position.y + boundingSphere.Radius);
				maxZ = std::max(maxZ, boundingSphere.Position.z + boundingSphere.Radius);
			}

			m_boundingSphere.Position = glm::vec3{ minX + maxX, minY + maxY, minZ + maxZ } / 2.0f;
			m_boundingSphere.Radius = glm::distance(m_boundingSphere.Position, glm::vec3{ minX, minY, minZ });
		}
	}
}
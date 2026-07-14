#pragma once

#include "AmberRendering/Resources/Mesh.h"
#include "AmberRendering/Resources/ETextureType.h"

namespace AmberRendering::Resources::Loaders
{
	class ModelLoader;
}

namespace AmberRendering::Resources
{
	class Model
	{
		friend class Loaders::ModelLoader;

	public:
		std::vector<std::string>& GetMaterialNames();
		std::vector<Mesh*>& GetMeshes();

		const AmberRendering::Geometry::BoundingSphere& GetBoundingSphere() const;

	private:
		Model(const std::string& p_filePath);
		~Model();

		void ComputeBoundingSphere();

	public:
		const std::string Path;
		std::map<int, std::vector<std::tuple<ETextureType, std::string>>> LoadedTextureData;

	private:
		std::vector<std::string> m_materialNames;
		std::vector<Mesh*> m_meshes;

		AmberRendering::Geometry::BoundingSphere m_boundingSphere;
	};
}
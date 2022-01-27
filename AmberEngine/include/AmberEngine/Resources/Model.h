#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Resources/Mesh.h"

namespace AmberEngine::Resources
{
	namespace Loaders { class ModelLoader; }
	
	class API_AMBERENGINE Model
	{
		friend class Loaders::ModelLoader;

	public:
		Model(const std::string& p_filePath);
		~Model();

		void SetShader(Shader* p_shader);

		std::vector<Mesh*>& GetMeshes();

	private:
		const std::string path;
		std::vector<Mesh*> m_meshes;
	};
}
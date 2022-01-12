#pragma once

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Resources
{
	class Mesh;
	class Shader;
	
	class API_AMBERENGINE Model
	{
	public:
		Model(const std::string& p_filePath);
		~Model();

		void Bind() const;
		void Unbind() const;
		void BindTextureCallback();

		void SetTextureUniform(const std::string_view p_uniformName, int p_id) const;

		void SetShader(Shader& p_shader);
		Shader* GetShader();

		std::vector<Mesh*>& GetMeshes();
		std::vector<std::string>& GetMaterialNames();

	private:
		const std::string path;

		std::vector<Mesh*> m_meshes;
		std::vector<std::string> m_materialNames;
		Shader* m_shader;
	};
}
#pragma once


#include "AmberEngine/API/Export.h"

#include "AmberEngine/Resources/Mesh.h"
#include "AmberEngine/Resources/Texture.h"
#include "AmberEngine/Resources/Shader.h"

namespace AmberEngine::Resources
{
	namespace Loaders { class ModelLoader; }
	
	class API_AMBERENGINE Model
	{
		friend class Loaders::ModelLoader;

	public:
		Model(const std::string& p_filePath);
		~Model();

		void Bind() const;
		void Unbind() const;
		void BindTextureCallback();

		void SetTextureUniform(const std::string_view p_uniformName, uint32_t p_id) const;

		void SetShader(Shader& p_shader);

		void SetTexture(Texture& p_texture) const;

		Shader* GetShader() const;
		std::vector<std::string>& GetMaterialNames();
		std::vector<Mesh*>& GetMeshes();

	private:
		const std::string path;
		Shader* m_shader;
		std::vector<std::string> m_materialNames;
		std::vector<Mesh*> m_meshes;
	};
}
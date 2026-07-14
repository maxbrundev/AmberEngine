#pragma once

#include "AmberRendering/Resources/Model.h"
#include "AmberRendering/Resources/Texture.h"
#include "AmberRendering/Resources/Shader.h"

namespace AmberEditor::Core
{
	class EditorResources
	{
	public:
		EditorResources(const std::string& p_editorAssetsPath);
		~EditorResources();

		AmberRendering::Resources::Model* GetModel(const std::string& p_id);
		AmberRendering::Resources::Texture* GetTexture(const std::string& p_id);
		AmberRendering::Resources::Shader* GetShader(const std::string& p_id);

		AmberRendering::Resources::Texture* GetFileIcon(const std::string& p_filename);

	private:
		std::unordered_map<std::string, AmberRendering::Resources::Model*> m_models;
		std::unordered_map<std::string, AmberRendering::Resources::Texture*> m_textures;
		std::unordered_map<std::string, AmberRendering::Resources::Shader*> m_shaders;
	};
}

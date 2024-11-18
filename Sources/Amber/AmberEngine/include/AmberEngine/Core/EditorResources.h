#pragma once

#include "AmberEngine/Resources/Model.h"
#include "AmberEngine/Resources/Texture.h"
#include "AmberEngine/Resources/Shader.h"

namespace AmberEngine::Core
{
	class EditorResources
	{
	public:
		EditorResources(const std::string& p_editorAssetsPath);
		~EditorResources();

		Resources::Model* GetModel(const std::string& p_id);
		Resources::Texture* GetTexture(const std::string& p_id);
		Resources::Shader* GetShader(const std::string& p_id);

		Resources::Texture* GetFileIcon(const std::string& p_filename);

	private:
		std::unordered_map<std::string, Resources::Model*> m_models;
		std::unordered_map<std::string, Resources::Texture*> m_textures;
		std::unordered_map<std::string, Resources::Shader*> m_shaders;
	};
}

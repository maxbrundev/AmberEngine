#include "Amberpch.h"

#include "AmberEngine/Core/EditorResources.h"

#include "AmberEngine/Resources/Loaders/ModelLoader.h"
#include "AmberEngine/Resources/Loaders/TextureLoader.h"
#include "AmberEngine/Resources/Loaders/ShaderLoader.h"
#include "AmberEngine/Resources/RawShaders.h"
#include "AmberEngine/Resources/RawTextures.h"

#include "AmberEngine/UI/GUIDrawer.h"

AmberEngine::Core::EditorResources::EditorResources(const std::string& p_editorAssetsPath)
{
	const std::string modelsFolder	= p_editorAssetsPath + "Models\\";
	const std::string shadersFolder	= p_editorAssetsPath + "Shaders\\";

	m_models["Cube"]           = Resources::Loaders::ModelLoader::Create(modelsFolder + "Cube.fbx");
	m_models["Cylinder"]       = Resources::Loaders::ModelLoader::Create(modelsFolder + "Cylinder.fbx");
	m_models["Plane"]          = Resources::Loaders::ModelLoader::Create(modelsFolder + "Plane.fbx");
	m_models["Vertical_Plane"] = Resources::Loaders::ModelLoader::Create(modelsFolder + "Vertical_Plane.fbx");
	m_models["Roll"]           = Resources::Loaders::ModelLoader::Create(modelsFolder + "Roll.fbx");
	m_models["Sphere"]         = Resources::Loaders::ModelLoader::Create(modelsFolder + "Sphere.fbx");

	const auto normalsSource = Resources::RawShader::GetNormalVisualizer();

	m_shaders["NormalsColors"] = Resources::Loaders::ShaderLoader::CreateFromSource(normalsSource.first, normalsSource.second);

	std::vector<uint64_t> raw = EMPTY_TEXTURE;
	m_textures["Empty_Texture"] = Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 64, 64, Resources::ETextureFilteringMode::LINEAR, Resources::ETextureFilteringMode::LINEAR, false);
	UI::GUIDrawer::ProvideEmptyTexture(*m_textures["Empty_Texture"]);
}

AmberEngine::Core::EditorResources::~EditorResources()
{
	for (auto[id, mesh] : m_models)
		Resources::Loaders::ModelLoader::Destroy(mesh);

	for (auto[id, texture] : m_textures)
		Resources::Loaders::TextureLoader::Destroy(texture);

	for (auto[id, shader] : m_shaders)
		Resources::Loaders::ShaderLoader::Destroy(shader);

	m_models.clear();
	m_textures.clear();
	m_shaders.clear();
}

AmberEngine::Resources::Model* AmberEngine::Core::EditorResources::GetModel(const std::string& p_id)
{
	if (m_models.find(p_id) != m_models.end())
		return m_models.at(p_id);

	return nullptr;
}

AmberEngine::Resources::Texture* AmberEngine::Core::EditorResources::GetTexture(const std::string& p_id)
{
	if (m_textures.find(p_id) != m_textures.end())
		return m_textures.at(p_id);

	return nullptr;
}

AmberEngine::Resources::Shader* AmberEngine::Core::EditorResources::GetShader(const std::string& p_id)
{
	if (m_shaders.find(p_id) != m_shaders.end())
		return m_shaders.at(p_id);

	return nullptr;
}

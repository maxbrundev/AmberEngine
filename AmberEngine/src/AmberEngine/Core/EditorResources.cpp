#include "Amberpch.h"

#include "AmberEngine/Core/EditorResources.h"
#include "AmberEngine/Managers/ResourcesManager.h"
#include "AmberEngine/Resources/Loaders/ShaderLoader.h"
#include "AmberEngine/Resources/RawShaders.h"

AmberEngine::Core::EditorResources::EditorResources(const std::string& p_editorAssetsPath)
{
	std::string modelsFolder	= p_editorAssetsPath + "Models\\";
	std::string shadersFolder	= p_editorAssetsPath + "Shaders\\";

	m_models["Cube"]            = &AmberEngine::Managers::ResourcesManager::Instance().LoadModel("Cube", modelsFolder + "Cube.fbx");
	m_models["Cylinder"]        = &AmberEngine::Managers::ResourcesManager::Instance().LoadModel("Cylinder", modelsFolder + "Cylinder.fbx");
	m_models["Plane"]           = &AmberEngine::Managers::ResourcesManager::Instance().LoadModel("Plane", modelsFolder + "Plane.fbx");
	m_models["Vertical_Plane"]  = &AmberEngine::Managers::ResourcesManager::Instance().LoadModel("Vertical_Plane", modelsFolder + "Vertical_Plane.fbx");
	m_models["Roll"]            = &AmberEngine::Managers::ResourcesManager::Instance().LoadModel("Roll", modelsFolder + "Roll.fbx");
	m_models["Sphere"]          = &AmberEngine::Managers::ResourcesManager::Instance().LoadModel("Sphere", modelsFolder + "Sphere.fbx");

	auto normalsSource = Resources::RawShader::GetNormalVisualizer();

	m_shaders["Normals"] = Resources::ShaderLoader::CreateFromSource(normalsSource.first, normalsSource.second);
}

AmberEngine::Core::EditorResources::~EditorResources()
{
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

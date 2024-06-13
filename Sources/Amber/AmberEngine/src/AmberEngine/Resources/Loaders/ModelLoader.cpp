#include "Amberpch.h"

#include "AmberEngine/Resources/Loaders/ModelLoader.h"

AmberEngine::Resources::Parsers::AssimpParser AmberEngine::Resources::Loaders::ModelLoader::__ASSIMP;

std::string AmberEngine::Resources::Loaders::ModelLoader::__FILE_TRACE;

AmberEngine::Resources::Model* AmberEngine::Resources::Loaders::ModelLoader::Create(const std::string& p_filePath)
{
	__FILE_TRACE = p_filePath;

	auto model = new Model(p_filePath);

	if(__ASSIMP.LoadModel(p_filePath, model->m_meshes, model->m_materialNames))
	{

		model->LoadedTextureData = std::move(__ASSIMP.textureData);
		return model;
	}

	delete model;

	return nullptr;
}

void AmberEngine::Resources::Loaders::ModelLoader::Reload(Model& p_model, const std::string& p_filePath)
{
	Model* newModel = Create(p_filePath);

	if (newModel)
	{
		p_model.m_meshes = newModel->m_meshes;
		newModel->m_meshes.clear();
		delete newModel;
	}
}

bool AmberEngine::Resources::Loaders::ModelLoader::Destroy(Model*& p_modelInstance)
{
	if (p_modelInstance)
	{
		delete p_modelInstance;
		p_modelInstance = nullptr;

		return true;
	}

	return false;
}

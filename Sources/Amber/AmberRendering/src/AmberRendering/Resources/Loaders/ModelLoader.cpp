#include "Amberpch.h"

#include "AmberRendering/Resources/Loaders/ModelLoader.h"

AmberRendering::Resources::Parsers::AssimpParser AmberRendering::Resources::Loaders::ModelLoader::__ASSIMP;

std::string AmberRendering::Resources::Loaders::ModelLoader::__FILE_TRACE;

AmberRendering::Resources::Model* AmberRendering::Resources::Loaders::ModelLoader::Create(const std::string& p_filePath, Parsers::EModelParserFlags p_parserFlags)
{
	__FILE_TRACE = p_filePath;

	auto model = new Model(p_filePath);

	if(__ASSIMP.LoadModel(p_filePath, model->m_meshes, model->m_materialNames, p_parserFlags))
	{

		model->LoadedTextureData = std::move(__ASSIMP.textureData);
		model->ComputeBoundingSphere();
		return model;
	}

	delete model;

	return nullptr;
}

void AmberRendering::Resources::Loaders::ModelLoader::Reload(Model& p_model, const std::string& p_filePath, Parsers::EModelParserFlags p_parserFlags)
{
	Model* newModel = Create(p_filePath, p_parserFlags);

	if (newModel)
	{
		p_model.m_meshes = newModel->m_meshes;
		p_model.m_materialNames = newModel->m_materialNames;
		p_model.m_boundingSphere = newModel->m_boundingSphere;
		newModel->m_meshes.clear();
		delete newModel;
	}
}

bool AmberRendering::Resources::Loaders::ModelLoader::Destroy(Model*& p_modelInstance)
{
	if (p_modelInstance)
	{
		delete p_modelInstance;
		p_modelInstance = nullptr;

		return true;
	}

	return false;
}

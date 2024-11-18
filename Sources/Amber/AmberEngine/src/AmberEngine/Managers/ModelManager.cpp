#include "Amberpch.h"

#include "AmberEngine/Managers/ModelManager.h"

#include "AmberEngine/Resources/Loaders/ModelLoader.h"
#include "AmberEngine/Tools/FileSystem/IniFile.h"

AmberEngine::Resources::Parsers::EModelParserFlags GetAssetMetadata(const std::string& p_path)
{
	auto metaFile = AmberEngine::Tools::Filesystem::IniFile(p_path + ".meta");

	AmberEngine::Resources::Parsers::EModelParserFlags flags = AmberEngine::Resources::Parsers::EModelParserFlags::NONE;

	if (metaFile.GetOrDefault("CALC_TANGENT_SPACE", true))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::CALC_TANGENT_SPACE;
	if (metaFile.GetOrDefault("JOIN_IDENTICAL_VERTICES", true))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::JOIN_IDENTICAL_VERTICES;
	if (metaFile.GetOrDefault("MAKE_LEFT_HANDED", false))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::MAKE_LEFT_HANDED;
	if (metaFile.GetOrDefault("TRIANGULATE", true))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::TRIANGULATE;
	if (metaFile.GetOrDefault("REMOVE_COMPONENT", false))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::REMOVE_COMPONENT;
	if (metaFile.GetOrDefault("GEN_NORMALS", false))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::GEN_NORMALS;
	if (metaFile.GetOrDefault("GEN_SMOOTH_NORMALS", true))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::GEN_SMOOTH_NORMALS;
	if (metaFile.GetOrDefault("SPLIT_LARGE_MESHES", false))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::SPLIT_LARGE_MESHES;
	if (metaFile.GetOrDefault("PRE_TRANSFORM_VERTICES", true))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::PRE_TRANSFORM_VERTICES;
	if (metaFile.GetOrDefault("LIMIT_BONE_WEIGHTS", false))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::LIMIT_BONE_WEIGHTS;
	if (metaFile.GetOrDefault("VALIDATE_DATA_STRUCTURE", false))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::VALIDATE_DATA_STRUCTURE;
	if (metaFile.GetOrDefault("IMPROVE_CACHE_LOCALITY", true))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::IMPROVE_CACHE_LOCALITY;
	if (metaFile.GetOrDefault("REMOVE_REDUNDANT_MATERIALS", false))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::REMOVE_REDUNDANT_MATERIALS;
	if (metaFile.GetOrDefault("FIX_INFACING_NORMALS", false))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::FIX_INFACING_NORMALS;
	if (metaFile.GetOrDefault("SORT_BY_PTYPE", false))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::SORT_BY_PTYPE;
	if (metaFile.GetOrDefault("FIND_DEGENERATES", false))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::FIND_DEGENERATES;
	if (metaFile.GetOrDefault("FIND_INVALID_DATA", true))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::FIND_INVALID_DATA;
	if (metaFile.GetOrDefault("GEN_UV_COORDS", true))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::GEN_UV_COORDS;
	if (metaFile.GetOrDefault("TRANSFORM_UV_COORDS", false))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::TRANSFORM_UV_COORDS;
	if (metaFile.GetOrDefault("FIND_INSTANCES", true))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::FIND_INSTANCES;
	if (metaFile.GetOrDefault("OPTIMIZE_MESHES", true))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::OPTIMIZE_MESHES;
	if (metaFile.GetOrDefault("OPTIMIZE_GRAPH", true))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::OPTIMIZE_GRAPH;
	if (metaFile.GetOrDefault("FLIP_UVS", false))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::FLIP_UVS;
	if (metaFile.GetOrDefault("FLIP_WINDING_ORDER", false))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::FLIP_WINDING_ORDER;
	if (metaFile.GetOrDefault("SPLIT_BY_BONE_COUNT", false))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::SPLIT_BY_BONE_COUNT;
	if (metaFile.GetOrDefault("DEBONE", true))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::DEBONE;
	if (metaFile.GetOrDefault("GLOBAL_SCALE", true))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::GLOBAL_SCALE;
	if (metaFile.GetOrDefault("EMBED_TEXTURES", false))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::EMBED_TEXTURES;
	if (metaFile.GetOrDefault("FORCE_GEN_NORMALS", false))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::FORCE_GEN_NORMALS;
	if (metaFile.GetOrDefault("DROP_NORMALS", false))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::DROP_NORMALS;
	if (metaFile.GetOrDefault("GEN_BOUNDING_BOXES", false))	flags |= AmberEngine::Resources::Parsers::EModelParserFlags::GEN_BOUNDING_BOXES;

	return { flags };
}

AmberEngine::Resources::Model* AmberEngine::ResourceManagement::ModelManager::CreateResource(const std::string& p_path, bool isRealPathRequired)
{
	const std::string realPath = GetRealPath(p_path);

	const auto model = Resources::Loaders::ModelLoader::Create(isRealPathRequired ? realPath : p_path, GetAssetMetadata(realPath));

	if (model)
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(model) + offsetof(Resources::Model, Path)) = p_path; // Force the resource path to fit the given path

	return model;
}

void AmberEngine::ResourceManagement::ModelManager::DestroyResource(Resources::Model* p_resource)
{
	Resources::Loaders::ModelLoader::Destroy(p_resource);
}

void AmberEngine::ResourceManagement::ModelManager::ReloadResource(Resources::Model* p_resource, const std::string& p_path, bool isRealPathRequired)
{
	const std::string realPath = GetRealPath(p_path);
	Resources::Loaders::ModelLoader::Reload(*p_resource, isRealPathRequired ? realPath : p_path, GetAssetMetadata(realPath));
}

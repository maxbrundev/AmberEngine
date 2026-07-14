#include "Amberpch.h"

#include "AmberCore/ResourceManagement/ModelManager.h"

#include "AmberTools/Global/ServiceLocator.h"

#include "AmberRendering/Resources/Loaders/ModelLoader.h"
#include "AmberTools/FileSystem/IniFile.h"

AmberRendering::Resources::Parsers::EModelParserFlags GetAssetMetadata(const std::string& p_path)
{
	auto metaFile = AmberTools::FileSystem::IniFile(p_path + ".meta");

	AmberRendering::Resources::Parsers::EModelParserFlags flags = AmberRendering::Resources::Parsers::EModelParserFlags::NONE;

	if (metaFile.GetOrDefault("CALC_TANGENT_SPACE", true))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::CALC_TANGENT_SPACE;
	if (metaFile.GetOrDefault("JOIN_IDENTICAL_VERTICES", true))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::JOIN_IDENTICAL_VERTICES;
	if (metaFile.GetOrDefault("MAKE_LEFT_HANDED", false))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::MAKE_LEFT_HANDED;
	if (metaFile.GetOrDefault("TRIANGULATE", true))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::TRIANGULATE;
	if (metaFile.GetOrDefault("REMOVE_COMPONENT", false))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::REMOVE_COMPONENT;
	if (metaFile.GetOrDefault("GEN_NORMALS", false))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::GEN_NORMALS;
	if (metaFile.GetOrDefault("GEN_SMOOTH_NORMALS", true))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::GEN_SMOOTH_NORMALS;
	if (metaFile.GetOrDefault("SPLIT_LARGE_MESHES", false))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::SPLIT_LARGE_MESHES;
	if (metaFile.GetOrDefault("PRE_TRANSFORM_VERTICES", true))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::PRE_TRANSFORM_VERTICES;
	if (metaFile.GetOrDefault("LIMIT_BONE_WEIGHTS", false))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::LIMIT_BONE_WEIGHTS;
	if (metaFile.GetOrDefault("VALIDATE_DATA_STRUCTURE", false))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::VALIDATE_DATA_STRUCTURE;
	if (metaFile.GetOrDefault("IMPROVE_CACHE_LOCALITY", true))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::IMPROVE_CACHE_LOCALITY;
	if (metaFile.GetOrDefault("REMOVE_REDUNDANT_MATERIALS", false))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::REMOVE_REDUNDANT_MATERIALS;
	if (metaFile.GetOrDefault("FIX_INFACING_NORMALS", false))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::FIX_INFACING_NORMALS;
	if (metaFile.GetOrDefault("SORT_BY_PTYPE", false))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::SORT_BY_PTYPE;
	if (metaFile.GetOrDefault("FIND_DEGENERATES", false))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::FIND_DEGENERATES;
	if (metaFile.GetOrDefault("FIND_INVALID_DATA", true))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::FIND_INVALID_DATA;
	if (metaFile.GetOrDefault("GEN_UV_COORDS", true))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::GEN_UV_COORDS;
	if (metaFile.GetOrDefault("TRANSFORM_UV_COORDS", false))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::TRANSFORM_UV_COORDS;
	if (metaFile.GetOrDefault("FIND_INSTANCES", true))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::FIND_INSTANCES;
	if (metaFile.GetOrDefault("OPTIMIZE_MESHES", true))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::OPTIMIZE_MESHES;
	if (metaFile.GetOrDefault("OPTIMIZE_GRAPH", true))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::OPTIMIZE_GRAPH;
	if (metaFile.GetOrDefault("FLIP_UVS", false))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::FLIP_UVS;
	if (metaFile.GetOrDefault("FLIP_WINDING_ORDER", false))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::FLIP_WINDING_ORDER;
	if (metaFile.GetOrDefault("SPLIT_BY_BONE_COUNT", false))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::SPLIT_BY_BONE_COUNT;
	if (metaFile.GetOrDefault("DEBONE", true))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::DEBONE;
	if (metaFile.GetOrDefault("GLOBAL_SCALE", true))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::GLOBAL_SCALE;
	if (metaFile.GetOrDefault("EMBED_TEXTURES", false))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::EMBED_TEXTURES;
	if (metaFile.GetOrDefault("FORCE_GEN_NORMALS", false))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::FORCE_GEN_NORMALS;
	if (metaFile.GetOrDefault("DROP_NORMALS", false))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::DROP_NORMALS;
	if (metaFile.GetOrDefault("GEN_BOUNDING_BOXES", false))	flags |= AmberRendering::Resources::Parsers::EModelParserFlags::GEN_BOUNDING_BOXES;

	return { flags };
}

AmberRendering::Resources::Model* AmberCore::ResourceManagement::ModelManager::CreateResource(const std::string& p_path, bool isRealPathRequired)
{
	const std::string realPath = GetRealPath(p_path);

	const auto model = AmberRendering::Resources::Loaders::ModelLoader::Create(isRealPathRequired ? realPath : p_path, GetAssetMetadata(realPath));

	if (model)
	{
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(model) + offsetof(AmberRendering::Resources::Model, Path)) = p_path; // Force the resource path to fit the given path
	}

	return model;
}

void AmberCore::ResourceManagement::ModelManager::DestroyResource(AmberRendering::Resources::Model* p_resource)
{
	AmberRendering::Resources::Loaders::ModelLoader::Destroy(p_resource);
}

void AmberCore::ResourceManagement::ModelManager::ReloadResource(AmberRendering::Resources::Model* p_resource, const std::string& p_path, bool isRealPathRequired)
{
	const std::string realPath = GetRealPath(p_path);
	AmberRendering::Resources::Loaders::ModelLoader::Reload(*p_resource, isRealPathRequired ? realPath : p_path, GetAssetMetadata(realPath));
}

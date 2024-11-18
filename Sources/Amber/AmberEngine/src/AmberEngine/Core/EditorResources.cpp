#include "Amberpch.h"

#include "AmberEngine/Core/EditorResources.h"

#include "AmberEngine/Resources/RawShaders.h"
#include "AmberEngine/Resources/RawTextures.h"
#include "AmberEngine/Resources/Loaders/ModelLoader.h"
#include "AmberEngine/Resources/Loaders/ShaderLoader.h"
#include "AmberEngine/Resources/Loaders/TextureLoader.h"

#include "AmberEngine/Tools/Utils/PathParser.h"

#include "AmberEngine/UI/GUIDrawer.h"

AmberEngine::Core::EditorResources::EditorResources(const std::string& p_editorAssetsPath)
{
	const std::string modelsFolder	= p_editorAssetsPath + "Models\\";
	const std::string shadersFolder	= p_editorAssetsPath + "Shaders\\";

	Resources::Parsers::EModelParserFlags modelParserFlags = Resources::Parsers::EModelParserFlags::NONE;

	modelParserFlags |= Resources::Parsers::EModelParserFlags::TRIANGULATE;
	modelParserFlags |= Resources::Parsers::EModelParserFlags::GEN_SMOOTH_NORMALS;
	modelParserFlags |= Resources::Parsers::EModelParserFlags::OPTIMIZE_MESHES;
	modelParserFlags |= Resources::Parsers::EModelParserFlags::OPTIMIZE_GRAPH;
	modelParserFlags |= Resources::Parsers::EModelParserFlags::FIND_INSTANCES;
	modelParserFlags |= Resources::Parsers::EModelParserFlags::CALC_TANGENT_SPACE;
	modelParserFlags |= Resources::Parsers::EModelParserFlags::JOIN_IDENTICAL_VERTICES;
	modelParserFlags |= Resources::Parsers::EModelParserFlags::DEBONE;
	modelParserFlags |= Resources::Parsers::EModelParserFlags::FIND_INVALID_DATA;
	modelParserFlags |= Resources::Parsers::EModelParserFlags::IMPROVE_CACHE_LOCALITY;
	modelParserFlags |= Resources::Parsers::EModelParserFlags::GEN_UV_COORDS;
	modelParserFlags |= Resources::Parsers::EModelParserFlags::PRE_TRANSFORM_VERTICES;
	modelParserFlags |= Resources::Parsers::EModelParserFlags::GLOBAL_SCALE;

	Resources::ETextureFilteringMode firstFilterEditor  = Resources::ETextureFilteringMode::LINEAR;
	Resources::ETextureFilteringMode secondFilterEditor = Resources::ETextureFilteringMode::LINEAR;

	m_models["Cube"]            = Resources::Loaders::ModelLoader::Create(modelsFolder + "Cube.fbx", modelParserFlags);
	m_models["Cylinder"]        = Resources::Loaders::ModelLoader::Create(modelsFolder + "Cylinder.fbx", modelParserFlags);
	m_models["Plane"]           = Resources::Loaders::ModelLoader::Create(modelsFolder + "Plane.fbx", modelParserFlags);
	m_models["Vertical_Plane"]  = Resources::Loaders::ModelLoader::Create(modelsFolder + "Vertical_Plane.fbx", modelParserFlags);
	m_models["Roll"]            = Resources::Loaders::ModelLoader::Create(modelsFolder + "Roll.fbx", modelParserFlags);
	m_models["Sphere"]          = Resources::Loaders::ModelLoader::Create(modelsFolder + "Sphere.fbx", modelParserFlags);
	m_models["Arrow_Translate"] = Resources::Loaders::ModelLoader::Create(modelsFolder + "Arrow_Translate.fbx", modelParserFlags);
	m_models["Arrow_Rotate"]    = Resources::Loaders::ModelLoader::Create(modelsFolder + "Arrow_Rotate.fbx", modelParserFlags);
	m_models["Arrow_Scale"]     = Resources::Loaders::ModelLoader::Create(modelsFolder + "Arrow_Scale.fbx", modelParserFlags);
	m_models["Arrow_Picking"]   = Resources::Loaders::ModelLoader::Create(modelsFolder + "Arrow_Picking.fbx", modelParserFlags);
	m_models["Camera"]          = Resources::Loaders::ModelLoader::Create(modelsFolder + "Camera.fbx", modelParserFlags);

	const auto gridSource    = Resources::RawShader::GetGrid();
	const auto gizmoSource   = Resources::RawShader::GetGizmo();
	const auto normalsSource = Resources::RawShader::GetNormalVisualizer();

	m_shaders["Grid"]          = Resources::Loaders::ShaderLoader::CreateFromSource(gridSource.first, gridSource.second);
	m_shaders["Gizmo"]         = Resources::Loaders::ShaderLoader::CreateFromSource(gizmoSource.first, gizmoSource.second);
	m_shaders["NormalsColors"] = Resources::Loaders::ShaderLoader::CreateFromSource(normalsSource.first, normalsSource.second);

	std::vector<uint64_t> raw = EMPTY_TEXTURE;
	m_textures["Empty_Texture"] = Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 64, 64, firstFilterEditor, secondFilterEditor, false);
	UI::GUIDrawer::ProvideEmptyTexture(*m_textures["Empty_Texture"]);

	{
		std::vector<uint64_t> raw = BUTTON_PLAY;
		m_textures["Button_Play"] = Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 64, 64, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = BUTTON_PAUSE;
		m_textures["Button_Pause"] = Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 64, 64, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = BUTTON_STOP;
		m_textures["Button_Stop"] = Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 64, 64, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = BUTTON_NEXT;
		m_textures["Button_Next"] = Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 64, 64, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = BUTTON_REFRESH;
		m_textures["Button_Refresh"] = Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 64, 64, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_FILE;
		m_textures["Icon_Unknown"] = Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_FOLDER;
		m_textures["Icon_Folder"] = Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_TEXTURE;
		m_textures["Icon_Texture"] = Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_MODEL;
		m_textures["Icon_Model"] = Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_SHADER;
		m_textures["Icon_Shader"] = Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_MATERIAL;
		m_textures["Icon_Material"] = Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_SCENE;
		m_textures["Icon_Scene"] = Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_SOUND;
		m_textures["Icon_Sound"] = Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_SCRIPT;
		m_textures["Icon_Script"] = Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_FONT;
		m_textures["Icon_Font"] = Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}
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

AmberEngine::Resources::Texture* AmberEngine::Core::EditorResources::GetFileIcon(const std::string& p_filename)
{
	return GetTexture("Icon_" + Tools::Utils::PathParser::FileTypeToString(Tools::Utils::PathParser::GetFileType(p_filename)));
}

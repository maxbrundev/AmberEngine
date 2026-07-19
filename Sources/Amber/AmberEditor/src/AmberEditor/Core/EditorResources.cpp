#include "Amberpch.h"

#include "AmberEditor/Core/EditorResources.h"

#include "AmberRendering/Resources/RawShaders.h"
#include "AmberRendering/Resources/RawTextures.h"
#include "AmberRendering/Resources/Loaders/ModelLoader.h"
#include "AmberRendering/Resources/Loaders/ShaderLoader.h"
#include "AmberRendering/Resources/Loaders/TextureLoader.h"

#include "AmberTools/Utils/PathParser.h"

#include "AmberCore/Helpers/GUIDrawer.h"

AmberEditor::Core::EditorResources::EditorResources(const std::string& p_editorAssetsPath)
{
	const std::string modelsFolder	= p_editorAssetsPath + "Models\\";
	const std::string shadersFolder	= p_editorAssetsPath + "Shaders\\";

	AmberRendering::Resources::Parsers::EModelParserFlags modelParserFlags = AmberRendering::Resources::Parsers::EModelParserFlags::NONE;

	modelParserFlags |= AmberRendering::Resources::Parsers::EModelParserFlags::TRIANGULATE;
	modelParserFlags |= AmberRendering::Resources::Parsers::EModelParserFlags::GEN_SMOOTH_NORMALS;
	modelParserFlags |= AmberRendering::Resources::Parsers::EModelParserFlags::OPTIMIZE_MESHES;
	modelParserFlags |= AmberRendering::Resources::Parsers::EModelParserFlags::OPTIMIZE_GRAPH;
	modelParserFlags |= AmberRendering::Resources::Parsers::EModelParserFlags::FIND_INSTANCES;
	modelParserFlags |= AmberRendering::Resources::Parsers::EModelParserFlags::CALC_TANGENT_SPACE;
	modelParserFlags |= AmberRendering::Resources::Parsers::EModelParserFlags::JOIN_IDENTICAL_VERTICES;
	modelParserFlags |= AmberRendering::Resources::Parsers::EModelParserFlags::DEBONE;
	modelParserFlags |= AmberRendering::Resources::Parsers::EModelParserFlags::FIND_INVALID_DATA;
	modelParserFlags |= AmberRendering::Resources::Parsers::EModelParserFlags::IMPROVE_CACHE_LOCALITY;
	modelParserFlags |= AmberRendering::Resources::Parsers::EModelParserFlags::GEN_UV_COORDS;
	modelParserFlags |= AmberRendering::Resources::Parsers::EModelParserFlags::PRE_TRANSFORM_VERTICES;
	modelParserFlags |= AmberRendering::Resources::Parsers::EModelParserFlags::GLOBAL_SCALE;

	AmberRendering::Resources::ETextureFilteringMode firstFilterEditor  = AmberRendering::Resources::ETextureFilteringMode::LINEAR;
	AmberRendering::Resources::ETextureFilteringMode secondFilterEditor = AmberRendering::Resources::ETextureFilteringMode::LINEAR;

	m_models["Cube"]            = AmberRendering::Resources::Loaders::ModelLoader::Create(modelsFolder + "Cube.fbx", modelParserFlags);
	m_models["Cylinder"]        = AmberRendering::Resources::Loaders::ModelLoader::Create(modelsFolder + "Cylinder.fbx", modelParserFlags);
	m_models["Plane"]           = AmberRendering::Resources::Loaders::ModelLoader::Create(modelsFolder + "Plane.fbx", modelParserFlags);
	m_models["Vertical_Plane"]  = AmberRendering::Resources::Loaders::ModelLoader::Create(modelsFolder + "Vertical_Plane.fbx", modelParserFlags);
	m_models["Roll"]            = AmberRendering::Resources::Loaders::ModelLoader::Create(modelsFolder + "Roll.fbx", modelParserFlags);
	m_models["Sphere"]          = AmberRendering::Resources::Loaders::ModelLoader::Create(modelsFolder + "Sphere.fbx", modelParserFlags);
	m_models["Arrow_Translate"] = AmberRendering::Resources::Loaders::ModelLoader::Create(modelsFolder + "Arrow_Translate.fbx", modelParserFlags);
	m_models["Arrow_Rotate"]    = AmberRendering::Resources::Loaders::ModelLoader::Create(modelsFolder + "Arrow_Rotate.fbx", modelParserFlags);
	m_models["Arrow_Scale"]     = AmberRendering::Resources::Loaders::ModelLoader::Create(modelsFolder + "Arrow_Scale.fbx", modelParserFlags);
	m_models["Arrow_Picking"]   = AmberRendering::Resources::Loaders::ModelLoader::Create(modelsFolder + "Arrow_Picking.fbx", modelParserFlags);
	m_models["Camera"]          = AmberRendering::Resources::Loaders::ModelLoader::Create(modelsFolder + "Camera.fbx", modelParserFlags);

	AmberRendering::Resources::ETextureFilteringMode firstFilterBillboard  = AmberRendering::Resources::ETextureFilteringMode::NEAREST;
	AmberRendering::Resources::ETextureFilteringMode secondFilterBillboard = AmberRendering::Resources::ETextureFilteringMode::NEAREST;

	const auto gridSource      = AmberRendering::Resources::RawShader::GetGrid();
	const auto gizmoSource     = AmberRendering::Resources::RawShader::GetGizmo();
	const auto normalsSource   = AmberRendering::Resources::RawShader::GetNormalVisualizer();
	const auto billboardSource = AmberRendering::Resources::RawShader::GetBillboard();

	m_shaders["Grid"]          = AmberRendering::Resources::Loaders::ShaderLoader::CreateFromSource(gridSource.first, gridSource.second);
	m_shaders["Gizmo"]         = AmberRendering::Resources::Loaders::ShaderLoader::CreateFromSource(gizmoSource.first, gizmoSource.second);
	m_shaders["NormalsColors"] = AmberRendering::Resources::Loaders::ShaderLoader::CreateFromSource(normalsSource.first, normalsSource.second);
	m_shaders["Billboard"]     = AmberRendering::Resources::Loaders::ShaderLoader::CreateFromSource(billboardSource.first, billboardSource.second);

	std::vector<uint64_t> raw = EMPTY_TEXTURE;
	m_textures["Empty_Texture"] = AmberRendering::Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 64, 64, firstFilterEditor, secondFilterEditor, false);
	AmberCore::Helpers::GUIDrawer::ProvideEmptyTexture(*m_textures["Empty_Texture"]);

	{
		std::vector<uint64_t> raw = BILL_PLIGHT;
		m_textures["Bill_Point_Light"] = AmberRendering::Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 128, 128, firstFilterBillboard, secondFilterBillboard, false);
	}

	{
		std::vector<uint64_t> raw = BILL_SLIGHT;
		m_textures["Bill_Spot_Light"] = AmberRendering::Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 128, 128, firstFilterBillboard, secondFilterBillboard, false);
	}

	{
		std::vector<uint64_t> raw = BILL_DLIGHT;
		m_textures["Bill_Directional_Light"] = AmberRendering::Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 128, 128, firstFilterBillboard, secondFilterBillboard, false);
	}

	{
		std::vector<uint64_t> raw = BILL_ABLIGHT;
		m_textures["Bill_Ambient_Box_Light"] = AmberRendering::Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 128, 128, firstFilterBillboard, secondFilterBillboard, false);
	}

	{
		std::vector<uint64_t> raw = BILL_ASLIGHT;
		m_textures["Bill_Ambient_Sphere_Light"] = AmberRendering::Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 128, 128, firstFilterBillboard, secondFilterBillboard, false);
	}

	{
		std::vector<uint64_t> raw = BUTTON_PLAY;
		m_textures["Button_Play"] = AmberRendering::Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 64, 64, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = BUTTON_PAUSE;
		m_textures["Button_Pause"] = AmberRendering::Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 64, 64, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = BUTTON_STOP;
		m_textures["Button_Stop"] = AmberRendering::Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 64, 64, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = BUTTON_NEXT;
		m_textures["Button_Next"] = AmberRendering::Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 64, 64, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = BUTTON_REFRESH;
		m_textures["Button_Refresh"] = AmberRendering::Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 64, 64, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_FILE;
		m_textures["Icon_Unknown"] = AmberRendering::Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_FOLDER;
		m_textures["Icon_Folder"] = AmberRendering::Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_TEXTURE;
		m_textures["Icon_Texture"] = AmberRendering::Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_MODEL;
		m_textures["Icon_Model"] = AmberRendering::Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_SHADER;
		m_textures["Icon_Shader"] = AmberRendering::Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_MATERIAL;
		m_textures["Icon_Material"] = AmberRendering::Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_SCENE;
		m_textures["Icon_Scene"] = AmberRendering::Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_SOUND;
		m_textures["Icon_Sound"] = AmberRendering::Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_SCRIPT;
		m_textures["Icon_Script"] = AmberRendering::Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}

	{
		std::vector<uint64_t> raw = ICON_FONT;
		m_textures["Icon_Font"] = AmberRendering::Resources::Loaders::TextureLoader::CreateFromMemory(reinterpret_cast<uint8_t*>(raw.data()), 16, 16, firstFilterEditor, secondFilterEditor, false);
	}
}

AmberEditor::Core::EditorResources::~EditorResources()
{
	for (auto[id, mesh] : m_models)
		AmberRendering::Resources::Loaders::ModelLoader::Destroy(mesh);

	for (auto[id, texture] : m_textures)
		AmberRendering::Resources::Loaders::TextureLoader::Destroy(texture);

	for (auto[id, shader] : m_shaders)
		AmberRendering::Resources::Loaders::ShaderLoader::Destroy(shader);

	m_models.clear();
	m_textures.clear();
	m_shaders.clear();
}

AmberRendering::Resources::Model* AmberEditor::Core::EditorResources::GetModel(const std::string& p_id)
{
	if (m_models.find(p_id) != m_models.end())
		return m_models.at(p_id);

	return nullptr;
}

AmberRendering::Resources::Texture* AmberEditor::Core::EditorResources::GetTexture(const std::string& p_id)
{
	if (m_textures.find(p_id) != m_textures.end())
		return m_textures.at(p_id);

	return nullptr;
}

AmberRendering::Resources::Shader* AmberEditor::Core::EditorResources::GetShader(const std::string& p_id)
{
	if (m_shaders.find(p_id) != m_shaders.end())
		return m_shaders.at(p_id);

	return nullptr;
}

AmberRendering::Resources::Texture* AmberEditor::Core::EditorResources::GetFileIcon(const std::string& p_filename)
{
	return GetTexture("Icon_" + AmberTools::Utils::PathParser::FileTypeToString(AmberTools::Utils::PathParser::GetFileType(p_filename)));
}

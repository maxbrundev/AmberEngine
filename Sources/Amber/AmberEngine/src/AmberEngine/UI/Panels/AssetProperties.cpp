#include "Amberpch.h"

#include "AmberEngine/UI/Panels/AssetProperties.h"

#include "AmberEngine/Core/EditorAction.h"
#include "AmberEngine/Tools/Utils/SizeConverter.h"
#include "AmberEngine/UI/GUIDrawer.h"
#include "AmberEngine/UI/Panels/Views/AssetView.h"
#include "AmberEngine/UI/Widgets/ComboBox.h"
#include "AmberEngine/UI/Widgets/GroupCollapsable.h"
#include "AmberEngine/UI/Widgets/NewLine.h"
#include "AmberEngine/UI/Widgets/Separator.h"

AmberEngine::UI::Panels::AssetProperties::AssetProperties(const std::string& p_title, bool p_opened,
                                                          const PanelSettings& p_windowSettings) : APanelWindow(p_title, p_opened, p_windowSettings)
{
	m_targetChanged += [this]() { SetTarget(m_assetSelector->content); };

	CreateHeaderButtons();

	m_headerSeparator = &CreateWidget<Widgets::Separator>();
	m_headerSeparator->enabled = false;

	CreateAssetSelector();

	m_settings = &CreateWidget<Widgets::GroupCollapsable>("Settings");
	m_settingsColumns = &m_settings->CreateWidget<Widgets::Columns<2>>();
	m_settingsColumns->widths[0] = 150;

	m_info = &CreateWidget<Widgets::GroupCollapsable>("Info");
	m_infoColumns = &m_info->CreateWidget<Widgets::Columns<2>>();
	m_infoColumns->widths[0] = 150;

	m_settings->enabled = m_info->enabled = false;
}

void AmberEngine::UI::Panels::AssetProperties::SetTarget(const std::string& p_path)
{
	m_resource = p_path == "" ? p_path : EDITOR_EXEC(GetResourcePath(p_path));

	if (m_assetSelector)
	{
		m_assetSelector->content = m_resource;
	}

	Refresh();
}

void AmberEngine::UI::Panels::AssetProperties::Refresh()
{
	m_metadata.reset(new Tools::Filesystem::IniFile(EDITOR_EXEC(GetRealPath(m_resource)) + ".meta"));

	CreateSettings();
	CreateInfo();

	m_applyButton->enabled = m_settings->enabled;
	m_resetButton->enabled = m_settings->enabled;
	m_revertButton->enabled = m_settings->enabled;

	switch (Tools::Utils::PathParser::GetFileType(m_resource))
	{
	case Tools::Utils::PathParser::EFileType::MODEL:
	case Tools::Utils::PathParser::EFileType::TEXTURE:
	case Tools::Utils::PathParser::EFileType::MATERIAL:
		m_previewButton->enabled = true;
		break;
	default:
		m_previewButton->enabled = false;
		break;
	}

	// Enables the header separator (And the line break) if at least one button is enabled
	m_headerSeparator->enabled = m_applyButton->enabled || m_resetButton->enabled || m_revertButton->enabled || m_previewButton->enabled;
	m_headerLineBreak->enabled = m_headerSeparator->enabled;
}

void AmberEngine::UI::Panels::AssetProperties::Preview()
{
	auto& assetView = EDITOR_PANEL(AssetView, "Asset View");

	const auto fileType = Tools::Utils::PathParser::GetFileType(m_resource);

	if (fileType == Tools::Utils::PathParser::EFileType::MODEL)
	{
		if (auto resource = Tools::Global::ServiceLocator::Get<ResourceManagement::ModelManager>().GetResource(m_resource))
		{
			assetView.SetResource(resource);
		}
	}
	else if (fileType == Tools::Utils::PathParser::EFileType::TEXTURE)
	{
		if (auto resource = Tools::Global::ServiceLocator::Get<ResourceManagement::TextureManager>().GetResource(m_resource))
		{
			assetView.SetResource(resource);
		}
	}

	assetView.Open();
}

void AmberEngine::UI::Panels::AssetProperties::CreateHeaderButtons()
{
	m_applyButton = &CreateWidget<Widgets::Button>("Apply");
	m_applyButton->idleBackgroundColor = { 0.0f, 0.5f, 0.0f };
	m_applyButton->enabled = false;
	m_applyButton->lineBreak = false;
	m_applyButton->ClickedEvent += std::bind(&AssetProperties::Apply, this);

	m_revertButton = &CreateWidget<Widgets::Button>("Revert");
	m_revertButton->idleBackgroundColor = { 0.7f, 0.5f, 0.0f };
	m_revertButton->enabled = false;
	m_revertButton->lineBreak = false;
	m_revertButton->ClickedEvent += std::bind(&AssetProperties::SetTarget, this, m_resource);

	m_previewButton = &CreateWidget<Widgets::Button>("Preview");
	m_previewButton->idleBackgroundColor = { 0.7f, 0.5f, 0.0f };
	m_previewButton->enabled = false;
	m_previewButton->lineBreak = false;
	m_previewButton->ClickedEvent += std::bind(&AssetProperties::Preview, this);

	m_resetButton = &CreateWidget<Widgets::Button>("Reset to default");
	m_resetButton->idleBackgroundColor = { 0.5f, 0.0f, 0.0f };
	m_resetButton->enabled = false;
	m_resetButton->lineBreak = false;
	m_resetButton->ClickedEvent += [this]
	{
		m_metadata->RemoveAll();
		CreateSettings();
	};

	m_headerLineBreak = &CreateWidget<Widgets::NewLine>();
	m_headerLineBreak->enabled = false;
}

void AmberEngine::UI::Panels::AssetProperties::CreateAssetSelector()
{
	auto& columns = CreateWidget<Widgets::Columns<2>>();
	columns.widths[0] = 150;
	m_assetSelector = &GUIDrawer::DrawAsset(columns, "Target", m_resource, &m_targetChanged);
}

void AmberEngine::UI::Panels::AssetProperties::CreateSettings()
{
	m_settingsColumns->RemoveAllWidgets();

	const auto fileType = Tools::Utils::PathParser::GetFileType(m_resource);

	m_settings->enabled = true;

	if (fileType == Tools::Utils::PathParser::EFileType::MODEL)
	{
		CreateModelSettings();
	}
	else if (fileType == Tools::Utils::PathParser::EFileType::TEXTURE)
	{
		CreateTextureSettings();
	}
	else
	{
		m_settings->enabled = false;
	}
}

void AmberEngine::UI::Panels::AssetProperties::CreateInfo()
{
	const auto realPath = EDITOR_EXEC(GetRealPath(m_resource));

	m_infoColumns->RemoveAllWidgets();

	if (std::filesystem::exists(realPath))
	{
		m_info->enabled = true;

		GUIDrawer::CreateTitle(*m_infoColumns, "Path");
		m_infoColumns->CreateWidget<Widgets::Text>(realPath);

		GUIDrawer::CreateTitle(*m_infoColumns, "Size");
		const auto[size, unit] = Tools::Utils::SizeConverter::ConvertToOptimalUnit(static_cast<float>(std::filesystem::file_size(realPath)), Tools::Utils::SizeConverter::ESizeUnit::BYTE);
		m_infoColumns->CreateWidget<Widgets::Text>(std::to_string(size) + " " + Tools::Utils::SizeConverter::UnitToString(unit));

		GUIDrawer::CreateTitle(*m_infoColumns, "Metadata");
		m_infoColumns->CreateWidget<Widgets::Text>(std::filesystem::exists(realPath + ".meta") ? "Yes" : "No");
	}
	else
	{
		m_info->enabled = false;
	}
}

#define MODEL_FLAG_ENTRY(setting) AmberEngine::UI::GUIDrawer::DrawBoolean(*m_settingsColumns, setting, [&]() { return m_metadata->Get<bool>(setting); }, [&](bool value) { m_metadata->Set<bool>(setting, value); })


void AmberEngine::UI::Panels::AssetProperties::CreateModelSettings()
{
	m_metadata->Add("CALC_TANGENT_SPACE", true);
	m_metadata->Add("JOIN_IDENTICAL_VERTICES", true);
	m_metadata->Add("MAKE_LEFT_HANDED", false);
	m_metadata->Add("TRIANGULATE", true);
	m_metadata->Add("REMOVE_COMPONENT", false);
	m_metadata->Add("GEN_NORMALS", false);
	m_metadata->Add("GEN_SMOOTH_NORMALS", true);
	m_metadata->Add("SPLIT_LARGE_MESHES", false);
	m_metadata->Add("PRE_TRANSFORM_VERTICES", true);
	m_metadata->Add("LIMIT_BONE_WEIGHTS", false);
	m_metadata->Add("VALIDATE_DATA_STRUCTURE", false);
	m_metadata->Add("IMPROVE_CACHE_LOCALITY", true);
	m_metadata->Add("REMOVE_REDUNDANT_MATERIALS", false);
	m_metadata->Add("FIX_INFACING_NORMALS", false);
	m_metadata->Add("SORT_BY_PTYPE", false);
	m_metadata->Add("FIND_DEGENERATES", false);
	m_metadata->Add("FIND_INVALID_DATA", true);
	m_metadata->Add("GEN_UV_COORDS", true);
	m_metadata->Add("TRANSFORM_UV_COORDS", false);
	m_metadata->Add("FIND_INSTANCES", true);
	m_metadata->Add("OPTIMIZE_MESHES", true);
	m_metadata->Add("OPTIMIZE_GRAPH", true);
	m_metadata->Add("FLIP_UVS", false);
	m_metadata->Add("FLIP_WINDING_ORDER", false);
	m_metadata->Add("SPLIT_BY_BONE_COUNT", false);
	m_metadata->Add("DEBONE", true);
	m_metadata->Add("GLOBAL_SCALE", true);
	m_metadata->Add("EMBED_TEXTURES", false);
	m_metadata->Add("FORCE_GEN_NORMALS", false);
	m_metadata->Add("DROP_NORMALS", false);
	m_metadata->Add("GEN_BOUNDING_BOXES", false);

	MODEL_FLAG_ENTRY("CALC_TANGENT_SPACE");
	MODEL_FLAG_ENTRY("JOIN_IDENTICAL_VERTICES");
	MODEL_FLAG_ENTRY("MAKE_LEFT_HANDED");
	MODEL_FLAG_ENTRY("TRIANGULATE");
	MODEL_FLAG_ENTRY("REMOVE_COMPONENT");
	MODEL_FLAG_ENTRY("GEN_NORMALS");
	MODEL_FLAG_ENTRY("GEN_SMOOTH_NORMALS");
	MODEL_FLAG_ENTRY("SPLIT_LARGE_MESHES");
	MODEL_FLAG_ENTRY("PRE_TRANSFORM_VERTICES");
	MODEL_FLAG_ENTRY("LIMIT_BONE_WEIGHTS");
	MODEL_FLAG_ENTRY("VALIDATE_DATA_STRUCTURE");
	MODEL_FLAG_ENTRY("IMPROVE_CACHE_LOCALITY");
	MODEL_FLAG_ENTRY("REMOVE_REDUNDANT_MATERIALS");
	MODEL_FLAG_ENTRY("FIX_INFACING_NORMALS");
	MODEL_FLAG_ENTRY("SORT_BY_PTYPE");
	MODEL_FLAG_ENTRY("FIND_DEGENERATES");
	MODEL_FLAG_ENTRY("FIND_INVALID_DATA");
	MODEL_FLAG_ENTRY("GEN_UV_COORDS");
	MODEL_FLAG_ENTRY("TRANSFORM_UV_COORDS");
	MODEL_FLAG_ENTRY("FIND_INSTANCES");
	MODEL_FLAG_ENTRY("OPTIMIZE_MESHES");
	MODEL_FLAG_ENTRY("OPTIMIZE_GRAPH");
	MODEL_FLAG_ENTRY("FLIP_UVS");
	MODEL_FLAG_ENTRY("FLIP_WINDING_ORDER");
	MODEL_FLAG_ENTRY("SPLIT_BY_BONE_COUNT");
	MODEL_FLAG_ENTRY("DEBONE");
	MODEL_FLAG_ENTRY("GLOBAL_SCALE");
	MODEL_FLAG_ENTRY("EMBED_TEXTURES");
	MODEL_FLAG_ENTRY("FORCE_GEN_NORMALS");
	MODEL_FLAG_ENTRY("DROP_NORMALS");
	MODEL_FLAG_ENTRY("GEN_BOUNDING_BOXES");
}

void AmberEngine::UI::Panels::AssetProperties::CreateTextureSettings()
{
	m_metadata->Add("MIN_FILTER", static_cast<int>(Resources::ETextureFilteringMode::LINEAR_MIPMAP_LINEAR));
	m_metadata->Add("MAG_FILTER", static_cast<int>(Resources::ETextureFilteringMode::LINEAR));
	m_metadata->Add("ENABLE_MIPMAPPING", true);

	std::map<int, std::string> filteringModes
	{
		{0x2600, "NEAREST"},
		{0x2601, "LINEAR"},
		{0x2700, "NEAREST_MIPMAP_NEAREST"},
		{0x2703, "LINEAR_MIPMAP_LINEAR"},
		{0x2701, "LINEAR_MIPMAP_NEAREST"},
		{0x2702, "NEAREST_MIPMAP_LINEAR"}
	};

	GUIDrawer::CreateTitle(*m_settingsColumns, "MIN_FILTER");
	auto& minFilter = m_settingsColumns->CreateWidget<Widgets::ComboBox>(m_metadata->Get<int>("MIN_FILTER"));
	minFilter.choices = filteringModes;
	minFilter.ValueChangedEvent += [this](int p_choice)
	{
		m_metadata->Set("MIN_FILTER", p_choice);
	};

	GUIDrawer::CreateTitle(*m_settingsColumns, "MAG_FILTER");
	auto& magFilter = m_settingsColumns->CreateWidget<Widgets::ComboBox>(m_metadata->Get<int>("MAG_FILTER"));
	magFilter.choices = filteringModes;
	magFilter.ValueChangedEvent += [this](int p_choice)
	{
		m_metadata->Set("MAG_FILTER", p_choice);
	};

	GUIDrawer::DrawBoolean(*m_settingsColumns, "ENABLE_MIPMAPPING", [&]() { return m_metadata->Get<bool>("ENABLE_MIPMAPPING"); }, [&](bool value) { m_metadata->Set<bool>("ENABLE_MIPMAPPING", value); });
}

void AmberEngine::UI::Panels::AssetProperties::Apply()
{
	m_metadata->Rewrite();

	const auto resourcePath = EDITOR_EXEC(GetResourcePath(m_resource));
	const auto fileType = Tools::Utils::PathParser::GetFileType(m_resource);

	if (fileType == Tools::Utils::PathParser::EFileType::MODEL)
	{
		auto& modelManager = Tools::Global::ServiceLocator::Get< ResourceManagement::ModelManager>();
		if (modelManager.IsResourceRegistered(resourcePath))
		{
			modelManager.AResourceManager::ReloadResource(resourcePath);
		}
	}
	else if (fileType == Tools::Utils::PathParser::EFileType::TEXTURE)
	{
		auto& textureManager = Tools::Global::ServiceLocator::Get < ResourceManagement::TextureManager>();
		if (textureManager.IsResourceRegistered(resourcePath))
		{
			textureManager.AResourceManager::ReloadResource(resourcePath);
		}
	}

	Refresh();
}

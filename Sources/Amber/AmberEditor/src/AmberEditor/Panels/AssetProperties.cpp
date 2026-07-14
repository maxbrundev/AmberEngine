#include "Amberpch.h"

#include "AmberEditor/Panels/AssetProperties.h"

#include "AmberEditor/Core/EditorAction.h"
#include "AmberTools/Utils/SizeConverter.h"
#include "AmberCore/Helpers/GUIDrawer.h"
#include "AmberEditor/Panels/AssetView.h"
#include "AmberUI/Widgets/ComboBox.h"
#include "AmberUI/Widgets/GroupCollapsable.h"
#include "AmberUI/Widgets/NewLine.h"
#include "AmberUI/Widgets/Separator.h"

AmberEditor::Panels::AssetProperties::AssetProperties(const std::string& p_title, bool p_opened,
                                                          const AmberUI::Panels::PanelSettings& p_windowSettings) : AmberUI::Panels::APanelWindow(p_title, p_opened, p_windowSettings)
{
	m_targetChanged += [this]() { SetTarget(m_assetSelector->Content); };

	CreateHeaderButtons();

	m_headerSeparator = &CreateWidget<AmberUI::Widgets::Separator>();
	m_headerSeparator->Enabled = false;

	CreateAssetSelector();

	m_settings = &CreateWidget<AmberUI::Widgets::GroupCollapsable>("Settings");
	m_settingsColumns = &m_settings->CreateWidget<AmberUI::Widgets::Columns<2>>();
	m_settingsColumns->Widths[0] = 150;

	m_info = &CreateWidget<AmberUI::Widgets::GroupCollapsable>("Info");
	m_infoColumns = &m_info->CreateWidget<AmberUI::Widgets::Columns<2>>();
	m_infoColumns->Widths[0] = 150;

	m_settings->Enabled = m_info->Enabled = false;
}

void AmberEditor::Panels::AssetProperties::SetTarget(const std::string& p_path)
{
	m_resource = p_path == "" ? p_path : EDITOR_EXEC(GetResourcePath(p_path));

	if (m_assetSelector)
	{
		m_assetSelector->Content = m_resource;
	}

	Refresh();
}

void AmberEditor::Panels::AssetProperties::Refresh()
{
	m_metadata.reset(new AmberTools::FileSystem::IniFile(EDITOR_EXEC(GetRealPath(m_resource)) + ".meta"));

	CreateSettings();
	CreateInfo();

	m_applyButton->Enabled = m_settings->Enabled;
	m_resetButton->Enabled = m_settings->Enabled;
	m_revertButton->Enabled = m_settings->Enabled;

	switch (AmberTools::Utils::PathParser::GetFileType(m_resource))
	{
	case AmberTools::Utils::PathParser::EFileType::MODEL:
	case AmberTools::Utils::PathParser::EFileType::TEXTURE:
	case AmberTools::Utils::PathParser::EFileType::MATERIAL:
		m_previewButton->Enabled = true;
		break;
	default:
		m_previewButton->Enabled = false;
		break;
	}

	// Enables the header separator (And the line break) if at least one button is enabled
	m_headerSeparator->Enabled = m_applyButton->Enabled || m_resetButton->Enabled || m_revertButton->Enabled || m_previewButton->Enabled;
	m_headerLineBreak->Enabled = m_headerSeparator->Enabled;
}

void AmberEditor::Panels::AssetProperties::Preview()
{
	auto& assetView = EDITOR_PANEL(AssetView, "Asset View");

	const auto fileType = AmberTools::Utils::PathParser::GetFileType(m_resource);

	if (fileType == AmberTools::Utils::PathParser::EFileType::MODEL)
	{
		if (auto resource = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::ModelManager>().GetResource(m_resource))
		{
			assetView.SetResource(resource);
		}
	}
	else if (fileType == AmberTools::Utils::PathParser::EFileType::TEXTURE)
	{
		if (auto resource = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::TextureManager>().GetResource(m_resource))
		{
			assetView.SetResource(resource);
		}
	}

	assetView.Open();
}

void AmberEditor::Panels::AssetProperties::CreateHeaderButtons()
{
	m_applyButton = &CreateWidget<AmberUI::Widgets::Button>("Apply");
	m_applyButton->IdleBackgroundColor = { 0.0f, 0.5f, 0.0f };
	m_applyButton->Enabled = false;
	m_applyButton->LineBreak = false;
	m_applyButton->ClickedEvent += std::bind(&AssetProperties::Apply, this);

	m_revertButton = &CreateWidget<AmberUI::Widgets::Button>("Revert");
	m_revertButton->IdleBackgroundColor = { 0.7f, 0.5f, 0.0f };
	m_revertButton->Enabled = false;
	m_revertButton->LineBreak = false;
	m_revertButton->ClickedEvent += std::bind(&AssetProperties::SetTarget, this, m_resource);

	m_previewButton = &CreateWidget<AmberUI::Widgets::Button>("Preview");
	m_previewButton->IdleBackgroundColor = { 0.7f, 0.5f, 0.0f };
	m_previewButton->Enabled = false;
	m_previewButton->LineBreak = false;
	m_previewButton->ClickedEvent += std::bind(&AssetProperties::Preview, this);

	m_resetButton = &CreateWidget<AmberUI::Widgets::Button>("Reset to default");
	m_resetButton->IdleBackgroundColor = { 0.5f, 0.0f, 0.0f };
	m_resetButton->Enabled = false;
	m_resetButton->LineBreak = false;
	m_resetButton->ClickedEvent += [this]
	{
		m_metadata->RemoveAll();
		CreateSettings();
	};

	m_headerLineBreak = &CreateWidget<AmberUI::Widgets::NewLine>();
	m_headerLineBreak->Enabled = false;
}

void AmberEditor::Panels::AssetProperties::CreateAssetSelector()
{
	auto& columns = CreateWidget<AmberUI::Widgets::Columns<2>>();
	columns.Widths[0] = 150;
	m_assetSelector = &AmberCore::Helpers::GUIDrawer::DrawAsset(columns, "Target", m_resource, &m_targetChanged);
}

void AmberEditor::Panels::AssetProperties::CreateSettings()
{
	m_settingsColumns->RemoveAllWidgets();

	const auto fileType = AmberTools::Utils::PathParser::GetFileType(m_resource);

	m_settings->Enabled = true;

	if (fileType == AmberTools::Utils::PathParser::EFileType::MODEL)
	{
		CreateModelSettings();
	}
	else if (fileType == AmberTools::Utils::PathParser::EFileType::TEXTURE)
	{
		CreateTextureSettings();
	}
	else
	{
		m_settings->Enabled = false;
	}
}

void AmberEditor::Panels::AssetProperties::CreateInfo()
{
	const auto realPath = EDITOR_EXEC(GetRealPath(m_resource));

	m_infoColumns->RemoveAllWidgets();

	if (std::filesystem::exists(realPath))
	{
		m_info->Enabled = true;

		AmberCore::Helpers::GUIDrawer::CreateTitle(*m_infoColumns, "Path");
		m_infoColumns->CreateWidget<AmberUI::Widgets::Text>(realPath);

		AmberCore::Helpers::GUIDrawer::CreateTitle(*m_infoColumns, "Size");
		const auto[size, unit] = AmberTools::Utils::SizeConverter::ConvertToOptimalUnit(static_cast<float>(std::filesystem::file_size(realPath)), AmberTools::Utils::SizeConverter::ESizeUnit::BYTE);
		m_infoColumns->CreateWidget<AmberUI::Widgets::Text>(std::to_string(size) + " " + AmberTools::Utils::SizeConverter::UnitToString(unit));

		AmberCore::Helpers::GUIDrawer::CreateTitle(*m_infoColumns, "Metadata");
		m_infoColumns->CreateWidget<AmberUI::Widgets::Text>(std::filesystem::exists(realPath + ".meta") ? "Yes" : "No");
	}
	else
	{
		m_info->Enabled = false;
	}
}

#define MODEL_FLAG_ENTRY(setting) AmberCore::Helpers::GUIDrawer::DrawBoolean(*m_settingsColumns, setting, [&]() { return m_metadata->Get<bool>(setting); }, [&](bool value) { m_metadata->Set<bool>(setting, value); })


void AmberEditor::Panels::AssetProperties::CreateModelSettings()
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

void AmberEditor::Panels::AssetProperties::CreateTextureSettings()
{
	m_metadata->Add("MIN_FILTER", static_cast<int>(AmberRendering::Resources::ETextureFilteringMode::LINEAR_MIPMAP_LINEAR));
	m_metadata->Add("MAG_FILTER", static_cast<int>(AmberRendering::Resources::ETextureFilteringMode::LINEAR));
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

	AmberCore::Helpers::GUIDrawer::CreateTitle(*m_settingsColumns, "MIN_FILTER");
	auto& minFilter = m_settingsColumns->CreateWidget<AmberUI::Widgets::ComboBox>(m_metadata->Get<int>("MIN_FILTER"));
	minFilter.Choices = filteringModes;
	minFilter.ValueChangedEvent += [this](int p_choice)
	{
		m_metadata->Set("MIN_FILTER", p_choice);
	};

	AmberCore::Helpers::GUIDrawer::CreateTitle(*m_settingsColumns, "MAG_FILTER");
	auto& magFilter = m_settingsColumns->CreateWidget<AmberUI::Widgets::ComboBox>(m_metadata->Get<int>("MAG_FILTER"));
	magFilter.Choices = filteringModes;
	magFilter.ValueChangedEvent += [this](int p_choice)
	{
		m_metadata->Set("MAG_FILTER", p_choice);
	};

	AmberCore::Helpers::GUIDrawer::DrawBoolean(*m_settingsColumns, "ENABLE_MIPMAPPING", [&]() { return m_metadata->Get<bool>("ENABLE_MIPMAPPING"); }, [&](bool value) { m_metadata->Set<bool>("ENABLE_MIPMAPPING", value); });
}

void AmberEditor::Panels::AssetProperties::Apply()
{
	m_metadata->Rewrite();

	const auto resourcePath = EDITOR_EXEC(GetResourcePath(m_resource));
	const auto fileType = AmberTools::Utils::PathParser::GetFileType(m_resource);

	if (fileType == AmberTools::Utils::PathParser::EFileType::MODEL)
	{
		auto& modelManager = AmberTools::Global::ServiceLocator::Get< AmberCore::ResourceManagement::ModelManager>();
		if (modelManager.IsResourceRegistered(resourcePath))
		{
			modelManager.AResourceManager::ReloadResource(resourcePath);
		}
	}
	else if (fileType == AmberTools::Utils::PathParser::EFileType::TEXTURE)
	{
		auto& textureManager = AmberTools::Global::ServiceLocator::Get < AmberCore::ResourceManagement::TextureManager>();
		if (textureManager.IsResourceRegistered(resourcePath))
		{
			textureManager.AResourceManager::ReloadResource(resourcePath);
		}
	}

	Refresh();
}

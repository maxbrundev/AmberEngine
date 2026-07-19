#include "Amberpch.h"

#include "AmberEditor/Panels/MaterialEditor.h"

#include "AmberEditor/Core/EditorAction.h"
#include "AmberCore/Resources/Loaders/MaterialLoader.h"
#include "AmberTools/Global/ServiceLocator.h"
#include "AmberCore/Helpers/GUIDrawer.h"
#include "AmberEditor/Panels/AssetView.h"
#include "AmberUI/Widgets/Button.h"
#include "AmberUI/Widgets/ColorEdit.h"
#include "AmberUI/Widgets/DragMultipleScalars.h"
#include "AmberUI/Widgets/GroupCollapsable.h"
#include "AmberUI/Widgets/Separator.h"

AmberEditor::Panels::MaterialEditor::MaterialEditor(const std::string& p_title, bool p_opened, const AmberUI::Panels::PanelSettings& p_windowSettings) : AmberUI::Panels::APanelWindow(p_title, p_opened, p_windowSettings)
{
	CreateHeaderButtons();
	CreateWidget<AmberUI::Widgets::Separator>();
	CreateMaterialSelector();
	m_settings = &CreateWidget<AmberUI::Widgets::Group>();
	CreateShaderSelector();
	CreateMaterialSettings();
	CreateShaderSettings();

	m_settings->Enabled = false;
	m_shaderSettings->Enabled = false;

	m_materialDroppedEvent += std::bind(&MaterialEditor::OnMaterialDropped, this);
	m_shaderDroppedEvent += std::bind(&MaterialEditor::OnShaderDropped, this);
}

void AmberEditor::Panels::MaterialEditor::Refresh()
{
	if (m_target)
		SetTarget(*m_target);
}

void AmberEditor::Panels::MaterialEditor::SetTarget(AmberCore::Resources::Material& p_newTarget)
{
	m_target = &p_newTarget;
	m_targetMaterialText->Content = m_target->path;
	OnMaterialDropped();
}

AmberCore::Resources::Material* AmberEditor::Panels::MaterialEditor::GetTarget() const
{
	return m_target;
}

void AmberEditor::Panels::MaterialEditor::RemoveTarget()
{
	m_target = nullptr;
	m_targetMaterialText->Content = "Empty";
	OnMaterialDropped();
}

void AmberEditor::Panels::MaterialEditor::Preview()
{
	auto& assetView = EDITOR_PANEL(Panels::AssetView, "Asset View");
	
	if (m_target)
		assetView.SetResource(m_target);
	
	assetView.Open();
}

void AmberEditor::Panels::MaterialEditor::Reset()
{
	if (m_target && m_shader)
	{
		m_target->SetShader(nullptr);
		OnShaderDropped();
	}
}

void AmberEditor::Panels::MaterialEditor::OnMaterialDropped()
{
	m_settings->Enabled = m_target; // Enable m_settings group if the target material is non-null

	if (m_settings->Enabled)
	{
		GenerateMaterialSettingsContent();
		m_shaderText->Content = m_target->GetShader() ? m_target->GetShader()->Path : "Empty";
		m_shader = m_target->GetShader();
	}
	else
	{
		m_materialSettingsColumns->RemoveAllWidgets();
	}

	m_shaderSettings->Enabled = false;
	m_shaderSettingsColumns->RemoveAllWidgets();

	if (m_target && m_target->GetShader())
		OnShaderDropped();
}

void AmberEditor::Panels::MaterialEditor::OnShaderDropped()
{
	m_shaderSettings->Enabled = m_shader; // Enable m_shaderSettings group if the shader of the target material is non-null

	if (m_shader != m_target->GetShader())
		m_target->SetShader(m_shader);

	if (m_shaderSettings->Enabled)
	{
		GenerateShaderSettingsContent();
	}
	else
	{
		m_shaderSettingsColumns->RemoveAllWidgets();
	}
}

void AmberEditor::Panels::MaterialEditor::CreateHeaderButtons()
{
	auto& saveButton = CreateWidget<AmberUI::Widgets::Button>("Save to file");
	saveButton.IdleBackgroundColor = { 0.0f, 0.5f, 0.0f };
	saveButton.ClickedEvent += [this]
	{
		if (m_target)
			AmberCore::Resources::Loaders::MaterialLoader::Save(*m_target, EDITOR_EXEC(GetRealPath(m_target->path)));
	};

	saveButton.LineBreak = false;

	auto& reloadButton = CreateWidget<AmberUI::Widgets::Button>("Reload from file");
	reloadButton.IdleBackgroundColor = { 0.7f, 0.5f, 0.0f };
	reloadButton.ClickedEvent += [this]
	{
		if (m_target)
			AmberCore::Resources::Loaders::MaterialLoader::Reload(*m_target, EDITOR_EXEC(GetRealPath(m_target->path)));

		OnMaterialDropped();
	};

	reloadButton.LineBreak = false;

	auto& previewButton = CreateWidget<AmberUI::Widgets::Button>("Preview");
	previewButton.IdleBackgroundColor = { 0.7f, 0.5f, 0.0f };
	previewButton.ClickedEvent += std::bind(&MaterialEditor::Preview, this);
	previewButton.LineBreak = false;

	auto& resetButton = CreateWidget<AmberUI::Widgets::Button>("Reset to default");
	resetButton.IdleBackgroundColor = { 0.5f, 0.0f, 0.0f };
	resetButton.ClickedEvent += std::bind(&MaterialEditor::Reset, this);
}

void AmberEditor::Panels::MaterialEditor::CreateMaterialSelector()
{
	auto& columns = CreateWidget<AmberUI::Widgets::Columns<2>>();
	columns.Widths[0] = 150;
	m_targetMaterialText = &AmberCore::Helpers::GUIDrawer::DrawMaterial(columns, "Material", m_target, &m_materialDroppedEvent);
}

void AmberEditor::Panels::MaterialEditor::CreateShaderSelector()
{
	auto& columns = m_settings->CreateWidget<AmberUI::Widgets::Columns<2>>();
	columns.Widths[0] = 150;
	m_shaderText = &AmberCore::Helpers::GUIDrawer::DrawShader(columns, "Shader", m_shader, &m_shaderDroppedEvent);
}

void AmberEditor::Panels::MaterialEditor::CreateMaterialSettings()
{
	m_materialSettings = &m_settings->CreateWidget<AmberUI::Widgets::GroupCollapsable>("Material Settings");
	m_materialSettingsColumns = &m_materialSettings->CreateWidget<AmberUI::Widgets::Columns<2>>();
	m_materialSettingsColumns->Widths[0] = 150;
}

void AmberEditor::Panels::MaterialEditor::CreateShaderSettings()
{
	m_shaderSettings = &m_settings->CreateWidget<AmberUI::Widgets::GroupCollapsable>("Shader Settings");
	m_shaderSettingsColumns = &m_shaderSettings->CreateWidget<AmberUI::Widgets::Columns<2>>();
	m_shaderSettingsColumns->Widths[0] = 150;
}

std::string UniformFormat(const std::string& p_string)
{
	std::string result;
	std::string formattedInput = p_string;

	if (formattedInput.rfind("u_", 0) == 0 || formattedInput.rfind("U_", 0) == 0)
	{
		formattedInput = formattedInput.substr(2);
	}

	std::string capsChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	bool first = true;
	bool capsNext = false;

	for (char c : formattedInput)
	{
		if (first || capsNext)
		{
			c = toupper(c);
			first = false;
			capsNext = false;
		}

		if (c == '_')
		{
			c = ' ';
			capsNext = true;
		}

		if (std::find(capsChars.begin(), capsChars.end(), c) != capsChars.end())
			result.push_back(' ');

		result.push_back(c);
	}

	return result;
}

void DrawHybridVec3(AmberUI::WidgetContainer& p_root, const std::string& p_name, glm::vec3& p_data, float p_step, float p_min, float p_max)
{
	AmberCore::Helpers::GUIDrawer::CreateTitle(p_root, p_name);

	auto& rightSide = p_root.CreateWidget<AmberUI::Widgets::Group>();

	auto& xyzWidget = rightSide.CreateWidget<AmberUI::Widgets::DragMultipleScalars<float, 3>>(AmberCore::Helpers::GUIDrawer::GetDataType<float>(), p_min, p_max, 0.f, p_step, "", AmberCore::Helpers::GUIDrawer::GetFormat<float>());

	auto RegisterGatherer([&p_data]()
	{
		return reinterpret_cast<const std::array<float, 3>&>(p_data);
	});

	auto RegisterProvider([&p_data](std::array<float, 3> p_value)
	{
		p_data = reinterpret_cast<glm::vec3&>(p_value);
	});
	\
	xyzWidget.SetGathererAndProvider(RegisterGatherer, RegisterProvider);
	
	xyzWidget.LineBreak = false;

	auto& rgbWidget = rightSide.CreateWidget<AmberUI::Widgets::ColorEdit>(false, AmberRendering::Data::Color{ p_data.x, p_data.y, p_data.z });

	auto RegisterGathererColor([&p_data]()
	{
		return reinterpret_cast<const AmberRendering::Data::Color&>(p_data);
	});

	auto RegisterProviderColor([&p_data](AmberRendering::Data::Color p_value)
	{
		p_data = reinterpret_cast<glm::vec4&>(p_value);
	});

	rgbWidget.SetGathererAndProvider(RegisterGathererColor, RegisterProviderColor);

	rgbWidget.Enabled = false;
	rgbWidget.LineBreak = false;

	auto& xyzButton = rightSide.CreateWidget<AmberUI::Widgets::Button>("XYZ");
	xyzButton.IdleBackgroundColor = { 0.7f, 0.5f, 0.0f };
	xyzButton.LineBreak = false;

	auto& rgbButton = rightSide.CreateWidget<AmberUI::Widgets::Button>("RGB");
	rgbButton.IdleBackgroundColor = { 0.7f, 0.5f, 0.0f };

	xyzButton.ClickedEvent += [&]
	{
		xyzWidget.Enabled = true;
		rgbWidget.Enabled = false;
	};

	rgbButton.ClickedEvent += [&]
	{
		xyzWidget.Enabled = false;
		rgbWidget.Enabled = true;
	};
}

void DrawHybridVec4(AmberUI::WidgetContainer& p_root, const std::string& p_name, glm::vec4& p_data, float p_step, float p_min, float p_max)
{
	AmberCore::Helpers::GUIDrawer::CreateTitle(p_root, p_name);

	auto& rightSide = p_root.CreateWidget<AmberUI::Widgets::Group>();

	auto& xyzWidget = rightSide.CreateWidget<AmberUI::Widgets::DragMultipleScalars<float, 4>>(AmberCore::Helpers::GUIDrawer::GetDataType<float>(), p_min, p_max, 0.f, p_step, "", AmberCore::Helpers::GUIDrawer::GetFormat<float>());


	auto RegisterGatherer([&p_data]()
	{
		return reinterpret_cast<const std::array<float, 4>&>(p_data);
	});

	auto RegisterProvider([&p_data](std::array<float, 4> p_value)
	{
		p_data = reinterpret_cast<glm::vec4&>(p_value);
	});

	xyzWidget.SetGathererAndProvider(RegisterGatherer, RegisterProvider);

	xyzWidget.LineBreak = false;

	auto& rgbaWidget = rightSide.CreateWidget<AmberUI::Widgets::ColorEdit>(true, AmberRendering::Data::Color{ p_data.x, p_data.y, p_data.z, p_data.w });

	rgbaWidget.ColorChangedEvent += [ &p_data](AmberRendering::Data::Color& color)
	{
		auto& test = reinterpret_cast<AmberRendering::Data::Color&>(p_data);

		test = color;
	};

	auto RegisterGathererColor([&p_data]()
	{
		return reinterpret_cast<const AmberRendering::Data::Color&>(p_data);
	});

	auto RegisterProviderColor([&p_data](AmberRendering::Data::Color p_value)
	{
		p_data = reinterpret_cast<glm::vec4&>(p_value);
	});

	rgbaWidget.SetGathererAndProvider(RegisterGathererColor, RegisterProviderColor);
	rgbaWidget.Enabled = false;
	rgbaWidget.LineBreak = false;

	auto& xyzwButton = rightSide.CreateWidget<AmberUI::Widgets::Button>("XYZW");
	xyzwButton.IdleBackgroundColor = { 0.7f, 0.5f, 0.0f };
	xyzwButton.LineBreak = false;

	auto& rgbaButton = rightSide.CreateWidget<AmberUI::Widgets::Button>("RGBA");
	rgbaButton.IdleBackgroundColor = { 0.7f, 0.5f, 0.0f };

	xyzwButton.ClickedEvent += [&]
	{
		xyzWidget.Enabled = true;
		rgbaWidget.Enabled = false;
	};

	rgbaButton.ClickedEvent += [&]
	{
		xyzWidget.Enabled = false;
		rgbaWidget.Enabled = true;
	};
}

void AmberEditor::Panels::MaterialEditor::GenerateShaderSettingsContent()
{
	m_shaderSettingsColumns->RemoveAllWidgets(); // Ensure that the m_shaderSettingsColumns is empty

	std::multimap<int, std::pair<std::string, std::any*>> sortedUniformsData;

	for (auto&[name, value] : m_target->GetUniformsData())
	{
		int orderID = 0;

		auto uniformData = m_target->GetShader()->GetUniformInfo(name);

		if (uniformData)
		{
			switch (uniformData->type)
			{
			case AmberRendering::Resources::EUniformType::UNIFORM_SAMPLER_2D:	orderID = 0; break;
			case AmberRendering::Resources::EUniformType::UNIFORM_FLOAT_VEC4:	orderID = 1; break;
			case AmberRendering::Resources::EUniformType::UNIFORM_FLOAT_VEC3:	orderID = 2; break;
			case AmberRendering::Resources::EUniformType::UNIFORM_FLOAT_VEC2:	orderID = 3; break;
			case AmberRendering::Resources::EUniformType::UNIFORM_FLOAT:		orderID = 4; break;
			case AmberRendering::Resources::EUniformType::UNIFORM_INT:			orderID = 5; break;
			case AmberRendering::Resources::EUniformType::UNIFORM_BOOL:			orderID = 6; break;
			}

			sortedUniformsData.emplace(orderID, std::pair<std::string, std::any*>{ name, &value });
		}
	}

	for (auto&[order, info] : sortedUniformsData)
	{
		auto uniformData = m_target->GetShader()->GetUniformInfo(info.first);

		if (uniformData)
		{
			switch (uniformData->type)
			{
			case AmberRendering::Resources::EUniformType::UNIFORM_BOOL:			AmberCore::Helpers::GUIDrawer::DrawBoolean(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<bool&>(*info.second));																	break;
			case AmberRendering::Resources::EUniformType::UNIFORM_INT:			AmberCore::Helpers::GUIDrawer::DrawScalar<int>(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<int&>(*info.second));																break;
			case AmberRendering::Resources::EUniformType::UNIFORM_FLOAT:		AmberCore::Helpers::GUIDrawer::DrawScalar<float>(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<float&>(*info.second), 0.01f, AmberCore::Helpers::GUIDrawer::MIN_FLOAT, AmberCore::Helpers::GUIDrawer::MAX_FLOAT);		break;
			case AmberRendering::Resources::EUniformType::UNIFORM_FLOAT_VEC2:	AmberCore::Helpers::GUIDrawer::DrawVec2(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<glm::vec2&>(*info.second), 0.01f, AmberCore::Helpers::GUIDrawer::MIN_FLOAT, AmberCore::Helpers::GUIDrawer::MAX_FLOAT);	break;
			case AmberRendering::Resources::EUniformType::UNIFORM_FLOAT_VEC3:	DrawHybridVec3(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<glm::vec3&>(*info.second), 0.01f, AmberCore::Helpers::GUIDrawer::MIN_FLOAT, AmberCore::Helpers::GUIDrawer::MAX_FLOAT);			break;
			case AmberRendering::Resources::EUniformType::UNIFORM_FLOAT_VEC4:	DrawHybridVec4(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<glm ::vec4&>(*info.second), 0.01f, AmberCore::Helpers::GUIDrawer::MIN_FLOAT, AmberCore::Helpers::GUIDrawer::MAX_FLOAT);			break;
			case AmberRendering::Resources::EUniformType::UNIFORM_SAMPLER_2D:	AmberCore::Helpers::GUIDrawer::DrawTexture(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<AmberRendering::Resources::Texture * &>(*info.second));																break;
			}
		}
	}
}

void AmberEditor::Panels::MaterialEditor::GenerateMaterialSettingsContent()
{
	m_materialSettingsColumns->RemoveAllWidgets(); // Ensure that the m_shaderSettingsColumns is empty

	AmberCore::Helpers::GUIDrawer::DrawBoolean(*m_materialSettingsColumns, "Blendable", std::bind(&AmberCore::Resources::Material::IsBlendable, m_target), std::bind(&AmberCore::Resources::Material::SetBlendable, m_target, std::placeholders::_1));
	AmberCore::Helpers::GUIDrawer::DrawBoolean(*m_materialSettingsColumns, "Back-face Culling", std::bind(&AmberCore::Resources::Material::HasBackFaceCulling, m_target), std::bind(&AmberCore::Resources::Material::SetBackFaceCulling, m_target, std::placeholders::_1));
	AmberCore::Helpers::GUIDrawer::DrawBoolean(*m_materialSettingsColumns, "Front-face Culling", std::bind(&AmberCore::Resources::Material::HasFrontFaceCulling, m_target), std::bind(&AmberCore::Resources::Material::SetFrontFaceCulling, m_target, std::placeholders::_1));
	AmberCore::Helpers::GUIDrawer::DrawBoolean(*m_materialSettingsColumns, "Depth Test", std::bind(&AmberCore::Resources::Material::HasDepthTest, m_target), std::bind(&AmberCore::Resources::Material::SetDepthTest, m_target, std::placeholders::_1));
	AmberCore::Helpers::GUIDrawer::DrawBoolean(*m_materialSettingsColumns, "Depth Writing", std::bind(&AmberCore::Resources::Material::HasDepthWriting, m_target), std::bind(&AmberCore::Resources::Material::SetDepthWriting, m_target, std::placeholders::_1));
	AmberCore::Helpers::GUIDrawer::DrawBoolean(*m_materialSettingsColumns, "Color Writing", std::bind(&AmberCore::Resources::Material::HasColorWriting, m_target), std::bind(&AmberCore::Resources::Material::SetColorWriting, m_target, std::placeholders::_1));
	AmberCore::Helpers::GUIDrawer::DrawScalar<int>(*m_materialSettingsColumns, "GPU Instances", m_target->m_gpuInstances, 1.0f, 0, 100000);
}

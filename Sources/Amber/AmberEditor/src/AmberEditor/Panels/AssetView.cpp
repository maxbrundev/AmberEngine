#include "Amberpch.h"

#include "AmberEditor/Panels/AssetView.h"

#include "AmberEditor/Core/EditorAction.h"
#include "AmberUI/Widgets/Group.h"
#include "AmberTools/Utils/PathParser.h"
#include "AmberTools/Global/ServiceLocator.h"
#include "AmberCore/ResourceManagement/ModelManager.h"
#include "AmberCore/ResourceManagement/MaterialManager.h"
#include "AmberCore/ResourceManagement/TextureManager.h"

AmberEditor::Panels::AssetView::AssetView(const std::string& p_title, bool p_opened, const AmberUI::Panels::PanelSettings& p_windowSettings) : AView(p_title, p_opened, p_windowSettings), m_cameraController(m_camera, m_cameraPosition, m_cameraRotation)
{
	m_camera.SetClearColor({ 0.098f, 0.098f, 0.098f });
	m_camera.SetFar(5000.0f);

	m_resource = static_cast<AmberRendering::Resources::Model*>(nullptr);
	m_image->AddDragDropTarget("File").DataReceivedEvent += [this](auto p_data)
	{
		std::string path = p_data.first;

		switch (AmberTools::Utils::PathParser::GetFileType(path))
		{
		case AmberTools::Utils::PathParser::EFileType::MODEL:
			if (auto resource = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::ModelManager>().GetResource(path); resource)
				m_resource = resource;
			break;
		case AmberTools::Utils::PathParser::EFileType::TEXTURE:
			if (auto resource = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::TextureManager>().GetResource(path); resource)
				m_resource = resource;
			break;

		case AmberTools::Utils::PathParser::EFileType::MATERIAL:
			if (auto resource = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::MaterialManager>().GetResource(path); resource)
				m_resource = resource;
			break;
		}
	};
}

void AmberEditor::Panels::AssetView::Update(float p_deltaTime)
{
	AView::Update(p_deltaTime);
	if (IsHovered())
	{
		m_cameraController.Update(p_deltaTime);
	}
}

void AmberEditor::Panels::AssetView::RenderImplementation()
{
	PrepareCamera();

	auto& baseRenderer = *EDITOR_CONTEXT(renderer).get();

	m_frameBuffer.Bind();

	baseRenderer.GetDriver().SetStencilMask(0xFF);
	baseRenderer.Clear(m_camera);
	baseRenderer.GetDriver().SetStencilMask(0x00);

	uint8_t glState = baseRenderer.FetchGLState();
	baseRenderer.ApplyStateMask(glState);

	m_editorRenderer.RenderGrid(m_cameraPosition, m_gridColor);

	if (auto pval = std::get_if<AmberRendering::Resources::Model*>(&m_resource); pval && *pval)
		m_editorRenderer.RenderModelAsset(**pval);

	if (auto pval = std::get_if<AmberRendering::Resources::Texture*>(&m_resource); pval && *pval)
		m_editorRenderer.RenderTextureAsset(**pval);

	if (auto pval = std::get_if<AmberCore::Resources::Material*>(&m_resource); pval && *pval)
		m_editorRenderer.RenderMaterialAsset(**pval);

	baseRenderer.ApplyStateMask(glState);

	m_frameBuffer.Unbind();
}

void AmberEditor::Panels::AssetView::SetResource(ViewableResource p_resource)
{
	m_resource = p_resource;
}

AmberEditor::Panels::AssetView::ViewableResource AmberEditor::Panels::AssetView::GetResource() const
{
	return m_resource;
}

AmberEditor::Core::CameraController& AmberEditor::Panels::AssetView::GetCameraController()
{
	return m_cameraController;
}

#include "Amberpch.h"

#include "AmberEngine/UI/Panels/Views/AssetView.h"

#include "AmberEngine/Core/EditorAction.h"
#include "AmberEngine/UI/Widgets/Group.h"
#include "AmberEngine/Tools/Utils/PathParser.h"
#include "AmberEngine/Tools/Global/ServiceLocator.h"
#include "AmberEngine/Managers/ModelManager.h"
#include "AmberEngine/Managers/MaterialManager.h"
#include "AmberEngine/Managers/TextureManager.h"

AmberEngine::UI::Panels::AssetView::AssetView(const std::string& p_title, bool p_opened,
                                              const PanelSettings& p_windowSettings) : AView(p_title, p_opened, p_windowSettings), m_cameraController(m_camera, m_cameraPosition, m_cameraRotation)
{
	m_camera.SetClearColor({ 0.098f, 0.098f, 0.098f });
	m_camera.SetFar(5000.0f);

	m_resource = static_cast<Resources::Model*>(nullptr);
	m_image->AddPlugin<DDTarget<std::pair<std::string, Widgets::Group*>>>("File").DataReceivedEvent += [this](auto p_data)
	{
		std::string path = p_data.first;

		switch (Tools::Utils::PathParser::GetFileType(path))
		{
		case Tools::Utils::PathParser::EFileType::MODEL:
			if (auto resource = Tools::Global::ServiceLocator::Get<AmberEngine::ResourceManagement::ModelManager>().GetResource(path); resource)
				m_resource = resource;
			break;
		case Tools::Utils::PathParser::EFileType::TEXTURE:
			if (auto resource = Tools::Global::ServiceLocator::Get<ResourceManagement::TextureManager>().GetResource(path); resource)
				m_resource = resource;
			break;

		case Tools::Utils::PathParser::EFileType::MATERIAL:
			if (auto resource = Tools::Global::ServiceLocator::Get<ResourceManagement::MaterialManager>().GetResource(path); resource)
				m_resource = resource;
			break;
		}
	};
}

void AmberEngine::UI::Panels::AssetView::Update(float p_deltaTime)
{
	AView::Update(p_deltaTime);
	if (IsHovered())
	{
		m_cameraController.Update(p_deltaTime);
	}
}

void AmberEngine::UI::Panels::AssetView::RenderImplementation()
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

	if (auto pval = std::get_if<Resources::Model*>(&m_resource); pval && *pval)
		m_editorRenderer.RenderModelAsset(**pval);

	if (auto pval = std::get_if<Resources::Texture*>(&m_resource); pval && *pval)
		m_editorRenderer.RenderTextureAsset(**pval);

	if (auto pval = std::get_if<Resources::Material*>(&m_resource); pval && *pval)
		m_editorRenderer.RenderMaterialAsset(**pval);

	baseRenderer.ApplyStateMask(glState);

	m_frameBuffer.Unbind();
}

void AmberEngine::UI::Panels::AssetView::SetResource(ViewableResource p_resource)
{
	m_resource = p_resource;
}

AmberEngine::UI::Panels::AssetView::ViewableResource AmberEngine::UI::Panels::AssetView::GetResource() const
{
	return m_resource;
}

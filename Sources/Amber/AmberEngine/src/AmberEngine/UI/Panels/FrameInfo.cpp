#include "Amberpch.h"

#include "AmberEngine/UI/Panels/FrameInfo.h"

#include "AmberEngine/Core/Context.h"

#include "AmberEngine/Tools/Global/ServiceLocator.h"
#include "AmberEngine/Tools/Utils/NumberFormat.h"

AmberEngine::UI::Panels::FrameInfo::FrameInfo(const std::string& p_title, bool p_opened, const PanelSettings& p_windowSettings) :
APanelWindow(p_title, p_opened, p_windowSettings)
{
	m_batchCountText    = &CreateWidget<Widgets::TextColored>("");
	m_instanceCountText = &CreateWidget<Widgets::TextColored>("");
	m_polyCountText     = &CreateWidget<Widgets::TextColored>("");
	m_polyCountText->lineBreak = false;
	m_vertexCountText   = &CreateWidget<Widgets::TextColored>("");
}

void AmberEngine::UI::Panels::FrameInfo::Update(float p_deltaTime) const
{
	const auto& renderer = Tools::Global::ServiceLocator::Get<AmberEngine::Core::Context>().renderer;

	m_batchCountText->content    = "Batches: "   + Tools::Utils::Format::ToReadableNumber(renderer->GetFrameInfo().batchCount);
	m_instanceCountText->content = "Instances: " + Tools::Utils::Format::ToReadableNumber(renderer->GetFrameInfo().instanceCount);
	m_polyCountText->content     = "Polygons: "  + Tools::Utils::Format::ToReadableNumber(renderer->GetFrameInfo().polyCount);
	m_vertexCountText->content   = "Vertices: "  + Tools::Utils::Format::ToReadableNumber(renderer->GetFrameInfo().vertexCount);
}

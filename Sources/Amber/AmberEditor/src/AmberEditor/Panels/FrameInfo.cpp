#include "Amberpch.h"

#include "AmberEditor/Panels/FrameInfo.h"

#include "AmberEditor/Core/Context.h"

#include "AmberTools/Global/ServiceLocator.h"
#include "AmberTools/Utils/NumberFormat.h"

AmberEditor::Panels::FrameInfo::FrameInfo(const std::string& p_title, bool p_opened, const AmberUI::Panels::PanelSettings& p_windowSettings) :
AmberUI::Panels::APanelWindow(p_title, p_opened, p_windowSettings)
{
	m_batchCountText    = &CreateWidget<AmberUI::Widgets::TextColored>("");
	m_instanceCountText = &CreateWidget<AmberUI::Widgets::TextColored>("");
	m_polyCountText     = &CreateWidget<AmberUI::Widgets::TextColored>("");
	m_polyCountText->LineBreak = false;
	m_vertexCountText   = &CreateWidget<AmberUI::Widgets::TextColored>("");
}

void AmberEditor::Panels::FrameInfo::Update(float p_deltaTime) const
{
	const auto& renderer = AmberTools::Global::ServiceLocator::Get<AmberEditor::Core::Context>().renderer;

	m_batchCountText->Content    = "Batches: "   + AmberTools::Utils::Format::ToReadableNumber(renderer->GetFrameInfo().batchCount);
	m_instanceCountText->Content = "Instances: " + AmberTools::Utils::Format::ToReadableNumber(renderer->GetFrameInfo().instanceCount);
	m_polyCountText->Content     = "Polygons: "  + AmberTools::Utils::Format::ToReadableNumber(renderer->GetFrameInfo().polyCount);
	m_vertexCountText->Content   = "Vertices: "  + AmberTools::Utils::Format::ToReadableNumber(renderer->GetFrameInfo().vertexCount);
}

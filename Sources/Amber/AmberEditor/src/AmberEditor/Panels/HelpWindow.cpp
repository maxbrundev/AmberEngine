#include "Amberpch.h"

#include "AmberEditor/Panels/HelpWindow.h"

#include "AmberUI/Widgets/Column.h"
#include "AmberUI/Widgets/GroupCollapsable.h"
#include "AmberUI/Widgets/Text.h"

AmberEditor::Panels::HelpWindow::HelpWindow
(
	const std::string& p_title,
	bool p_opened,
	AmberUI::Panels::PanelSettings p_panelSettings
) : APanelWindow(p_title, p_opened, p_panelSettings)
{
	auto& cameraControls = CreateWidget<AmberUI::Widgets::GroupCollapsable>("Camera");

	auto& cameraColumns = cameraControls.CreateWidget<AmberUI::Widgets::Columns<2>>();
	cameraColumns.Widths[0] = 75;

	cameraColumns.CreateWidget<AmberUI::Widgets::Text>("Slide:");
	cameraColumns.CreateWidget<AmberUI::Widgets::Text>("[MB_MIDDLE]");

	cameraColumns.CreateWidget<AmberUI::Widgets::Text>("Rotate:");
	cameraColumns.CreateWidget<AmberUI::Widgets::Text>("[MB_RIGHT]");

	cameraColumns.CreateWidget<AmberUI::Widgets::Text>("Forward:");
	cameraColumns.CreateWidget<AmberUI::Widgets::Text>("[MB_RIGHT] + [W]");

	cameraColumns.CreateWidget<AmberUI::Widgets::Text>("Backward:");
	cameraColumns.CreateWidget<AmberUI::Widgets::Text>("[MB_RIGHT] + [S]");

	cameraColumns.CreateWidget<AmberUI::Widgets::Text>("Left:");
	cameraColumns.CreateWidget<AmberUI::Widgets::Text>("[MB_RIGHT] + [A]");

	cameraColumns.CreateWidget<AmberUI::Widgets::Text>("Right:");
	cameraColumns.CreateWidget<AmberUI::Widgets::Text>("[MB_RIGHT] + [D]");

	cameraColumns.CreateWidget<AmberUI::Widgets::Text>("Up:");
	cameraColumns.CreateWidget<AmberUI::Widgets::Text>("[MB_RIGHT] + [E]");

	cameraColumns.CreateWidget<AmberUI::Widgets::Text>("Down:");
	cameraColumns.CreateWidget<AmberUI::Widgets::Text>("[MB_RIGHT] + [Q]");

	cameraColumns.CreateWidget<AmberUI::Widgets::Text>("Focus:");
	cameraColumns.CreateWidget<AmberUI::Widgets::Text>("[F]");

	cameraColumns.CreateWidget<AmberUI::Widgets::Text>("Orbit:");
	cameraColumns.CreateWidget<AmberUI::Widgets::Text>("[ARROWS] / [PAGE_UP] / [PAGE_DOWN]");

	auto& gizmoControls = CreateWidget<AmberUI::Widgets::GroupCollapsable>("Gizmo");

	auto& gizmoColumns = gizmoControls.CreateWidget<AmberUI::Widgets::Columns<2>>();
	gizmoColumns.Widths[0] = 75;

	gizmoColumns.CreateWidget<AmberUI::Widgets::Text>("Translate:");
	gizmoColumns.CreateWidget<AmberUI::Widgets::Text>("[W]");

	gizmoColumns.CreateWidget<AmberUI::Widgets::Text>("Rotate:");
	gizmoColumns.CreateWidget<AmberUI::Widgets::Text>("[E]");

	gizmoColumns.CreateWidget<AmberUI::Widgets::Text>("Scale:");
	gizmoColumns.CreateWidget<AmberUI::Widgets::Text>("[R]");

	gizmoColumns.CreateWidget<AmberUI::Widgets::Text>("Snap:");
	gizmoColumns.CreateWidget<AmberUI::Widgets::Text>("[CTRL]");
}

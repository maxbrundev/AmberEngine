#include "Amberpch.h"

#include "AmberUI/Panels/APanelWindow.h"

#include "AmberUI/Tools/Converter.h"

AmberUI::Panels::APanelWindow::APanelWindow(const std::string& p_title, bool p_opened, const PanelSettings p_panelSettings) :
	Title(p_title),
	Settings(p_panelSettings), m_size(256, 144), Resizable(p_panelSettings.Resizable),
	Closable(p_panelSettings.Closable),
	Movable(p_panelSettings.Movable),
	Scrollable(p_panelSettings.Scrollable),
	Dockable(p_panelSettings.Dockable),
	HideBackground(p_panelSettings.HideBackground),
	ForceHorizontalScrollbar(p_panelSettings.ForceHorizontalScrollbar),
	ForceVerticalScrollbar(p_panelSettings.ForceVerticalScrollbar),
	AllowHorizontalScrollbar(p_panelSettings.AllowHorizontalScrollbar),
	BringToFrontOnFocus(p_panelSettings.BringToFrontOnFocus),
	Collapsable(p_panelSettings.Collapsable),
	AllowInputs(p_panelSettings.AllowInputs),
	m_opened(p_opened)
{
	AutoSize = p_panelSettings.AutoSize;
}

void AmberUI::Panels::APanelWindow::Update()
{
	if (!m_firstFrame)
	{
		if (!Settings.AutoSize)
			UpdateSize();
		CopyImGuiSize();

		UpdatePosition();
		CopyImGuiPosition();
	}

	m_firstFrame = false;
}

void AmberUI::Panels::APanelWindow::UpdateSize()
{
	if (m_sizeChanged)
	{
		ImGui::SetWindowSize(ImVec2(m_size.x, m_size.y), ImGuiCond_Always);
		m_sizeChanged = false;
	}
}

void AmberUI::Panels::APanelWindow::UpdatePosition()
{
	if (m_defaultPosition.x != -1.f && m_defaultPosition.y != 1.f)
	{
		glm::vec2 offsettedDefaultPos = m_defaultPosition + CalculatePositionAlignmentOffset(true);
		ImGui::SetWindowPos(AmberUI::Tools::Converter::ToImVec2(offsettedDefaultPos), m_ignoreConfigFile ? ImGuiCond_Once : ImGuiCond_FirstUseEver);
	}

	if (m_positionChanged || m_alignmentChanged)
	{
		glm::vec2 offset = CalculatePositionAlignmentOffset(false);
		glm::vec2 offsettedPos(m_position.x + offset.x, m_position.y + offset.y);
		ImGui::SetWindowPos(AmberUI::Tools::Converter::ToImVec2(offsettedPos), ImGuiCond_Always);
		m_positionChanged = false;
		m_alignmentChanged = false;
	}
}

void AmberUI::Panels::APanelWindow::CopyImGuiPosition()
{
	m_position = AmberUI::Tools::Converter::ToFVector2(ImGui::GetWindowPos());
}

void AmberUI::Panels::APanelWindow::Open()
{
	if (!m_opened)
	{
		m_opened = true;
		OpenEvent.Invoke();
	}
}

void AmberUI::Panels::APanelWindow::Close()
{
	if (m_opened)
	{
		m_opened = false;
		CloseEvent.Invoke();
	}
}

void AmberUI::Panels::APanelWindow::SetOpened(bool p_value)
{
	if (p_value != m_opened)
	{
		m_opened = p_value;

		if (m_opened)
			OpenEvent.Invoke();
		else
			CloseEvent.Invoke();
	}
}

void AmberUI::Panels::APanelWindow::Focus()
{
	//TODO:
	//ImGui::SetWindowFocus((Title + ).c_str());
}

bool AmberUI::Panels::APanelWindow::IsHovered() const
{
	return m_hovered;
}

bool AmberUI::Panels::APanelWindow::IsFocused() const
{
	return m_focused;
}

bool AmberUI::Panels::APanelWindow::IsOpened() const
{
	return m_opened;
}

glm::vec2 AmberUI::Panels::APanelWindow::CalculatePositionAlignmentOffset(bool p_default)
{
	glm::vec2 result(0.0f, 0.0f);

	switch (p_default ? m_defaultHorizontalAlignment : m_horizontalAlignment)
	{
	case EHorizontalAlignment::CENTER:
		result.x -= m_size.x / 2.0f;
		break;
	case EHorizontalAlignment::RIGHT:
		result.x -= m_size.x;
		break;
	}

	switch (p_default ? m_defaultVerticalAlignment : m_verticalAlignment)
	{
	case EVerticalAlignment::MIDDLE:
		result.y -= m_size.y / 2.0f;
		break;
	case EVerticalAlignment::BOTTOM:
		result.y -= m_size.y;
		break;
	}

	return result;
}

void AmberUI::Panels::APanelWindow::CopyImGuiSize()
{
	m_size = glm::vec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
}

std::pair<uint16_t, uint16_t> AmberUI::Panels::APanelWindow::GetSafeSize() const
{
	const auto result = GetSize() - glm::vec2(0.0f, ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2);
	return { static_cast<uint16_t>(result.x), static_cast<uint16_t>(result.y) };
}

void AmberUI::Panels::APanelWindow::SetPosition(const glm::vec2& p_position)
{
	m_position = p_position;
	m_positionChanged = true;
}

void AmberUI::Panels::APanelWindow::SetSize(const glm::vec2& p_size)
{
	m_size = p_size;
	m_sizeChanged = true;
}

const glm::vec2 AmberUI::Panels::APanelWindow::GetSize() const
{
	return m_size;
}

void AmberUI::Panels::APanelWindow::DrawImplementation()
{
	if (m_opened)
	{
		int windowFlags = ImGuiWindowFlags_None;

		if (!Resizable)					windowFlags |= ImGuiWindowFlags_NoResize;
		if (!Movable)					windowFlags |= ImGuiWindowFlags_NoMove;
		if (!Dockable)					windowFlags |= ImGuiWindowFlags_NoDocking;
		if (HideBackground)				windowFlags |= ImGuiWindowFlags_NoBackground;
		if (ForceHorizontalScrollbar)	windowFlags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;
		if (ForceVerticalScrollbar)		windowFlags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;
		if (AllowHorizontalScrollbar)	windowFlags |= ImGuiWindowFlags_HorizontalScrollbar;
		if (!BringToFrontOnFocus)		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		if (!Collapsable)				windowFlags |= ImGuiWindowFlags_NoCollapse;
		if (!AllowInputs)				windowFlags |= ImGuiWindowFlags_NoInputs;
		if (!Scrollable)                windowFlags |= ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar;
		if (!TitleBar)					windowFlags |= ImGuiWindowFlags_NoTitleBar;

		ImVec2 minSizeConstraint = AmberUI::Tools::Converter::ToImVec2(MinSize);
		ImVec2 maxSizeConstraint = AmberUI::Tools::Converter::ToImVec2(MaxSize);

		/* Cancel constraint if x or y is <= 0.f */
		if (minSizeConstraint.x <= 0.f || minSizeConstraint.y <= 0.f)
			minSizeConstraint = { 0.0f, 0.0f };

		if (maxSizeConstraint.x <= 0.f || maxSizeConstraint.y <= 0.f)
			maxSizeConstraint = { 10000.f, 10000.f };

		ImGui::SetNextWindowSizeConstraints(minSizeConstraint, maxSizeConstraint);

		if (ImGui::Begin(Title.c_str(), Settings.Closable ? &m_opened : nullptr, windowFlags))
		{
			m_hovered = ImGui::IsWindowHovered();
			m_focused = ImGui::IsWindowFocused();

			auto scrollY = ImGui::GetScrollY();

			m_scrolledToBottom = scrollY == ImGui::GetScrollMaxY();
			m_scrolledToTop = scrollY == 0.0f;

			if (!m_opened)
				CloseEvent.Invoke();

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			Update();

			if (m_mustScrollToBottom)
			{
				ImGui::SetScrollY(ImGui::GetScrollMaxY());
				m_mustScrollToBottom = false;
			}

			if (m_mustScrollToTop)
			{
				ImGui::SetScrollY(0.0f);
				m_mustScrollToTop = false;
			}

			DrawWidgets();
			DrawContextualMenu();
		}
		ImGui::End();
	}
}

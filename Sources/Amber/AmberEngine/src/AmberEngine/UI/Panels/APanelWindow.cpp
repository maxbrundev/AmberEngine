#include "Amberpch.h"

#include "AmberEngine/UI/Panels/APanelWindow.h"

#include "AmberEngine/Data/EditorConstants.h"
#include "AmberEngine/Tools/Utils/Converter.h"

AmberEngine::UI::Panels::APanelWindow::APanelWindow(const std::string& p_title, bool p_opened, const PanelSettings p_panelSettings) :
	title(p_title),
	settings(p_panelSettings), m_size(256, 144), resizable(p_panelSettings.resizable),
	closable(p_panelSettings.closable),
	movable(p_panelSettings.movable),
	scrollable(p_panelSettings.scrollable),
	dockable(p_panelSettings.dockable),
	hideBackground(p_panelSettings.hideBackground),
	forceHorizontalScrollbar(p_panelSettings.forceHorizontalScrollbar),
	forceVerticalScrollbar(p_panelSettings.forceVerticalScrollbar),
	allowHorizontalScrollbar(p_panelSettings.allowHorizontalScrollbar),
	bringToFrontOnFocus(p_panelSettings.bringToFrontOnFocus),
	collapsable(p_panelSettings.collapsable),
	allowInputs(p_panelSettings.allowInputs),
	m_opened(p_opened)
{
	autoSize = p_panelSettings.autoSize;
}

void AmberEngine::UI::Panels::APanelWindow::Update()
{
	if (!m_firstFrame)
	{
		if (!settings.autoSize)
			UpdateSize();
		CopyImGuiSize();

		UpdatePosition();
		CopyImGuiPosition();
	}

	m_firstFrame = false;
}

void AmberEngine::UI::Panels::APanelWindow::UpdateSize()
{
	if (m_sizeChanged)
	{
		ImGui::SetWindowSize(ImVec2(m_size.x, m_size.y), ImGuiCond_Always);
		m_sizeChanged = false;
	}
}

void AmberEngine::UI::Panels::APanelWindow::UpdatePosition()
{
	if (m_defaultPosition.x != -1.f && m_defaultPosition.y != 1.f)
	{
		glm::vec2 offsettedDefaultPos = m_defaultPosition + CalculatePositionAlignmentOffset(true);
		ImGui::SetWindowPos(Utils::Converter::ToImVec2(offsettedDefaultPos), m_ignoreConfigFile ? ImGuiCond_Once : ImGuiCond_FirstUseEver);
	}

	if (m_positionChanged || m_alignmentChanged)
	{
		glm::vec2 offset = CalculatePositionAlignmentOffset(false);
		glm::vec2 offsettedPos(m_position.x + offset.x, m_position.y + offset.y);
		ImGui::SetWindowPos(Utils::Converter::ToImVec2(offsettedPos), ImGuiCond_Always);
		m_positionChanged = false;
		m_alignmentChanged = false;
	}
}

void AmberEngine::UI::Panels::APanelWindow::CopyImGuiPosition()
{
	m_position = Utils::Converter::ToFVector2(ImGui::GetWindowPos());
}

void AmberEngine::UI::Panels::APanelWindow::Open()
{
	if (!m_opened)
	{
		m_opened = true;
		OpenEvent.Invoke();
	}
}

void AmberEngine::UI::Panels::APanelWindow::Close()
{
	if (m_opened)
	{
		m_opened = false;
		CloseEvent.Invoke();
	}
}

void AmberEngine::UI::Panels::APanelWindow::SetOpened(bool p_value)
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

void AmberEngine::UI::Panels::APanelWindow::Focus()
{
	//TODO:
	//ImGui::SetWindowFocus((title + ).c_str());
}

bool AmberEngine::UI::Panels::APanelWindow::IsHovered() const
{
	return m_hovered;
}

bool AmberEngine::UI::Panels::APanelWindow::IsFocused() const
{
	return m_focused;
}

bool AmberEngine::UI::Panels::APanelWindow::IsOpened() const
{
	return m_opened;
}

glm::vec2 AmberEngine::UI::Panels::APanelWindow::CalculatePositionAlignmentOffset(bool p_default)
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

void AmberEngine::UI::Panels::APanelWindow::CopyImGuiSize()
{
	m_size = glm::vec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
}

std::pair<uint16_t, uint16_t> AmberEngine::UI::Panels::APanelWindow::GetSafeSize() const
{
	const auto result = GetSize() - glm::vec2(0.0f, Data::EditorConstants::EDITOR_FONT_SIZE_MEDIUM + ImGui::GetStyle().FramePadding.y * 2);
	return { static_cast<uint16_t>(result.x), static_cast<uint16_t>(result.y) };
}

void AmberEngine::UI::Panels::APanelWindow::SetPosition(const glm::vec2& p_position)
{
	m_position = p_position;
	m_positionChanged = true;
}

void AmberEngine::UI::Panels::APanelWindow::SetSize(const glm::vec2& p_size)
{
	m_size = p_size;
	m_sizeChanged = true;
}

const glm::vec2 AmberEngine::UI::Panels::APanelWindow::GetSize() const
{
	return m_size;
}

void AmberEngine::UI::Panels::APanelWindow::DrawImplementation()
{
	if (m_opened)
	{
		int windowFlags = ImGuiWindowFlags_None;

		if (!resizable)					windowFlags |= ImGuiWindowFlags_NoResize;
		if (!movable)					windowFlags |= ImGuiWindowFlags_NoMove;
		if (!dockable)					windowFlags |= ImGuiWindowFlags_NoDocking;
		if (hideBackground)				windowFlags |= ImGuiWindowFlags_NoBackground;
		if (forceHorizontalScrollbar)	windowFlags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;
		if (forceVerticalScrollbar)		windowFlags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;
		if (allowHorizontalScrollbar)	windowFlags |= ImGuiWindowFlags_HorizontalScrollbar;
		if (!bringToFrontOnFocus)		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		if (!collapsable)				windowFlags |= ImGuiWindowFlags_NoCollapse;
		if (!allowInputs)				windowFlags |= ImGuiWindowFlags_NoInputs;
		if (!scrollable)                windowFlags |= ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar;
		if (!titleBar)					windowFlags |= ImGuiWindowFlags_NoTitleBar;

		ImVec2 minSizeConstraint = Utils::Converter::ToImVec2(minSize);
		ImVec2 maxSizeConstraint = Utils::Converter::ToImVec2(maxSize);

		/* Cancel constraint if x or y is <= 0.f */
		if (minSizeConstraint.x <= 0.f || minSizeConstraint.y <= 0.f)
			minSizeConstraint = { 0.0f, 0.0f };

		if (maxSizeConstraint.x <= 0.f || maxSizeConstraint.y <= 0.f)
			maxSizeConstraint = { 10000.f, 10000.f };

		ImGui::SetNextWindowSizeConstraints(minSizeConstraint, maxSizeConstraint);

		if (ImGui::Begin(title.c_str(), settings.closable ? &m_opened : nullptr, windowFlags))
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

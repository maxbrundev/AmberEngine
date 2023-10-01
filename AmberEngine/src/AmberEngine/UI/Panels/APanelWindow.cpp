#include "Amberpch.h"

#include "AmberEngine/UI/Panels/APanelWindow.h"

#include "AmberEngine/Data/Constants.h"
#include "AmberEngine/Tools/Utils/Converter.h"
#include "AmberEngine/UI/Panels/PanelSettings.h"

AmberEngine::UI::APanelWindow::APanelWindow(const std::string& p_title, bool p_opened, const PanelSettings p_panelSettings) :
	title(p_title),
	settings(p_panelSettings), m_opened(true), m_size(256, 144)
{
}

void AmberEngine::UI::APanelWindow::Update()
{
	if (!m_firstFrame)
	{
		if (!settings.autoSize)
			UpdateSize();
		CopyImGuiSize();

		//UpdatePosition();
		//CopyImGuiPosition();
	}

	m_firstFrame = false;
}

void AmberEngine::UI::APanelWindow::UpdateSize()
{
	if (m_sizeChanged)
	{
		ImGui::SetWindowSize(ImVec2(m_size.x, m_size.y), ImGuiCond_Always);
		m_sizeChanged = false;
	}
}

void AmberEngine::UI::APanelWindow::UpdatePosition()
{
}

void AmberEngine::UI::APanelWindow::CopyImGuiPosition()
{
}

void AmberEngine::UI::APanelWindow::SetOpened(bool p_value)
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

bool AmberEngine::UI::APanelWindow::IsHovered() const
{
	return m_hovered;
}

bool AmberEngine::UI::APanelWindow::IsFocused() const
{
	return m_focused;
}

bool AmberEngine::UI::APanelWindow::IsOpened() const
{
	return m_opened;
}

void AmberEngine::UI::APanelWindow::CopyImGuiSize()
{
	m_size = glm::vec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
}

std::pair<uint16_t, uint16_t> AmberEngine::UI::APanelWindow::GetSafeSize() const
{
	const auto result = GetSize() - glm::vec2(0.0f, Data::Constants::EDITOR_FONT_SIZE_MEDIUM + ImGui::GetStyle().FramePadding.y * 2);
	return { static_cast<uint16_t>(result.x), static_cast<uint16_t>(result.y) };
}

void AmberEngine::UI::APanelWindow::SetSize(const glm::vec2& p_size)
{
	m_size = p_size;
	m_sizeChanged = true;
}

const glm::vec2 AmberEngine::UI::APanelWindow::GetSize() const
{
	return m_size;
}

void AmberEngine::UI::APanelWindow::DrawImplementation()
{
	if (m_opened)
	{
		int windowFlags = ImGuiWindowFlags_None;

		if (!settings.resizable)					windowFlags |= ImGuiWindowFlags_NoResize;
		if (!settings.movable)					windowFlags |= ImGuiWindowFlags_NoMove;
		if (!settings.dockable)					windowFlags |= ImGuiWindowFlags_NoDocking;
		if (settings.hideBackground)				windowFlags |= ImGuiWindowFlags_NoBackground;
		if (settings.forceHorizontalScrollbar)	windowFlags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;
		if (settings.forceVerticalScrollbar)		windowFlags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;
		if (settings.allowHorizontalScrollbar)	windowFlags |= ImGuiWindowFlags_HorizontalScrollbar;
		if (!settings.bringToFrontOnFocus)		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		if (!settings.collapsable)				windowFlags |= ImGuiWindowFlags_NoCollapse;
		if (!settings.allowInputs)				windowFlags |= ImGuiWindowFlags_NoInputs;
		if (!settings.scrollable)                windowFlags |= ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar;
		if (!settings.titleBar)					windowFlags |= ImGuiWindowFlags_NoTitleBar;

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

			DrawContent();
		}
		ImGui::End();
	}
}

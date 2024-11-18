#pragma once

#include "AmberEngine/UI/Panels/APanel.h"
#include "AmberEngine/UI/Panels/PanelSettings.h"
#include "AmberEngine/UI/Settings/EAlignment.h"

#include "AmberEngine/Eventing/Event.h"

namespace AmberEngine::UI::Panels
{
	class APanelWindow : public APanel
	{
	public:
		APanelWindow(const std::string& p_title = "", bool p_opened = true, const PanelSettings p_panelSettings = PanelSettings{});
		virtual ~APanelWindow() override = default;

		void Update();
		void UpdateSize();
		void CopyImGuiSize();
		void UpdatePosition();
		void CopyImGuiPosition();
		void Open();
		void Close();
		void SetOpened(bool p_value);
		void Focus();
		bool IsHovered() const;
		bool IsFocused() const;

		bool IsOpened() const;
		glm::vec2 CalculatePositionAlignmentOffset(bool p_default = false);
		std::pair<uint16_t, uint16_t> GetSafeSize() const;
		void SetPosition(const glm::vec2& p_position);
		void SetSize(const glm::vec2& p_size);
		const glm::vec2 GetSize() const;

	protected:
		void DrawImplementation() override;

	public:
		std::string title;

		glm::vec2 minSize = { 0.f, 0.f };
		glm::vec2 maxSize = { 0.f, 0.f };

		PanelSettings settings;

		Eventing::Event<> OpenEvent;
		Eventing::Event<> CloseEvent;

	protected:
		glm::vec2 m_defaultPosition;
		glm::vec2 m_defaultSize;
		
		bool m_ignoreConfigFile;

		glm::vec2 m_position = glm::vec2(0.0f, 0.0f);
		glm::vec2 m_size = glm::vec2(0.0f, 0.0f);

		bool m_positionChanged = false;
		bool m_sizeChanged = false;
		EHorizontalAlignment m_defaultHorizontalAlignment;
		EVerticalAlignment m_defaultVerticalAlignment;
		EHorizontalAlignment m_horizontalAlignment = EHorizontalAlignment::LEFT;
		EVerticalAlignment m_verticalAlignment = EVerticalAlignment::TOP;
		bool m_alignmentChanged = false;
		bool m_firstFrame = true;

		glm::vec2 m_viewportSize;

		bool resizable = true;
		bool closable = false;
		bool movable = true;
		bool scrollable = true;
		bool dockable = false;
		bool hideBackground = false;
		bool forceHorizontalScrollbar = false;
		bool forceVerticalScrollbar = false;
		bool allowHorizontalScrollbar = false;
		bool bringToFrontOnFocus = true;
		bool collapsable = false;
		bool allowInputs = true;
		bool titleBar = true;
		bool autoSize = true;
	private:
		bool m_opened;
		bool m_hovered;
		bool m_focused;
		bool m_mustScrollToBottom = false;
		bool m_mustScrollToTop = false;
		bool m_scrolledToBottom = false;
		bool m_scrolledToTop = false;
	};
}

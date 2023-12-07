#pragma once

#include "AmberEngine/UI/Panels/APanel.h"
#include "AmberEngine/UI/Panels/PanelSettings.h"

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

		void SetOpened(bool p_value);

		bool IsHovered() const;
		bool IsFocused() const;

		bool IsOpened() const;

		std::pair<uint16_t, uint16_t> GetSafeSize() const;

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
		//Settings::EHorizontalAlignment m_defaultHorizontalAlignment;
		//Settings::EVerticalAlignment m_defaultVerticalAlignment;
		bool m_ignoreConfigFile;

		glm::vec2 m_position = glm::vec2(0.0f, 0.0f);
		glm::vec2 m_size = glm::vec2(0.0f, 0.0f);

		bool m_positionChanged = false;
		bool m_sizeChanged = false;

		//Settings::EHorizontalAlignment m_horizontalAlignment = Settings::EHorizontalAlignment::LEFT;
		//Settings::EVerticalAlignment m_verticalAlignment = Settings::EVerticalAlignment::TOP;

		bool m_alignmentChanged = false;
		bool m_firstFrame = true;

		glm::vec2 m_viewportSize;

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

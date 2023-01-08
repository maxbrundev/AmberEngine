#pragma once

namespace AmberEngine::UI
{
	class APanel
	{
	public:
		APanel(const std::string& p_name = "", bool p_opened = true);
		virtual ~APanel() = default;

		virtual void Update(float p_deltaTime) = 0;

		void CopyImGuiSize();

		std::pair<uint16_t, uint16_t> GetSafeSize() const;

		void SetSize(const glm::vec2& p_size);
		const glm::vec2 GetSize() const;

	protected:
		virtual void DrawImplementation();

		virtual void DrawContentImplementation() = 0;

	public:
		bool isOpen;
		bool isSizeChanged;
		bool isFirstFrame = true;
		bool isFocused = false;
		bool isHovered = false;

		std::string name;

		glm::vec2 viewportSize;
		glm::vec2 size;
	};
}
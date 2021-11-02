#pragma once

namespace AmberEngine::Views
{
	class AView
	{
	public:
		AView(std::string p_viewName);
		virtual ~AView() = default;

		void CopyImGuiSize();
		void UpdateSize();
		std::pair<uint16_t, uint16_t> GetSafeSize() const;
		
		const glm::vec2 GetSize() const;
		void SetSize(const glm::vec2& p_size);

	protected:
		glm::vec2 m_size;
		std::string m_name;
		bool m_sizeChanged;
	};
}
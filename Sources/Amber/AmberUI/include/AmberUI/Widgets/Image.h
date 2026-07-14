#pragma once

#include "AmberUI/Widgets/AWidget.h"

namespace AmberUI::Widgets
{
	union TextureID
	{
		uint32_t id;
		void* raw;
	};

	class Image : public AWidget
	{
	public:
		Image(uint32_t p_textureID, const glm::vec2& p_size);
		virtual ~Image() override = default;

	protected:
		void DrawImplementation() override;

	public:
		TextureID TextureID;
		glm::vec2 Size;
	};
}

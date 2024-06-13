#pragma once

#include "AmberEngine/UI/Widgets/AWidget.h"

namespace AmberEngine::UI::Widgets
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
		TextureID textureID;
		glm::vec2 size;
	};
}

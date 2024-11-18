#pragma once
#include "AButton.h"
#include "Image.h"
#include "AmberEngine/Data/Color.h"

namespace AmberEngine::UI::Widgets
{
	class ButtonImage : public AButton
	{
	public:
		/**
		* Constructor
		* @param p_textureID
		* @param p_size
		*/
		ButtonImage(uint32_t p_textureID, const glm::vec2& p_size);

	protected:
		void DrawImplementation() override;

	public:
		bool disabled = false;

		Data::Color background = { 0, 0, 0, 0 };
		Data::Color tint = { 1, 1, 1, 1 };

		TextureID textureID;
		glm::vec2 size;
	};
}

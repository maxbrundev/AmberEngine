#pragma once
#include "AButton.h"
#include "Image.h"
#include "AmberRendering/Data/Color.h"

namespace AmberUI::Widgets
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
		bool Disabled = false;

		AmberRendering::Data::Color Background = { 0, 0, 0, 0 };
		AmberRendering::Data::Color Tint = { 1, 1, 1, 1 };

		TextureID TextureID;
		glm::vec2 Size;
	};
}

#pragma once

#include <string>

/* Prevent enum and class name to be replaced by standard macros */
#undef MessageBox
#undef ERROR
#undef IGNORE

namespace AmberEngine::Windowing::Dialogs
{
	class MessageBox
	{
	public:
		enum class EMessageType
		{
			QUESTION    = 0x00000020L,
			INFORMATION = 0x00000040L,
			WARNING     = 0x00000030L,
			ERROR       = 0x00000010L
		};

		enum class EButtonLayout
		{
			OK                       = 0x00000000L,
			OK_CANCEL                = 0x00000001L,
			YES_NO                   = 0x00000004L,
			YES_NO_CANCEL            = 0x00000003L,
			RETRY_CANCEL             = 0x00000005L,
			ABORT_RETRY_IGNORE       = 0x00000002L,
			CANCEL_TRYAGAIN_CONTINUE = 0x00000006L,
			HELP                     = 0x00004000L
		};

		enum class EUserAction
		{
			OK       = 1,
			CANCEL   = 2,
			YES      = 6,
			NO       = 7,
			CONTINUE = 11,
			IGNORE   = 5,
			RETRY    = 4,
			TRYAGAIN = 10
		};

		MessageBox(std::string p_title, std::string p_message, EMessageType p_messageType = EMessageType::INFORMATION, EButtonLayout p_buttonLayout = EButtonLayout::OK, bool p_autoSpawn = true);

		void Spawn();

		const EUserAction& GetUserAction() const;

	private:
		std::string m_title;
		std::string m_message;

		EButtonLayout m_buttonLayout;
		EMessageType m_messageType;
		EUserAction m_userResult;
	};
}
#pragma once

namespace AmberEngine::Maths
{
	class TransformNotifier
	{
	public:
		enum class ENotification
		{
			TRANSFORM_CHANGED,
			TRANSFORM_DESTROYED
		};

		using NotificationHandler = std::function<void(ENotification)>;

		uint64_t AddNotificationHandler(NotificationHandler p_notificationHandler);

		void NotifyChildren(ENotification p_notification);

		bool RemoveNotificationHandler(const uint64_t& p_notificationHandlerID);

	private:
		std::unordered_map<uint64_t, NotificationHandler> m_notificationHandlers;
		uint64_t m_availableHandlerID = 0;
	};
}

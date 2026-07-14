#include "Amberpch.h"

#include "AmberMaths/TransformNotifier.h"

uint64_t AmberMaths::TransformNotifier::AddNotificationHandler(NotificationHandler p_notificationHandler)
{
	uint64_t handlerID = m_availableHandlerID++;
	m_notificationHandlers.emplace(handlerID, p_notificationHandler);

	return handlerID;
}

void AmberMaths::TransformNotifier::NotifyChildren(ENotification p_notification)
{
	if (!m_notificationHandlers.empty())
	{
		for (auto const&[id, handler] : m_notificationHandlers)
		{
			handler(p_notification);
		}
	}
}

bool AmberMaths::TransformNotifier::RemoveNotificationHandler(const uint64_t& p_notificationHandlerID)
{
	return m_notificationHandlers.erase(p_notificationHandlerID) != 0;
}
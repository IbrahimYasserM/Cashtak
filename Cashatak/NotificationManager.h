#pragma once
#include <queue>
#include <Notification.h>
class NotificationManager
{
	std::queue<Notification> notifications;
public:
	NotificationManager();
	~NotificationManager();
	void addNotification(const Notification& notification);
	void processNotifications();
};


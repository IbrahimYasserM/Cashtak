#include "stdafx.h"
#include "NotificationManager.h"
NotificationManager::NotificationManager() = default;
NotificationManager::~NotificationManager() = default;
void NotificationManager::addNotification(const Notification& notification) {
	notifications.push(notification);
}
void NotificationManager::processNotifications() {
	while (!notifications.empty()) {
		Notification notification = notifications.front();
		notifications.pop();
		// Process the notification (e.g., display it to the user)
	}
}
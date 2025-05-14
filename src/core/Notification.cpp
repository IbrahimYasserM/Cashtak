#include "stdafx.h"
#include "Notification.h"
Notification::Notification() = default;
Notification::Notification(const Transaction transaction) {
	title = "You received " + std::to_string(transaction.getAmount()) + "$";
	message = transaction.getSender() + " sent you " + std::to_string(transaction.getAmount()) + "$";
	date = transaction.getDate();
};
Notification::Notification(const PaymentRequest request) {
	title = "You received a request";
//	message = request.getSender() + " requested " + std::to_string(request.getAmount()) + "$";
//	date = request.getDate();
}
Notification::~Notification() = default;
void Notification::display() {
	// Display the notification (e.g., using a message box or a custom UI element)
}
#include "stdafx.h"
#include "Notification.h"
Notification::Notification(Transaction* transaction) {
	title = "You received " + std::to_string(transaction->getAmount()) + "$";
	message = transaction->getSender() + " sent you " + std::to_string(transaction->getAmount()) + "$";
};
Notification::Notification(PaymentRequest* request) {
	title = "You received a new request";
	message = request->getSender() + " requested " + std::to_string(request->getAmount()) + "$ from you";
}
Notification::~Notification() = default;
std::string Notification::getTitle() {
	return title;
}
std::string Notification::getMessage() {
	return message;
}
#pragma once
#include "Transaction.h"
#include "PaymentRequest.h"

class Notification
{
	std::string title, message;
	std::time_t date;
public:
	Notification();
	Notification(const Transaction);
	Notification(const PaymentRequest);
	~Notification();
	void display();
};


#pragma once
#include "Transaction.h"
#include "Request.h"
class Notification
{
	std::string title, message;
	std::time_t date;
public:
	Notification();
	Notification(const Transaction);
	Notification(const Request);
	~Notification();
	void display();
};


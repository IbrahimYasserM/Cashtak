#pragma once
#include "Transaction.h"
#include "PaymentRequest.h"

class Notification
{
	std::string title, message;
public:
	Notification(Transaction*);
	Notification(PaymentRequest*);
	~Notification();
	
	std::string getTitle();
	std::string getMessage();
};


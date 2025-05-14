#include "stdafx.h"
#include "PaymentRequest.h"

PaymentRequest::PaymentRequest(int id, double amount, const std::string& sender,
                const std::string& recipient, const std::string& status,
                const std::string& message)
    : Event(id, amount, sender, recipient, status), message(message)
{
}

std::string PaymentRequest::getMessage() const {
    return message;
}

void PaymentRequest::setMessage(const std::string& newMessage) {
    message = newMessage;
}

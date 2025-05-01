#include "stdafx.h"
#include "PaymentRequest.h"

Request::Request(int id, double amount, const std::string& sender,
                const std::string& recipient, const std::string& status,
                const std::string& message)
    : Event(id, amount, sender, recipient, status), message(message)
{
}

std::string Request::getMessage() const {
    return message;
}

void Request::setMessage(const std::string& newMessage) {
    message = newMessage;
}

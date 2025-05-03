#include "stdafx.h"
#include "Event.h"

Event::Event(int id, double amount, const std::string& sender, 
             const std::string& recipient, const std::string& status)
    : id(id), amount(amount), sender(sender), recipient(recipient), status(status)
{
    date = std::time(nullptr);
}

int Event::getId() const {
    return id;
}

double Event::getAmount() const {
    return amount;
}

std::string Event::getSender() const {
    return sender;
}

std::string Event::getRecipient() const {
    return recipient;
}

std::string Event::getStatus() const {
    return status;
}

std::time_t Event::getDate() const {
    return date;
}

void Event::setStatus(const std::string& newStatus) {
    status = newStatus;
}

void Event::setAmount(double newAmount) {
    amount = newAmount;
}

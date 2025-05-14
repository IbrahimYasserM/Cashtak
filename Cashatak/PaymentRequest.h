#pragma once
#include <string>
#include "Event.h"

class PaymentRequest : public Event
{
private:
    std::string message;

public:
    // Constructor
    PaymentRequest(int id, double amount, const std::string& sender,
           const std::string& recipient, const std::string& status,
           const std::string& message);

    // Getter
    std::string getMessage() const;

    // Setter
    void setMessage(const std::string& message);
};


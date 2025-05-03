#pragma once
#include <string>
#include "Event.h"

class Request : public Event
{
private:
    std::string message;

public:
    // Constructor
    Request(int id, double amount, const std::string& sender,
        const std::string& recipient, const std::string& status,
        const std::string& message);

    // Getter
    std::string getMessage() const;

    // Setter
    void setMessage(const std::string& message);
};
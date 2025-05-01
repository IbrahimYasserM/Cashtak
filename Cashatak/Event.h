#pragma once
#include <string>
#include <ctime>

class Event
{
private:
    int id;
    double amount;
    std::string sender;
    std::string recipient;
    std::string status;
    std::time_t date;

public:
    // Constructor
    Event(int id, double amount, const std::string& sender, 
          const std::string& recipient, const std::string& status);

    // Getters
    int getId() const;
    double getAmount() const;
    std::string getSender() const;
    std::string getRecipient() const;
    std::string getStatus() const;
    std::time_t getDate() const;

    // Setters
    void setStatus(const std::string& status);
    void setAmount(double amount);
};


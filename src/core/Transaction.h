#pragma once
#include <string>
#include "Event.h"

class Transaction : public Event
{
private:
    std::string table;
    std::string type;

public:
    // Constructor
    Transaction(int id, double amount, const std::string& sender,
               const std::string& recipient, const std::string& status,
               const std::string& table, const std::string& type);

    // Getters
    std::string getTable() const;
    std::string getType() const;

    // Setters
    void setTable(const std::string& table);
    void setType(const std::string& type);
};


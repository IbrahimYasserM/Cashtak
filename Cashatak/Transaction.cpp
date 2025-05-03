#include "stdafx.h"
#include "Transaction.h"

Transaction::Transaction(int id, double amount, const std::string& sender,
                       const std::string& recipient, const std::string& status,
                       const std::string& table, const std::string& type)
    : Event(id, amount, sender, recipient, status), table(table), type(type)
{
}

std::string Transaction::getTable() const {
    return table;
}

std::string Transaction::getType() const {
    return type;
}

void Transaction::setTable(const std::string& newTable) {
    table = newTable;
}

void Transaction::setType(const std::string& newType) {
    type = newType;
}

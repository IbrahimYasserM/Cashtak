#pragma once
#include "Account.h"
#include <vector>

class User;
class Event;

class Admin : public Account
{
public:
    Admin();
    Admin(const std::string& username, const std::string& email, 
          const std::string& password);
    ~Admin();

    std::vector<User*> ViewUsers() const;
    std::vector<Event*> ViewEvents() const;
    bool EditBalance(User* user, double newBalance);
    bool SuspendUser(User* user);
    bool deleteUser(User* user);
    bool addUser(const std::string& username, const std::string& email, 
                 const std::string& password, const std::string& accountType);
};


#pragma once
#include "Account.h"
#include <vector>

using namespace std;

class User;
class Event;

class Admin : public Account
{
public:
    Admin();
    Admin(const string& username, const string& email, 
          const string& password);
    ~Admin();

    vector<User*> ViewUsers() const;
    vector<Event*> ViewEvents() const;
    bool EditBalance(User* user, double newBalance);
    bool SuspendUser(User* user);
    bool deleteUser(User* user);
    bool addUser(const string& username, const string& email, 
                 const string& password, const string& accountType);
};


#pragma once
#include "Account.h"
#include <vector>
#include "User.h"

using namespace std;

class Admin : public Account
{
public:
    Admin();
    Admin(const string& username, const string& email, 
          const string& password);
    ~Admin();

    // User management methods
    vector<User*> getAllUsers() const; // Added method to get all users
    bool EditBalance(User* user, double newBalance);
    bool deleteUser(User* user);
    bool addUser(const string& username, const string& email, 
                 const string& password, const string& accountType);
    bool toggleUserStatus(User* user); // Added method to toggle user status
};


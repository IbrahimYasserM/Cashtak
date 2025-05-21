#pragma once
#include "Account.h"
#include <vector>
#include "User.h"
#include "Transaction.h"

using namespace std;

class Admin : public Account
{
public:
    Admin();
    Admin(const string& username, const string& email, 
          const string& password);
    ~Admin();

    // User management methods
    vector<User*> getAllUsers() const;
    bool EditBalance(User* user, double newBalance);
    bool deleteUser(User* user);
    bool addUser(const string& username, const string& email, 
                 const string& password, const string& accountType);
    bool toggleUserStatus(User* user);
    
    // Transaction viewing methods
    std::vector<Transaction*> getAllTransactions() const;
    std::vector<Transaction*> getUserTransactions(const std::string& username) const;
};


#pragma once
#include <string>

using namespace std;

class Account
{
protected:
    string username;
    string email;
    string hashedPassword;
    string accountType;

public:
    Account();
    Account(const string& username, const string& email, 
            const string& password, const string& accountType);
    virtual ~Account();

    bool login(const string& username, const string& password);
    bool registerAccount(const string& username, const string& email, 
                       const string& password, const string& accountType);
    void logout();

    // Getters
    string getUsername() const;
    string getEmail() const;
    string getAccountType() const;
};


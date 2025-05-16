#pragma once
#include <string>

using namespace std;

class Database; // Forward declaration

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
        const string& password, const string accountType);
    virtual ~Account();
	static string hashPassword(const string& password);
    
    
    // Getters
    virtual string getUsername() const;
    virtual string getEmail() const;
    virtual string getAccountType() const;
	virtual string getHashedPassword() const;

    // Make Database a friend class so it can directly access protected members
    friend class Database;
};

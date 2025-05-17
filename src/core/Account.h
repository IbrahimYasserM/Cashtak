#pragma once
#include <string>

using namespace std;

// Define status constants
enum class AccountStatus {
    ACTIVE,
    SUSPENDED
};

class Database; // Forward declaration

class Account
{
protected:
    string username;
    string email;
    string hashedPassword;
    string accountType;
    AccountStatus status; // New status field

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
    
    // Status methods
    AccountStatus getStatus() const;
    void setStatus(AccountStatus newStatus);
    bool isActive() const;
    string getStatusString() const; // Returns "ACTIVE" or "SUSPENDED"

    // Make Database a friend class so it can directly access protected members
    friend class Database;
};

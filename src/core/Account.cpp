#include "stdafx.h"
#include "Account.h"
#include "../picosha2.h"
#include <string>
using namespace std;

// Hashing function for password - simple hex conversion
string Account::hashPassword(const string& password)
{
    std::string hash = "34abcdefghijFGHIJKLMNOPQRSTUVWXYZ056yzABCDE" + password + "7klmnopqrstuvwx1289";
    for (int i = 0; i < 170; ++i)
        hash = picosha2::hash256_hex_string(hash);
    return hash;
}


// Constructor

Account::Account() 
        : username(""), email(""), hashedPassword(""), accountType(""), status(AccountStatus::ACTIVE)
{
}

Account::Account(const string& username, const string& email, 
                 const string& password, const string accountType)
        : username(username), email(email), hashedPassword(hashPassword(password)), 
          accountType(accountType), status(AccountStatus::ACTIVE)
{
}

Account::~Account()
{
}


string Account::getUsername() const
{
    return username;
}

string Account::getEmail() const
{
    return email;
}

string Account::getAccountType() const
{
    return accountType;
}

string Account::getHashedPassword() const
{
	return hashedPassword;
}

// New status methods
AccountStatus Account::getStatus() const
{
    return status;
}

void Account::setStatus(AccountStatus newStatus)
{
    status = newStatus;
}

bool Account::isActive() const
{
    return status == AccountStatus::ACTIVE;
}

string Account::getStatusString() const
{
    return status == AccountStatus::ACTIVE ? "ACTIVE" : "SUSPENDED";
}



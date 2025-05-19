#include "stdafx.h"
#include "Account.h"
#include <string>
#include <sstream>
using namespace std;


// Hashing function for password
string Account::hashPassword(const string& password)
{
    return password;
}


// Constructor

Account::Account() 
        : username(""), email(""), hashedPassword(""), accountType("")
{
}

Account::Account(const string& username, const string& email, 
                 const string& password, const string accountType)
        : username(username), email(email), hashedPassword(hashPassword(password)), accountType(accountType)
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



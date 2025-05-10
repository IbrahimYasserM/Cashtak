#include "stdafx.h"
#include "Account.h"
#include <string>
#include <sstream>
#include <functional> // For std::hash
using namespace std;


// Hashing function for password
string Account::hashPassword(const string& password)
{
    size_t hash = std::hash<string>{}(password);

    // Convert to string
    stringstream ss;
    ss << hex << hash;
    return ss.str();
}


// Constructor

Account::Account() 
        : username(""), email(""), hashedPassword(""), accountType("")
{
}

Account::Account(const string& username, const string& email, 
                 const string& password, const string& accountType)
        : username(username), email(email), hashedPassword(hashPassword(password)), accountType(accountType)
{
}

Account::~Account()
{
}

bool Account::login(const string& username, const string& password)
{
    // In a real implementation, this would check against stored credentials
    // For now just check if they match the current account
    return this->username == username && this->hashedPassword == hashPassword(password);
}

bool Account::registerAccount(const string& username, const string& email, 
                            const string& password, const string& accountType)
{
    // In a real implementation, this would check if the username is available
    // and store the new account in a database
    this->username = username;
    this->email = email;
    this->hashedPassword = hashPassword(password); 
    this->accountType = accountType;
    
    return true;
}

void Account::logout()
{
    // In a real implementation, this would clear session data or authentication tokens
    //cout << "User " << username << " logged out" << endl;
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




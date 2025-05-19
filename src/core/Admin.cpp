#include "stdafx.h"
#include "Admin.h"
#include "User.h"
#include "Event.h"
#include "Database.h" // Add Database.h include
#include <iostream>   // Add iostream for std::cerr

using namespace std;

Admin::Admin() : Account()
{
    accountType = "Admin";
}

Admin::Admin(const string& username, const string& email, const string& password)
    : Account(username, email, password, "Admin")
{
}

Admin::~Admin()
{
}

std::vector<User*> Admin::ViewUsers() const
{
    // In a real implementation, this would query the database for all users
    vector<User*> users;
    //cout << "Admin viewing all users" << endl;
    return users;
}

std::vector<Event*> Admin::ViewEvents() const
{
    // In a real implementation, this would query the database for all events
    vector<Event*> events;
    //cout << "Admin viewing all events" << endl;
    return events;
}

bool Admin::EditBalance(User* user, double newBalance)
{
    if (!user)
        return false;
        
    try {
        // Actually update the user's balance
        user->setBalanceDirectly(newBalance);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error editing balance: " << e.what() << std::endl;
        return false;
    }
}

bool Admin::deleteUser(User* user)
{
    if (!user)
        return false;
        
    // In a real implementation, this would delete the user from the database
    //cout << "Admin deleting user: " << user->getUsername() << endl;
    return true;
}

bool Admin::addUser(const string& username, const string& email, 
                    const string& password, const string& accountType)
{
    // Check if input is valid
    if (username.empty() || email.empty() || password.empty() || accountType.empty()) {
        return false;
    }
    
    // Check if username already exists in database
    Database* db = Database::getInstance();
    if (db->getAccount(username) != nullptr) {
        return false; // Username already exists
    }
    
    // Create and add the new user
    try {
        if (accountType == "User") {
            User* newUser = new User(username, email, password, 0.0);
            db->addAccount(newUser);
            return true;
        } else if (accountType == "Admin") {
            Admin* newAdmin = new Admin(username, email, password);
            db->addAccount(newAdmin);
            return true;
        }
    } catch (const std::exception& e) {
        // Log the error
        std::cerr << "Error adding user: " << e.what() << std::endl;
        return false;
    }
    
    return false;
}

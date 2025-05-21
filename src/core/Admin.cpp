#include "stdafx.h"
#include "Admin.h"
#include "User.h"
#include "Event.h"
#include "Database.h"
#include <iostream>

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

std::vector<User*> Admin::getAllUsers() const
{
    // Get all users from the database
    std::vector<User*> users;
    Database* db = Database::getInstance();
    
    for (const auto& [username, account] : db->getAccounts()) {
        if (account && account->getAccountType() == "User") {
            User* user = dynamic_cast<User*>(account);
            if (user) {
                users.push_back(user);
            }
        }
    }
    
    return users;
}

bool Admin::EditBalance(User* user, double newBalance)
{
    if (!user)
        return false;
        
    try {
        // Update the user's balance
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
    
    try {
        // Get database instance
        Database* db = Database::getInstance();
        
        // Remove the user from the database
        db->removeAccount(user->getUsername());
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error deleting user: " << e.what() << std::endl;
        return false;
    }
}

bool Admin::addUser(const string& username, const string& email, 
                    const string& password, const string& accountType)
{
    // Check if input is valid
    if (username.empty() || email.empty() || password.empty() || accountType.empty()) {
        return false;
    }
    
    // Check if username already exists
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
        std::cerr << "Error adding user: " << e.what() << std::endl;
        return false;
    }
    
    return false;
}

bool Admin::toggleUserStatus(User* user)
{
    if (!user)
        return false;
        
    try {
        // Toggle the status between ACTIVE and SUSPENDED
        AccountStatus newStatus = (user->getStatus() == AccountStatus::ACTIVE) ? 
                               AccountStatus::SUSPENDED : AccountStatus::ACTIVE;
        user->setStatus(newStatus);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error toggling user status: " << e.what() << std::endl;
        return false;
    }
}

#include "stdafx.h"
#include "Admin.h"
#include "User.h"
#include "Event.h"
#include <iostream>

Admin::Admin() : Account()
{
    accountType = "Admin";
}

Admin::Admin(const std::string& username, const std::string& email, const std::string& password)
    : Account(username, email, password, "Admin")
{
}

Admin::~Admin()
{
}

std::vector<User*> Admin::ViewUsers() const
{
    // In a real implementation, this would query the database for all users
    std::vector<User*> users;
    std::cout << "Admin viewing all users" << std::endl;
    return users;
}

std::vector<Event*> Admin::ViewEvents() const
{
    // In a real implementation, this would query the database for all events
    std::vector<Event*> events;
    std::cout << "Admin viewing all events" << std::endl;
    return events;
}

bool Admin::EditBalance(User* user, double newBalance)
{
    if (!user)
        return false;
        
    // In a real implementation, this would update the user's balance in the database
    std::cout << "Admin editing balance for user: " << user->getUsername() << std::endl;
    // user->setBalance(newBalance);
    return true;
}

bool Admin::SuspendUser(User* user)
{
    if (!user)
        return false;
        
    // In a real implementation, this would set a suspended flag on the user in the database
    std::cout << "Admin suspending user: " << user->getUsername() << std::endl;
    // user->setSuspended(true);
    return true;
}

bool Admin::deleteUser(User* user)
{
    if (!user)
        return false;
        
    // In a real implementation, this would delete the user from the database
    std::cout << "Admin deleting user: " << user->getUsername() << std::endl;
    return true;
}

bool Admin::addUser(const std::string& username, const std::string& email, 
                    const std::string& password, const std::string& accountType)
{
    // In a real implementation, this would create a new user in the database
    std::cout << "Admin adding new user: " << username << std::endl;
    return true;
}

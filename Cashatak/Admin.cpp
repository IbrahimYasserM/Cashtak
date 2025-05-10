#include "stdafx.h"
#include "Admin.h"
#include "User.h"
#include "Event.h"
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

std::vector<User*> Admin::ViewUsers() const
{
    // In a real implementation, this would query the database for all users
    vector<User*> users;
    cout << "Admin viewing all users" << endl;
    return users;
}

std::vector<Event*> Admin::ViewEvents() const
{
    // In a real implementation, this would query the database for all events
    vector<Event*> events;
    cout << "Admin viewing all events" << endl;
    return events;
}

bool Admin::EditBalance(User* user, double newBalance)
{
    if (!user)
        return false;
        
    // In a real implementation, this would update the user's balance in the database
    cout << "Admin editing balance for user: " << user->getUsername() << endl;
    // user->setBalance(newBalance);
    return true;
}

bool Admin::SuspendUser(User* user)
{
    if (!user)
        return false;
        
    // In a real implementation, this would set a suspended flag on the user in the database
    cout << "Admin suspending user: " << user->getUsername() << endl;
    // user->setSuspended(true);
    return true;
}

bool Admin::deleteUser(User* user)
{
    if (!user)
        return false;
        
    // In a real implementation, this would delete the user from the database
    cout << "Admin deleting user: " << user->getUsername() << endl;
    return true;
}

bool Admin::addUser(const string& username, const string& email, 
                    const string& password, const string& accountType)
{
    // In a real implementation, this would create a new user in the database
    cout << "Admin adding new user: " << username << endl;
    return true;
}

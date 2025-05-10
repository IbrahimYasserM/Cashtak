#include <stdafx.h>
#include "User.h"

// Initialize the static account ID counter
long long User::accountIdCounter = 1;

// Default constructor
User::User()
    : Account(), balance(0.0)
{
    accountIdCounter++;
}

// Parameterized constructor
User::User(const string& username, const string& email,
    const string& password, const string& accountType,
    double initialBalance)
    : Account(username, email, password, accountType), balance(initialBalance)
{
    accountIdCounter++;
}



// Setter for balance with password authentication
void User::setBalance(double newBalance, string password) {
	if (this->hashedPassword == hashPassword(password)) {      
        this->balance = newBalance;
    }
    else {
        cout << "Incorrect password. Balance not updated." << endl;
    }
}

// Add balance with password authentication
void User::addBalance(double amount, string password) {
    if (this->hashedPassword ==hashPassword(password)) {      
        this->balance += amount;
    }
    else {
        cout << "Incorrect password. Amount not added." << endl;
    }
}


// Edit password with username and current password check
void User::editPassword(string userName, string currentPass, string newPass) {
    if (this->getUsername() == userName && this->hashedPassword == hashPassword(currentPass)) {
        this->hashedPassword = hashPassword(newPass);
        cout << "Password updated successfully." << endl;
    }
    else {
        cout << "Incorrect username or password. Password not updated." << endl;
    }
}




// Getters 
vector<Transaction>& User::getHistoryTransactions() const {
    return const_cast<vector<Transaction>&>(completedTransactions);
}

vector<Request>& User::getPendingIncomingRequests() const {
    return const_cast<vector<Request>&>(pendingIncomingRequests);
}

vector<Request>& User::getPendingOutgoingRequests() const {
    return const_cast<vector<Request>&>(pendingOutgoingRequests);
}
double User::getBalance() const {
    return balance;
}
long long User::getAccountId() const {
	return accountIdCounter;
}




//Handle requests and transactions
void User::requestMoney(Request moneyRequest) {
    pendingOutgoingRequests.push_back(moneyRequest);
}

void User::sendMoney(Transaction sendMoney) {
    if (sendMoney.getAmount() <= balance) {
        balance -= sendMoney.getAmount();
        completedTransactions.push_back(sendMoney);
    }
    else {
        cout << "Insufficient balance to send money." << endl;
    }
}


// Destructor
User::~User() {
}
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
User::User( string username,  string email,
     string password,
    double initialBalance)
    : Account(username, email, password, "User"), balance(initialBalance)
{
    accountIdCounter++;
}



// Setter for balance with password authentication
void User::setBalance(double newBalance) {
    this->balance = newBalance;
}

// Add balance with password authentication
void User::addBalance(double amount) {
    this->balance += amount;
}


// Edit password with username and current password check
void User::editPassword(string userName, string currentPass, string newPass) {
    if (this->getUsername() == userName && this->hashedPassword == hashPassword(currentPass)) {
        this->hashedPassword = hashPassword(newPass);
        //cout << "Password updated successfully." << endl;
    }
    else {
        //cout << "Incorrect username or password. Password not updated." << endl;
    }
}




// Getters 
vector<Transaction*>& User::getHistoryTransactions() const {
    return const_cast<vector<Transaction*>&>(completedTransactions);
}

vector<PaymentRequest>& User::getPendingIncomingRequests() const {
    return const_cast<vector<PaymentRequest>&>(pendingIncomingRequests);
}

vector<PaymentRequest>& User::getPendingOutgoingRequests() const {
    return const_cast<vector<PaymentRequest>&>(pendingOutgoingRequests);
}
double User::getBalance() const {
    return balance;
}
long long User::getAccountId() const {
    return accountIdCounter;
}




//Handle requests and transactions
void User::requestMoney(PaymentRequest moneyRequest) {
    pendingOutgoingRequests.push_back(moneyRequest);
}

void User::sendMoney(Transaction* sendMoney) {
    if (sendMoney->getAmount() <= balance) {
        balance -= sendMoney->getAmount();
        completedTransactions.push_back(sendMoney);
    }
    else {
        //cout << "Insufficient balance to send money." << endl;
    }
}
void User::addPendingIncomingRequest(PaymentRequest request) {
    pendingIncomingRequests.push_back(request);
}
void User::addPendingOutgoingRequest(PaymentRequest request) {
    pendingOutgoingRequests.push_back(request);
}
void User::addTransaction(Transaction* transaction) {
	completedTransactions.push_back(transaction);
}
// Destructor
User::~User() = default;
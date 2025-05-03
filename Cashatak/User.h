#pragma once
#include "Account.h"
#include "Transaction.h"
#include "Business.h"
#include "Request.h"
#include <string>
#include <vector>

using namespace std;

class User : public Account
{
private:
    static long long accountIdCounter;   // Static counter for generating unique IDs
    double balance;
    vector<Transaction> completedTransactions;
    vector<Request> pendingIncomingRequests;  // Pending requests that other users request it from me 
    vector<Request> pendingOutgoingRequests;  // Pending requests that I request from other users

public:
    // Constructors
    User();
    User(const string& username, const string& email,
        const string& password, const string& accountType,
        double initialBalance = 0.0);
    ~User();
   
    //getters and setters 
    double getBalance() const;
    void setBalance(double newBalance,string password);
    void addBalance(double amount,string password);
    void editPassword(string userName,string currentPass,string newPass);
    
    // Methods to handle transactions and requests
    vector<Transaction>& getHistoryTransactions() const;
    vector<Request>& getPendingIncomingRequests() const;
    vector<Request>& getPendingOutgoingRequests() const;
    void requestMoney(Request moneyRequest);
    void sendMoney(Transaction sendMoney);

};

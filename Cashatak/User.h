#pragma once
#include "Account.h"
#include "Transaction.h"
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
        double initialBalance=0.0);
    ~User();
   
    //getters and setters 
    void setBalance(double newBalance,string password);
    double getBalance() const;
    long long getAccountId() const;
    vector<Transaction>& getHistoryTransactions() const;
    vector<Request>& getPendingIncomingRequests() const;
    vector<Request>& getPendingOutgoingRequests() const;
    void addBalance(double amount,string password);
    
	// Methods to handle user account
    void editPassword(string userName,string currentPass,string newPass);

    // Methods to handle transactions and requests
    void requestMoney(Request moneyRequest);
    void sendMoney(Transaction sendMoney);
	};

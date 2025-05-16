#pragma once
#include "Account.h"
#include "Transaction.h"
#include "PaymentRequest.h"
#include <string>
#include <vector>

using namespace std;

class User : public Account
{
private:
    static long long accountIdCounter;   // Static counter for generating unique IDs
    double balance;
    vector<Transaction*> completedTransactions;
    vector<PaymentRequest> pendingIncomingRequests;  // Pending requests that other users request it from me 
    vector<PaymentRequest> pendingOutgoingRequests;  // Pending requests that I request from other users

public:
    // Constructors
    User();
    User( string username,  string email,
         string password,
        double initialBalance = 0.0);
    ~User();

    //getters and setters 
    void setBalance(double newBalance, string password);
    double getBalance() const;
    long long getAccountId() const;
    vector<Transaction*>& getHistoryTransactions() const;
    vector<PaymentRequest>& getPendingIncomingRequests() const;
    vector<PaymentRequest>& getPendingOutgoingRequests() const;
    void addBalance(double amount, string password);

    // Methods to handle user account
    void editPassword(string userName, string currentPass, string newPass);

    // Methods to handle transactions and requests
    void requestMoney(PaymentRequest moneyRequest);
    void sendMoney(Transaction* sendMoney);
    void addPendingIncomingRequest(PaymentRequest request);
    void addPendingOutgoingRequest(PaymentRequest request);
	void addTransaction(Transaction* transaction);

    // Add this to allow Database to directly set balance
    void setBalanceDirectly(double newBalance) {
        balance = newBalance;
    }
    
    // Make Database class a friend so it can access protected members
    friend class Database;
};

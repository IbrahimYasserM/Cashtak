#pragma once
#include "Account.h"
#include "Transaction.h"
#include "PaymentRequest.h"
#include "Notification.h"
#include <string>
#include <vector>
#include <queue>
#include <algorithm> // For std::remove_if

using namespace std;

class User : public Account
{
private:
    static long long accountIdCounter;   // Static counter for generating unique IDs
    double balance;
    vector<Transaction*> completedTransactions;
    vector<PaymentRequest> pendingIncomingRequests;  // Pending requests that other users request it from me 
    vector<PaymentRequest> pendingOutgoingRequests;  // Pending requests that I request from other users
    queue<Notification> notifications;

public:
    // Constructors
    User();
    User( string username,  string email,
         string password,
        double initialBalance = 0.0);
    ~User();

    //getters and setters 
    void setBalance(double newBalance);
    double getBalance() const;
    long long getAccountId() const;
    vector<Transaction*>& getHistoryTransactions() const;
    vector<PaymentRequest>& getPendingIncomingRequests() const;
    vector<PaymentRequest>& getPendingOutgoingRequests() const;
    queue<Notification>& getNotifications() const;
    void addBalance(double amount);

    // Methods to handle user account
    void editPassword(string userName, string currentPass, string newPass);

    // Methods to handle transactions and requests
    void requestMoney(PaymentRequest moneyRequest);
    void sendMoney(Transaction* sendMoney);
    void addPendingIncomingRequest(PaymentRequest request);
    void addPendingOutgoingRequest(PaymentRequest request);
    bool removePendingIncomingRequest(int requestId); // Added for Phase 2
    bool removePendingOutgoingRequest(int requestId); // Added for Phase 2
	void addTransaction(Transaction* transaction);
    void addNotificationTransaction(Transaction* transaction);
    void addNotificationRequest(PaymentRequest* request);


    // Add this to allow Database and Admin to directly set balance
    void setBalanceDirectly(double newBalance) {
        balance = newBalance;
    }
    
    // Make Database and Admin classes friends so they can access protected members
    friend class Database;
    friend class Admin;
};

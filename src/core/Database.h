#pragma once
#include "User.h"
#include "Event.h"
#include "Account.h"
#include "Admin.h"
#include "PaymentRequest.h"
#include "Transaction.h"
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>

class Database
{
private:
	Database();
	Database(const Database&) = delete;
	Database& operator=(const Database&) = delete;
	static Database* database;
	std::fstream myfile;
	std::unordered_map<std::string, Account*> accounts;
	std::vector<Transaction*> transactions;
	User* user = nullptr;
	Admin* admin = nullptr;
	std::string filePath;
	std::vector<std::string> fileNames = { "Admins.txt", "Users.txt","CompletedTransactions.txt" };

public:
	~Database();
	static Database* getInstance();
	static void cleanUp();
	void addAccount(Account* account);
	void removeAccount(const std::string& username);
	void createDummyData();
	void addTransaction(Transaction* transaction);
	Account* getCurrentAccount();
	Account* getAccount(const std::string  username);
	Transaction* getTransaction(int id);
	void setCurrentAccount(Account* account);

	const std::vector<Transaction*>& getAllTransactions() const { return transactions; }
	const std::vector<Transaction*>& getTransactions() const { return transactions; }
	const std::unordered_map<std::string, Account*>& getAccounts() const { return accounts; }
};


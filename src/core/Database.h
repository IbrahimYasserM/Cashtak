#pragma once
#include "User.h"
#include "Event.h"
#include "Account.h"
#include "Admin.h"
#include "PaymentRequest.h"
#include "Transaction.h"
#include <map>
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

public:
	~Database();
	static Database* getInstance();
	std::map<std::string, Account*> accounts;
	std::vector<Event*> events;
	User* user = nullptr;
	Admin* admin = nullptr;
};


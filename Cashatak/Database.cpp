#include "stdafx.h"
#include "Database.h"
#include <sstream>


Database* Database::database = nullptr;

Database::Database() {
   myfile.open("Accounts.txt", std::ios::in | std::ios::out | std::ios::app);
   if (!myfile) {
       throw std::runtime_error("Unable to open file");
       return;
   }
   // logic to read from the file and populate accounts, events, user, admin, business
   myfile.close();
}
Database::~Database() {
	// Destructor implementation
	// logic to write back to the file
	// Clean up dynamically allocated memory
	for (auto& account : accounts) {
		delete account.second;
	}
	for (auto& event : events) {
		delete event;
	}
	if (user) {
		delete user;
	}
	if (admin) {
		delete admin;
	}
	if (business) {
		delete business;
	}
}
Database* Database::getInstance() {
	if (database == nullptr) {
		database = new Database();
	}
	return database;
}

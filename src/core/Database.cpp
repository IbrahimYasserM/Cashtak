#include "stdafx.h"
#include "Database.h"
#include <sstream>
#include <iostream>
#include <QDir>
#include <QCoreApplication>

Database* Database::database = nullptr;

Database::Database() {
    // Resolve the path relative to the application's directory
    QString appPath = QCoreApplication::applicationDirPath();
    QString resolvedPath = QDir(appPath).filePath("../../src/resources/Data/");
    filePath = resolvedPath.toStdString();
    
    std::cout << "Using data path: " << filePath << std::endl;
    
    // Create the directory if it doesn't exist
    QDir dir(resolvedPath);
    if (!dir.exists()) {
        std::cout << "Creating directory: " << filePath << std::endl;
        if (!dir.mkpath(".")) {
            std::cerr << "Failed to create directory: " << filePath << std::endl;
        }
    }
    
    bool fileReadSuccess = false;
    
    // Rest of the constructor remains the same
    for (auto& fileName : fileNames) {
        myfile.open(filePath + fileName, std::ios::in);
        if (!myfile) {
            std::cerr << "Error opening " << fileName << " for reading from " << filePath << std::endl;
            // Create the file if it doesn't exist
            std::ofstream createFile(filePath + fileName);
            if (createFile) {
                std::cout << "Created new file: " << fileName << std::endl;
                createFile.close();
                // Try opening again
                myfile.open(filePath + fileName, std::ios::in);
                if (!myfile) {
                    std::cerr << "Still can't open " << fileName << " after creation." << std::endl;
                    continue;
                }
            } else {
                std::cerr << "Failed to create " << fileName << std::endl;
                continue;
            }
        }

        // logic to read from the file and populate accounts, events, user, admin
        std::string line;
        if (fileName == "Admins.txt") {
            while (std::getline(myfile, line)) {
                std::istringstream iss(line);
                std::string username, email, password;
                if (!(iss >> username >> email >> password)) {
                    throw std::runtime_error("No enough paramaters in " + fileName);
                }
                Admin* account = new Admin(username, email, password);
                accounts[username] = account;
            }
        }
        else if (fileName == "Users.txt") {
            while (std::getline(myfile, line)) {
                std::istringstream iss(line);
                std::string username, email, password;
                double balance;
                if (!(iss >> username >> email >> password >> balance)) {
                    throw std::runtime_error("No enough paramaters in " + fileName); // error
                }
                User* account = new User(username, email, password, balance);

                accounts[username] = account;
            }
        }
        else if (fileName == "CompletedTransactions.txt") {
            while (std::getline(myfile, line)) {
                std::istringstream iss(line);
                int id;
                double amount;
                std::string sender, recipient, table, type;
                if (!(iss >> id >> amount >> sender >> recipient >> table >> type)) {
                    throw std::runtime_error("No enough paramaters in " + fileName);
                }
                Transaction* transaction = new Transaction(id, amount, sender, recipient, "Completed", table, type);
                if (accounts.find(sender) != accounts.end() && accounts.find(recipient) != accounts.end()) {
					if (accounts[sender]->getAccountType() == "User" && accounts[recipient]->getAccountType() == "User") {
						User* senderAcc = dynamic_cast<User*>(accounts[sender]);
						User* recipientAcc = dynamic_cast<User*>(accounts[recipient]);
						if (senderAcc && recipientAcc) {
							senderAcc->addTransaction(transaction);
							recipientAcc->addTransaction(transaction);
						}
						else throw std::runtime_error("Sender or recipient account not found");
					}
					else throw std::runtime_error("Sender or recipient account not found or not a user");
				}
				else {
					throw std::runtime_error("Sender or recipient account not found");
                }
                transactions.push_back(transaction);
            }
        }
        myfile.close();
        fileReadSuccess = true;
    }
    
    // If we couldn't read any files, create dummy data
    if (!fileReadSuccess) {
        std::cout << "No files could be read. Creating dummy data." << std::endl;
        createDummyData();
        saveToFiles();
    }
}

Database::~Database() {
    std::cout << "Database destructor called. Starting to write files..." << std::endl;
    
    // Write Admins
    std::cout << "Attempting to write to: " << (filePath + "Admins.txt") << std::endl;
    std::ofstream adminFile(filePath + "Admins.txt", std::ios::out);
    if (adminFile) {
        for (const auto& [username, account] : accounts) {
            if (account->getAccountType() == "Admin") {
                adminFile << account->getUsername() << " "
                    << account->getEmail() << " "
                    << account->getHashedPassword() << std::endl;
                std::cout << "Wrote admin account: " << account->getUsername() << std::endl;
            }
        }
        adminFile.close();
    } else {
        std::cerr << "Failed to open Admins.txt for writing!" << std::endl;
    }

    // Write Users
    std::cout << "Attempting to write to: " << (filePath + "Users.txt") << std::endl;
    std::ofstream userFile(filePath + "Users.txt", std::ios::out);
    if (userFile) {
        for (const auto& [username, account] : accounts) {
            if (account->getAccountType() == "User") {
                User* user = dynamic_cast<User*>(account);
                if (user) {
                    userFile << user->getUsername() << " "
                        << user->getEmail() << " "
                        << user->getHashedPassword() << " "
                        << user->getBalance() << std::endl;
                    std::cout << "Wrote user account: " << user->getUsername() << std::endl;
                }
            }
        }
        userFile.close();
    } else {
        std::cerr << "Failed to open Users.txt for writing!" << std::endl;
    }

    // Write Completed Transactions
    std::cout << "Attempting to write to: " << (filePath + "CompletedTransactions.txt") << std::endl;
    std::ofstream transFile(filePath + "CompletedTransactions.txt", std::ios::out);
    if (transFile) {
        for (const auto& transaction : transactions) {
            transFile << transaction->getId() << " "
                << transaction->getAmount() << " "
                << transaction->getSender() << " "
                << transaction->getRecipient() << " "
                << transaction->getTable() << " "
                << transaction->getType() << std::endl;
            std::cout << "Wrote transaction ID: " << transaction->getId() << std::endl;
        }
        transFile.close();
    } else {
        std::cerr << "Failed to open CompletedTransactions.txt for writing!" << std::endl;
    }

    // Write Pending Outgoing Requests
    std::cout << "Attempting to write to: " << (filePath + "pendingOutgoingRequests.txt") << std::endl;
    std::ofstream pendingFile(filePath + "pendingOutgoingRequests.txt", std::ios::out);
    if (pendingFile) {
        std::set<int> writtenRequestIds;
        for (const auto& [username, account] : accounts) {
            if (account->getAccountType() == "User") {
                User* user = dynamic_cast<User*>(account);
                if (user) {
                    for (const auto& req : user->getPendingOutgoingRequests()) {
                        if (writtenRequestIds.insert(req.getId()).second) {
                            pendingFile << req.getId() << " "
                                << req.getAmount() << " "
                                << req.getSender() << " "
                                << req.getRecipient() << " "
                                << req.getStatus() << " "
                                << req.getMessage() << std::endl;
                            std::cout << "Wrote pending request ID: " << req.getId() << std::endl;
                        }
                    }
                }
            }
        }
        pendingFile.close();
    } else {
        std::cerr << "Failed to open pendingOutgoingRequests.txt for writing!" << std::endl;
    }

    std::cout << "Cleaning up memory..." << std::endl;
    // Delete all dynamically allocated memory
    for (auto& [username, account] : accounts) {
        std::cout << "Deleting account: " << username << std::endl;
        delete account;
    }
    accounts.clear();

    for (auto& transaction : transactions) {
        std::cout << "Deleting transaction ID: " << transaction->getId() << std::endl;
        delete transaction;
    }
    transactions.clear();

    user = nullptr;
    admin = nullptr;
    std::cout << "Database destructor completed." << std::endl;
}

void Database::createDummyData() {
    // Create dummy admin
    Admin* admin1 = new Admin("admin3", "admin1@example.com", "admin123");
    accounts["admin1"] = admin1;

    // Create dummy users
    User* user1 = new User("user1", "user1@example.com", "pass123", 1000.0);
    User* user2 = new User("user2", "user2@example.com", "pass456", 2000.0);
    User* user3 = new User("user3", "user3@example.com", "pass789", 3000.0);

    accounts["user1"] = user1;
    accounts["user2"] = user2;
    accounts["user3"] = user3;

    // Create dummy transactions
    Transaction* trans1 = new Transaction(1, 100.0, "user1", "user2", "Completed", "Transfer", "Send");
    Transaction* trans2 = new Transaction(2, 200.0, "user2", "user3", "Completed", "Transfer", "Send");
    Transaction* trans3 = new Transaction(3, 150.0, "user3", "user1", "Completed", "Transfer", "Send");

    transactions.push_back(trans1);
    transactions.push_back(trans2);
    transactions.push_back(trans3);

    // Create dummy payment requests
    PaymentRequest req1(4, 50.0, "user1", "user2", "Pending", "Please pay me");
    PaymentRequest req2(5, 75.0, "user2", "user3", "Pending", "Dinner payment");

    // Add the pending requests to users
    user1->addPendingOutgoingRequest(req1);
    user2->addPendingIncomingRequest(req1);

    user2->addPendingOutgoingRequest(req2);
    user3->addPendingIncomingRequest(req2);

    std::cout << "Dummy data created successfully!" << std::endl;
}

Database* Database::getInstance() {
    if (database == nullptr) {
        database = new Database();
    }
    return database;
}
void Database::addAccount(Account* account) {
	if (accounts.find(account->getUsername()) != accounts.end()) {
		throw std::runtime_error("Account with this username already exists");
		return;
	}
    accounts[account->getUsername()] = account;
}
void Database::removeAccount(const std::string& username) {
    auto it = accounts.find(username);
    if (it != accounts.end()) {
        delete it->second; // Free the memory
        accounts.erase(it);
    }
    else {
        std::cout << "Account not found." << std::endl;
    }
}
Account* Database::getAccount(const std::string username) {
    auto it = accounts.find(username);
    if (it != accounts.end()) {
        return it->second;
    }
    else {
        std::cout << "Account not found." << std::endl;
        return nullptr;
    }
}
Account* Database::getCurrentAccount() {
	if (user) {
		return user;
	}
	else if (admin) {
		return admin;
	}
	else {
		std::cout << "No current account." << std::endl;
		return nullptr;
	}
}
Transaction* Database::getTransaction(int id) {
	for (auto& transaction : transactions) {
		if (transaction->getId() == id) {
			return transaction;
		}
	}
	std::cout << "Transaction not found." << std::endl;
	return nullptr;
}
void Database::cleanUp() {
	if (database) {
		delete database;
		database = nullptr;
	}
}
void Database::addTransaction(Transaction* transaction) {
	std::string sender = transaction->getSender();
	std::string recipient = transaction->getRecipient();
    if (accounts.find(sender) != accounts.end() && accounts.find(recipient) != accounts.end()) {
        if (accounts[sender]->getAccountType() == "User" && accounts[recipient]->getAccountType() == "User") {
            User* senderAcc = dynamic_cast<User*>(accounts[sender]);
            User* recipientAcc = dynamic_cast<User*>(accounts[recipient]);
            if (senderAcc && recipientAcc) {
                senderAcc->addTransaction(transaction);
                recipientAcc->addTransaction(transaction);
            }
            else throw std::runtime_error("Sender or recipient account not found");
        }
        else throw std::runtime_error("Sender or recipient account not found or not a user");
    }
    else {
        throw std::runtime_error("Sender or recipient account not found");
    }
    transactions.push_back(transaction);
}
void Database::setCurrentAccount(Account* account) {
    if (!account) {
        user = nullptr;
        admin = nullptr;
        return;
    }
    
    if (account->getAccountType() == "User") {
        user = dynamic_cast<User*>(account);
        admin = nullptr;
    } else if (account->getAccountType() == "Admin") {
        admin = dynamic_cast<Admin*>(account);
        user = nullptr;
    }
}

bool Database::saveToFiles() {
    bool success = true;
    
    // Write Admins
    std::ofstream adminFile(filePath + "Admins.txt", std::ios::out);
    if (adminFile) {
        for (const auto& [username, account] : accounts) {
            if (account->getAccountType() == "Admin") {
                adminFile << account->getUsername() << " "
                    << account->getEmail() << " "
                    << account->getHashedPassword() << std::endl;
            }
        }
        adminFile.close();
    } else {
        std::cerr << "Failed to open Admins.txt for writing!" << std::endl;
        success = false;
    }

    // Write Users
    std::ofstream userFile(filePath + "Users.txt", std::ios::out);
    if (userFile) {
        for (const auto& [username, account] : accounts) {
            if (account->getAccountType() == "User") {
                User* user = dynamic_cast<User*>(account);
                if (user) {
                    userFile << user->getUsername() << " "
                        << user->getEmail() << " "
                        << user->getHashedPassword() << " "
                        << user->getBalance() << std::endl;
                }
            }
        }
        userFile.close();
    } else {
        std::cerr << "Failed to open Users.txt for writing!" << std::endl;
        success = false;
    }
    
    return success;
}
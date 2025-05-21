#include "stdafx.h"
#include "AdminPage.h"
#include "ui_AdminPage.h"
#include <QMessageBox>
#include <QInputDialog>
#include "../dialogs/TransactionsDialog.h"

AdminPage::AdminPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminPage)
{
    ui->setupUi(this);
    setupConnections();
    refreshUserTable();
    
}

AdminPage::~AdminPage()
{
    delete ui;
}

void AdminPage::setupConnections()
{
    // All button connections are handled by the auto-generated slots
}

void AdminPage::refreshUserTable()
{
    loadUsers();
    ui->tableWidgetUsers->clearContents();
    ui->tableWidgetUsers->setRowCount(0);

    for (const auto& user : userList) {
        int row = ui->tableWidgetUsers->rowCount();
        ui->tableWidgetUsers->insertRow(row);
        
        ui->tableWidgetUsers->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(user->getUsername())));
        ui->tableWidgetUsers->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(user->getEmail())));
        ui->tableWidgetUsers->setItem(row, 2, new QTableWidgetItem(QString::number(user->getBalance())));
        ui->tableWidgetUsers->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(user->getStatusString())));
        
        // Add transactions button in the last column
        QPushButton* transButton = new QPushButton("Transactions", this);
        transButton->setProperty("username", QString::fromStdString(user->getUsername()));
        connect(transButton, &QPushButton::clicked, this, &AdminPage::on_userTransactionButtonClicked);
        ui->tableWidgetUsers->setCellWidget(row, 4, transButton);
    }

    ui->tableWidgetUsers->resizeColumnsToContents();
}

void AdminPage::loadUsers()
{
    userList.clear();
    
    // Get the admin account from the database
    Database* db = Database::getInstance();
    Account* currentAccount = db->getCurrentAccount();
    
    if (currentAccount && currentAccount->getAccountType() == "Admin") {
        admin = dynamic_cast<Admin*>(currentAccount);
        if (admin) {
            // Use the Admin class method to get all users
            userList = admin->getAllUsers();
        }
    }
}

std::vector<std::string> AdminPage::getAllUsernames()
{
    std::vector<std::string> usernames;
    
    // Extract usernames from the userList
    for (const auto& user : userList) {
        usernames.push_back(user->getUsername());
    }
    
    return usernames;
}

void AdminPage::searchUsers(const QString& searchTerm)
{
    if (searchTerm.isEmpty()) {
        refreshUserTable();
        return;
    }
    
    ui->tableWidgetUsers->clearContents();
    ui->tableWidgetUsers->setRowCount(0);
    
    for (const auto& user : userList) {
        QString username = QString::fromStdString(user->getUsername());
        QString email = QString::fromStdString(user->getEmail());
        
        if (username.contains(searchTerm, Qt::CaseInsensitive) || 
            email.contains(searchTerm, Qt::CaseInsensitive)) {
            int row = ui->tableWidgetUsers->rowCount();
            ui->tableWidgetUsers->insertRow(row);
            
            ui->tableWidgetUsers->setItem(row, 0, new QTableWidgetItem(username));
            ui->tableWidgetUsers->setItem(row, 1, new QTableWidgetItem(email));
            ui->tableWidgetUsers->setItem(row, 2, new QTableWidgetItem(QString::number(user->getBalance())));
        }
    }
    
    ui->tableWidgetUsers->resizeColumnsToContents();
}

void AdminPage::on_logout_clicked()
{
    emit navigateToLoginRequested();
}

void AdminPage::on_pushButtonRefresh_clicked()
{
    refreshUserTable();
}

void AdminPage::on_pushButtonSearch_clicked()
{
    QString searchTerm = ui->lineEditSearch->text();
    searchUsers(searchTerm);
}

void AdminPage::on_pushButtonEditBalance_clicked()
{
    int selectedRow = ui->tableWidgetUsers->currentRow();
    if (selectedRow < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a user to edit.");
        return;
    }
    
    QString username = ui->tableWidgetUsers->item(selectedRow, 0)->text();
    double currentBalance = ui->tableWidgetUsers->item(selectedRow, 2)->text().toDouble();
    
    bool ok;
    double newBalance = QInputDialog::getDouble(this, "Edit Balance", 
                                             "Enter new balance for " + username,
                                             currentBalance, 0, 1000000, 2, &ok);
    if (ok) {
        Database* db = Database::getInstance();
        Account* account = db->getAccount(username.toStdString());
        if (account && account->getAccountType() == "User") {
            User* user = dynamic_cast<User*>(account);
            if (user && admin) {
                // Use the Admin class method to edit balance
                if (admin->EditBalance(user, newBalance)) {
                    QMessageBox::information(this, "Success", "Balance updated successfully!");
                    refreshUserTable();
                } else {
                    QMessageBox::warning(this, "Error", "Failed to update balance.");
                }
            }
        }
    }
}

void AdminPage::on_pushButtonDeleteUser_clicked()
{
    int selectedRow = ui->tableWidgetUsers->currentRow();
    if (selectedRow < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a user to delete.");
        return;
    }
    
    QString username = ui->tableWidgetUsers->item(selectedRow, 0)->text();
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Deletion", 
                                                             "Are you sure you want to delete user " + username + "?\nThis action cannot be undone.",
                                                             QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        Database* db = Database::getInstance();
        Account* account = db->getAccount(username.toStdString());
        if (account && account->getAccountType() == "User" && admin) {
            User* user = dynamic_cast<User*>(account);
            
            // Use the Admin class method to delete the user
            if (admin->deleteUser(user)) {
                QMessageBox::information(this, "Success", "User deleted successfully!");
                refreshUserTable();
            } else {
                QMessageBox::warning(this, "Error", "Failed to delete user.");
            }
        }
    }
}

void AdminPage::on_pushButtonAddUser_clicked()
{
    // Open a dialog to get user information
    QDialog dialog(this);
    dialog.setWindowTitle("Add New User");
    
    QFormLayout formLayout(&dialog);
    
    QLineEdit usernameEdit;
    formLayout.addRow("Username:", &usernameEdit);
    
    QLineEdit emailEdit;
    formLayout.addRow("Email:", &emailEdit);
    
    QLineEdit passwordEdit;
    passwordEdit.setEchoMode(QLineEdit::Password);
    formLayout.addRow("Password:", &passwordEdit);
    
    QDoubleSpinBox balanceSpinBox;
    balanceSpinBox.setRange(0, 1000000);
    balanceSpinBox.setValue(0);
    formLayout.addRow("Initial Balance:", &balanceSpinBox);
    
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                              Qt::Horizontal, &dialog);
    formLayout.addRow(&buttonBox);
    
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        QString username = usernameEdit.text();
        QString email = emailEdit.text();
        QString password = passwordEdit.text();
        double balance = balanceSpinBox.value();
        
        if (username.isEmpty() || email.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "Invalid Input", "All fields are required.");
            return;
        }
        
        // Check if username already exists
        Database* db = Database::getInstance();
        if (db->getAccount(username.toStdString())) {
            QMessageBox::warning(this, "Username Taken", "This username is already taken. Please choose another one.");
            return;
        }
        
        if (admin) {
            // Let the admin handle user creation
            bool success = admin->addUser(username.toStdString(), email.toStdString(), 
                                         password.toStdString(), "User");
            if (success) {
                // Set the balance for the new user
                User* newUser = dynamic_cast<User*>(db->getAccount(username.toStdString()));
                if (newUser) {
                    newUser->setBalanceDirectly(balance);
                }
                
                QMessageBox::information(this, "Success", "User added successfully!");
                refreshUserTable();
            } else {
                QMessageBox::warning(this, "Error", "Failed to add user. The username may already exist.");
            }
        } else {
            QMessageBox::warning(this, "Error", "Admin privileges required to add users.");
        }
    }
}

// Add a new method to toggle user status
void AdminPage::on_pushButtonToggleStatus_clicked()
{
    int selectedRow = ui->tableWidgetUsers->currentRow();
    if (selectedRow < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a user first.");
        return;
    }
    
    QString username = ui->tableWidgetUsers->item(selectedRow, 0)->text();
    
    Database* db = Database::getInstance();
    Account* account = db->getAccount(username.toStdString());
    
    if (account && account->getAccountType() == "User" && admin) {
        User* user = dynamic_cast<User*>(account);
        
        // Use the Admin class method to toggle user status
        if (admin->toggleUserStatus(user)) {
            QString statusText = (user->getStatus() == AccountStatus::ACTIVE) ? "activated" : "suspended";
            QMessageBox::information(this, "Status Changed", 
                                  "User " + username + " has been " + statusText + ".");
            refreshUserTable();
        } else {
            QMessageBox::warning(this, "Error", "Failed to change user status.");
        }
    }
}

void AdminPage::on_pushButtonViewAllTransactions_clicked()
{
    if (admin) {
        std::vector<Transaction*> allTransactions = admin->getAllTransactions();
        TransactionsDialog dialog(allTransactions, "All Transactions", this);
        dialog.exec();
    } else {
        QMessageBox::warning(this, "Error", "Admin account required to view transactions.");
    }
}

void AdminPage::on_userTransactionButtonClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    
    QString username = button->property("username").toString();
    
    if (admin && !username.isEmpty()) {
        std::vector<Transaction*> userTransactions = admin->getUserTransactions(username.toStdString());
        TransactionsDialog dialog(userTransactions, 
                                "Transactions for " + username, 
                                this);
        dialog.exec();
    }
}
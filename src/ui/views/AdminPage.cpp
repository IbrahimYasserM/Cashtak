#include "stdafx.h"
#include "AdminPage.h"
#include "ui_AdminPage.h"
#include <QMessageBox>
#include <QInputDialog>

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
            userList = admin->ViewUsers();
        }
    }
    
    // If we couldn't get users from the admin (functionality not implemented), 
    // try to get users directly from the database
    if (userList.empty()) {
        for (const auto& username : getAllUsernames()) {
            Account* account = db->getAccount(username);
            if (account && account->getAccountType() == "User") {
                User* user = dynamic_cast<User*>(account);
                if (user) {
                    userList.push_back(user);
                }
            }
        }
    }
}

std::vector<std::string> AdminPage::getAllUsernames()
{
    std::vector<std::string> usernames;
    Database* db = Database::getInstance();
    
    // Get all accounts from database and filter users
    for (const auto& [username, account] : db->getAccounts()) {
        if (account && account->getAccountType() == "User") {
            usernames.push_back(username);
        }
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

void AdminPage::on_pushButtonBackHome_clicked()
{
    emit navigateToHomeRequested();
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
            if (user) {
                // Directly set the balance without going through Admin
                user->setBalanceDirectly(newBalance);
                // Save changes to file
                db->saveToFiles();
                QMessageBox::information(this, "Success", "Balance updated successfully!");
                refreshUserTable();
            } else {
                QMessageBox::warning(this, "Error", "Could not cast account to User type.");
            }
        } else {
            QMessageBox::warning(this, "Error", "Selected account is not a valid user.");
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
        
        // Directly remove the account from the database
        db->removeAccount(username.toStdString());
        
        // Save changes to files
        db->saveToFiles();
        
        QMessageBox::information(this, "Success", "User deleted successfully!");
        refreshUserTable();
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
                    // Use an empty password since we're setting directly after creation
                    newUser->setBalanceDirectly(balance);
                    db->saveToFiles(); // Save changes to disk
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
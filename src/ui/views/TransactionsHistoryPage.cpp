// TransactionsHistoryPage.cpp
#include "TransactionsHistoryPage.h"
#include "ui_TransactionsHistoryPage.h"
#include "../core/Database.h"
#include <QDateTime>
#include <QMessageBox>
#include <algorithm>
#include <iostream>
TransactionsHistoryPage::TransactionsHistoryPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransactionsHistoryPage),
    m_currentUser(nullptr)
{
    ui->setupUi(this);
    
    // Setup the transaction table
    setupTransactionTable();
}

TransactionsHistoryPage::~TransactionsHistoryPage()
{
    delete ui;
}

void TransactionsHistoryPage::loadUserData(User* currentUser)
{
    m_currentUser = currentUser;
    
    if (m_currentUser) {
        displayRecentTransactions();
    }
}

void TransactionsHistoryPage::setupTransactionTable()
{
    // Configure table for transactions
    ui->tableWidgetTransactions->setColumnCount(6);
    ui->tableWidgetTransactions->setHorizontalHeaderLabels(
        {"ID", "Date", "Amount", "Sender", "Recipient", "Status"});
    
    // Set column widths
    ui->tableWidgetTransactions->setColumnWidth(0, 50);  // ID
    ui->tableWidgetTransactions->setColumnWidth(1, 150); // Date
    ui->tableWidgetTransactions->setColumnWidth(2, 100); // Amount
    ui->tableWidgetTransactions->setColumnWidth(3, 150); // Sender
    ui->tableWidgetTransactions->setColumnWidth(4, 150); // Recipient
    ui->tableWidgetTransactions->setColumnWidth(5, 100); // Status
    
    // Make table read-only
    ui->tableWidgetTransactions->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetTransactions->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetTransactions->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidgetTransactions->horizontalHeader()->setStretchLastSection(true);
}

void TransactionsHistoryPage::setupRequestTable()
{
    // Configure table for payment requests
    ui->tableWidgetTransactions->setColumnCount(6);
    ui->tableWidgetTransactions->setHorizontalHeaderLabels(
        {"ID", "Date", "Amount", "From", "To", "Message"});
    
    // Set column widths (similar to transaction table)
    ui->tableWidgetTransactions->setColumnWidth(0, 50);   // ID
    ui->tableWidgetTransactions->setColumnWidth(1, 150);  // Date
    ui->tableWidgetTransactions->setColumnWidth(2, 100);  // Amount
    ui->tableWidgetTransactions->setColumnWidth(3, 150);  // From
    ui->tableWidgetTransactions->setColumnWidth(4, 150);  // To
    ui->tableWidgetTransactions->setColumnWidth(5, 200);  // Message
}

void TransactionsHistoryPage::clearTable()
{
    ui->tableWidgetTransactions->setRowCount(0);
}

void TransactionsHistoryPage::displayRecentTransactions()
{
    if (!m_currentUser) {
        return;
    }
    
    // Set the view to transactions
    setupTransactionTable();
    clearTable();
    
    // Get user's completed transactions
    std::vector<Transaction*> transactions = m_currentUser->getHistoryTransactions();
	std::cout << "Number of transactions: " << transactions.size() << std::endl;
    // Show the most recent 3 transactions (or all if less than 3)
    int count = std::min(3, static_cast<int>(transactions.size()));
    
    // If we have transactions in reverse chronological order (newest first)
    for (int i = 0; i < count; i++) {
        Transaction* transaction = transactions[transactions.size() - 1 - i]; // Start from the end for newest first
        addTransactionToTable(transaction);
    }
    
    // Update title based on what we're showing
    ui->labelTitle->setText("Recent Transactions");
}

void TransactionsHistoryPage::addTransactionToTable(Transaction* transaction)
{
    int row = ui->tableWidgetTransactions->rowCount();
    ui->tableWidgetTransactions->insertRow(row);
    
    // Convert time_t to readable date format
    QDateTime dateTime;
    dateTime.setSecsSinceEpoch(transaction->getDate());
    QString dateString = dateTime.toString("yyyy-MM-dd hh:mm:ss");
    
    // Set the data for this row
    ui->tableWidgetTransactions->setItem(row, 0, new QTableWidgetItem(QString::number(transaction->getId())));
    ui->tableWidgetTransactions->setItem(row, 1, new QTableWidgetItem(dateString));
    ui->tableWidgetTransactions->setItem(row, 2, new QTableWidgetItem(QString::number(transaction->getAmount(), 'f', 2)));
    ui->tableWidgetTransactions->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(transaction->getSender())));
    ui->tableWidgetTransactions->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(transaction->getRecipient())));
    ui->tableWidgetTransactions->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(transaction->getStatus())));
    
    // Color-code the row based on transaction type
    QColor rowColor;
    if (transaction->getSender() == m_currentUser->getUsername()) {
        // Outgoing transaction (sent) - light red
        rowColor = QColor(255, 235, 235);
    } else {
        // Incoming transaction (received) - light green
        rowColor = QColor(235, 255, 235);
    }
    
    // Apply color to all cells in the row
    for (int col = 0; col < ui->tableWidgetTransactions->columnCount(); ++col) {
        if (ui->tableWidgetTransactions->item(row, col)) {
            ui->tableWidgetTransactions->item(row, col)->setBackground(rowColor);
        }
    }
}

void TransactionsHistoryPage::addRequestToTable(const PaymentRequest& request)
{
    int row = ui->tableWidgetTransactions->rowCount();
    ui->tableWidgetTransactions->insertRow(row);
    
    // Convert time_t to readable date format
    QDateTime dateTime;
    dateTime.setSecsSinceEpoch(request.getDate());
    QString dateString = dateTime.toString("yyyy-MM-dd hh:mm:ss");
    
    // Set the data for this row
    ui->tableWidgetTransactions->setItem(row, 0, new QTableWidgetItem(QString::number(request.getId())));
    ui->tableWidgetTransactions->setItem(row, 1, new QTableWidgetItem(dateString));
    ui->tableWidgetTransactions->setItem(row, 2, new QTableWidgetItem(QString::number(request.getAmount(), 'f', 2)));
    ui->tableWidgetTransactions->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(request.getSender())));
    ui->tableWidgetTransactions->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(request.getRecipient())));
    ui->tableWidgetTransactions->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(request.getMessage())));
}

void TransactionsHistoryPage::showAllTransactions()
{
    if (!m_currentUser) {
        return;
    }
    
    // Set the view to transactions
    setupTransactionTable();
    clearTable();
    
    // Get all user's completed transactions
    std::vector<Transaction*> transactions = m_currentUser->getHistoryTransactions();
    
    // If we have transactions in reverse chronological order (newest first)
    for (int i = transactions.size() - 1; i >= 0; i--) {
        addTransactionToTable(transactions[i]);
    }
    
    // Update title
    ui->labelTitle->setText("All Completed Transactions");
}

void TransactionsHistoryPage::showIncomingRequests()
{
    if (!m_currentUser) {
        return;
    }
    
    // Set the view to payment requests
    setupRequestTable();
    clearTable();
    
    // Get user's pending incoming requests
    std::vector<PaymentRequest> incomingRequests = m_currentUser->getPendingIncomingRequests();
    
    // Display all incoming requests
    for (const auto& request : incomingRequests) {
        addRequestToTable(request);
    }
    
    // Update title
    ui->labelTitle->setText("Pending Incoming Payment Requests");
}

void TransactionsHistoryPage::showOutgoingRequests()
{
    if (!m_currentUser) {
        return;
    }
    
    // Set the view to payment requests
    setupRequestTable();
    clearTable();
    
    // Get user's pending outgoing requests
    std::vector<PaymentRequest> outgoingRequests = m_currentUser->getPendingOutgoingRequests();
    
    // Display all outgoing requests
    for (const auto& request : outgoingRequests) {
        addRequestToTable(request);
    }
    
    // Update title
    ui->labelTitle->setText("Pending Outgoing Payment Requests");
}

void TransactionsHistoryPage::on_pushButtonBackToHome_clicked()
{
    emit navigateToHomeRequested();
}

void TransactionsHistoryPage::on_pushButtonViewAllTransactions_clicked()
{
    showAllTransactions();
}

void TransactionsHistoryPage::on_pushButtonViewIncomingRequests_clicked()
{
    showIncomingRequests();
}

void TransactionsHistoryPage::on_pushButtonViewOutgoingRequests_clicked()
{
    showOutgoingRequests();
}
// The errors are caused because `labelTitle` and `tableWidgetTransactions` are not members of the `Ui::TransactionsHistoryPage` class.
// This usually means that these widgets do not exist in your .ui file (designed in Qt Designer), or they are named differently there.
// To fix this:
// 1. Open your TransactionsHistoryPage.ui file in Qt Designer.
// 2. Make sure you have a QLabel named `labelTitle` and a QTableWidget named `tableWidgetTransactions`.
// 3. If they do not exist, add them and set their objectName properties to exactly `labelTitle` and `tableWidgetTransactions`.
// 4. Save the .ui file and re-run qmake/CMake and rebuild your project so the generated ui_TransactionsHistoryPage.h is updated.
// 5. If the names are different, either rename them in the .ui file or update your C++ code to use the correct names.

// Example: In Qt Designer, select the table widget and set its objectName to "tableWidgetTransactions" in the Property Editor.
// Do the same for the label, setting its objectName to "labelTitle".

#include "stdafx.h"
#include "TransactionsDialog.h"
#include <QHeaderView>

TransactionsDialog::TransactionsDialog(const std::vector<Transaction*>& transactions, 
                                     const QString& title,
                                     QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(title);
    setupUI();
    populateTable(transactions);
    resize(800, 400); // Set a reasonable default size
}

TransactionsDialog::~TransactionsDialog()
{
    // Qt will handle cleaning up the UI components
}

void TransactionsDialog::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    // Create table widget
    tableTransactions = new QTableWidget(this);
    tableTransactions->setColumnCount(6);
    tableTransactions->setHorizontalHeaderLabels({"ID", "Amount", "From", "To", "Type", "Status"});
    tableTransactions->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableTransactions->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableTransactions->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    // Create close button
    closeButton = new QPushButton("Close", this);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    
    // Add widgets to layout
    layout->addWidget(tableTransactions);
    layout->addWidget(closeButton);
}

void TransactionsDialog::populateTable(const std::vector<Transaction*>& transactions)
{
    tableTransactions->setRowCount(0);
    
    for (const auto& transaction : transactions) {
        int row = tableTransactions->rowCount();
        tableTransactions->insertRow(row);
        
        tableTransactions->setItem(row, 0, new QTableWidgetItem(QString::number(transaction->getId())));
        tableTransactions->setItem(row, 1, new QTableWidgetItem(QString::number(transaction->getAmount())));
        tableTransactions->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(transaction->getSender())));
        tableTransactions->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(transaction->getRecipient())));
        tableTransactions->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(transaction->getType())));
        tableTransactions->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(transaction->getStatus())));
    }
}

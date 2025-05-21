#include "TransactionsHistoryPage.h"
#include "ui_TransactionsHistoryPage.h"
#include "../core/User.h"         // For User class
#include "../core/Transaction.h"  // For Transaction class
#include "../core/PaymentRequest.h" // For PaymentRequest class
#include <QMessageBox>          // For potential error messages
#include <algorithm>            // For std::sort or std::min
#include <iostream>             // For debugging output

TransactionsHistoryPage::TransactionsHistoryPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransactionsHistoryPage),
    m_currentUser(nullptr)
{
    ui->setupUi(this);

    // Connect button signals to their respective slots (as per plan.md section 2.2)
    connect(ui->pushButtonViewAllTransactions, &QPushButton::clicked, this, &TransactionsHistoryPage::on_pushButtonViewAllTransactions_clicked);
    connect(ui->pushButtonViewIncomingRequests, &QPushButton::clicked, this, &TransactionsHistoryPage::on_pushButtonViewIncomingRequests_clicked);
    connect(ui->pushButtonViewOutgoingRequests, &QPushButton::clicked, this, &TransactionsHistoryPage::on_pushButtonViewOutgoingRequests_clicked);
    connect(ui->pushButtonBackToHome, &QPushButton::clicked, this, &TransactionsHistoryPage::on_pushButtonBackToHome_clicked);

    // Initial table setup (though displayRecentTransactions will also call it)
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
        displayRecentTransactions(); // Show initial view (plan.md section 2.3)
    } else {
        // Handle case where user is null, e.g., clear table and show a message
        clearTable();
        ui->labelTitle->setText("No user data loaded.");
        // Optionally disable buttons if no user is loaded
    }
}

void TransactionsHistoryPage::setupTransactionTable()
{
    ui->tableWidgetTransactions->setColumnCount(7); // Added "Type" column
    ui->tableWidgetTransactions->setHorizontalHeaderLabels(
        {"ID", "Date", "Amount", "Sender", "Recipient", "Status", "Type"}); // Plan.md section 2.4
    
    // Set column widths (adjust as needed)
    ui->tableWidgetTransactions->setColumnWidth(0, 50);   // ID
    ui->tableWidgetTransactions->setColumnWidth(1, 150);  // Date
    ui->tableWidgetTransactions->setColumnWidth(2, 100);  // Amount
    ui->tableWidgetTransactions->setColumnWidth(3, 120);  // Sender
    ui->tableWidgetTransactions->setColumnWidth(4, 120);  // Recipient
    ui->tableWidgetTransactions->setColumnWidth(5, 100);  // Status
    ui->tableWidgetTransactions->setColumnWidth(6, 80);   // Type
    
    ui->tableWidgetTransactions->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetTransactions->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetTransactions->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidgetTransactions->horizontalHeader()->setStretchLastSection(true);
}

void TransactionsHistoryPage::setupRequestTable()
{
    ui->tableWidgetTransactions->setColumnCount(7); // "ID", "Date", "Amount", "From", "To", "Message", "Status"
    ui->tableWidgetTransactions->setHorizontalHeaderLabels(
        {"ID", "Date", "Amount", "From", "To", "Message", "Status"}); // Plan.md section 2.4

    // Set column widths (adjust as needed)
    ui->tableWidgetTransactions->setColumnWidth(0, 50);   // ID
    ui->tableWidgetTransactions->setColumnWidth(1, 150);  // Date
    ui->tableWidgetTransactions->setColumnWidth(2, 100);  // Amount
    ui->tableWidgetTransactions->setColumnWidth(3, 120);  // From
    ui->tableWidgetTransactions->setColumnWidth(4, 120);  // To
    ui->tableWidgetTransactions->setColumnWidth(5, 180);  // Message
    ui->tableWidgetTransactions->setColumnWidth(6, 100);  // Status

    ui->tableWidgetTransactions->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetTransactions->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetTransactions->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidgetTransactions->horizontalHeader()->setStretchLastSection(true);
}

void TransactionsHistoryPage::clearTable()
{
    ui->tableWidgetTransactions->setRowCount(0); // Plan.md section 2.4
}

void TransactionsHistoryPage::addTransactionToTable(Transaction* transaction)
{
    if (!transaction || !m_currentUser) return; // Basic safety check

    int row = ui->tableWidgetTransactions->rowCount();
    ui->tableWidgetTransactions->insertRow(row);

    // ID
    ui->tableWidgetTransactions->setItem(row, 0, new QTableWidgetItem(QString::number(transaction->getId())));
    // Date
    QDateTime dateTime = QDateTime::fromSecsSinceEpoch(transaction->getDate());
    ui->tableWidgetTransactions->setItem(row, 1, new QTableWidgetItem(dateTime.toString("yyyy-MM-dd hh:mm:ss")));
    // Amount
    ui->tableWidgetTransactions->setItem(row, 2, new QTableWidgetItem(QString::number(transaction->getAmount(), 'f', 2)));
    // Sender
    ui->tableWidgetTransactions->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(transaction->getSender())));
    // Recipient
    ui->tableWidgetTransactions->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(transaction->getRecipient())));
    // Status
    ui->tableWidgetTransactions->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(transaction->getStatus())));
    // Type
    ui->tableWidgetTransactions->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(transaction->getType())));

    // Color-coding (Plan.md section 2.4)
    QColor rowColor;
    if (transaction->getSender() == m_currentUser->getUsername()) {
        rowColor = QColor(255, 235, 235); // Light red for outgoing
    } else if (transaction->getRecipient() == m_currentUser->getUsername()) {
        rowColor = QColor(235, 255, 235); // Light green for incoming
    } else {
        rowColor = Qt::white; // Default or for transactions not directly involving the user (e.g. admin view)
    }

    for (int col = 0; col < ui->tableWidgetTransactions->columnCount(); ++col) {
        if (ui->tableWidgetTransactions->item(row, col)) {
            ui->tableWidgetTransactions->item(row, col)->setBackground(rowColor);
        }
    }
}

void TransactionsHistoryPage::addRequestToTable(const PaymentRequest& request) // Plan.md section 2.4
{
    if (!m_currentUser) return;

    int row = ui->tableWidgetTransactions->rowCount();
    ui->tableWidgetTransactions->insertRow(row);

    // ID
    ui->tableWidgetTransactions->setItem(row, 0, new QTableWidgetItem(QString::number(request.getId())));
    // Date
    QDateTime dateTime = QDateTime::fromSecsSinceEpoch(request.getDate());
    ui->tableWidgetTransactions->setItem(row, 1, new QTableWidgetItem(dateTime.toString("yyyy-MM-dd hh:mm:ss")));
    // Amount
    ui->tableWidgetTransactions->setItem(row, 2, new QTableWidgetItem(QString::number(request.getAmount(), 'f', 2)));
    // From (Sender)
    ui->tableWidgetTransactions->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(request.getSender())));
    // To (Recipient)
    ui->tableWidgetTransactions->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(request.getRecipient())));
    // Message
    ui->tableWidgetTransactions->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(request.getMessage())));
    // Status
    ui->tableWidgetTransactions->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(request.getStatus())));

    // Color-coding (similar logic can be applied if needed for requests)
    QColor rowColor;
    if (request.getSender() == m_currentUser->getUsername()) {
        rowColor = QColor(255, 245, 235); // Light orange for outgoing requests
    } else if (request.getRecipient() == m_currentUser->getUsername()) {
        rowColor = QColor(235, 245, 255); // Light blue for incoming requests
    } else {
        rowColor = Qt::white;
    }

    for (int col = 0; col < ui->tableWidgetTransactions->columnCount(); ++col) {
        if (ui->tableWidgetTransactions->item(row, col)) {
            ui->tableWidgetTransactions->item(row, col)->setBackground(rowColor);
        }
    }
}

void TransactionsHistoryPage::displayRecentTransactions() // Plan.md section 2.5
{
    if (!m_currentUser) {
        clearTable();
        ui->labelTitle->setText("No user data to display recent transactions.");
        return;
    }

    setupTransactionTable();
    clearTable();

    std::vector<Transaction*> transactions = m_currentUser->getHistoryTransactions();
    
    // Sort transactions by date in descending order (newest first)
    std::sort(transactions.begin(), transactions.end(), [](const Transaction* a, const Transaction* b) {
        return a->getDate() > b->getDate();
    });

    int count = std::min(static_cast<int>(transactions.size()), 5); // Display up to 5 recent transactions
    for (int i = 0; i < count; ++i) {
        addTransactionToTable(transactions[i]);
    }

    ui->labelTitle->setText("Recent Transactions");
}


void TransactionsHistoryPage::on_pushButtonViewAllTransactions_clicked() // Plan.md section 2.5 & 4.8
{
    if (!m_currentUser) {
        QMessageBox::warning(this, "No User", "No user data loaded. Cannot display transactions.");
        return;
    }

    setupTransactionTable();
    clearTable();

    std::vector<Transaction*> transactions = m_currentUser->getHistoryTransactions();

    // Sort transactions by date in descending order (newest first)
    std::sort(transactions.begin(), transactions.end(), [](const Transaction* a, const Transaction* b) {
        return a->getDate() > b->getDate(); // Newest first
    });

    for (Transaction* transaction : transactions) {
        addTransactionToTable(transaction);
    }

    ui->labelTitle->setText("All Completed Transactions");
}

void TransactionsHistoryPage::on_pushButtonViewIncomingRequests_clicked() // Plan.md section 2.5
{
    if (!m_currentUser) {
        QMessageBox::warning(this, "No User", "No user data loaded. Cannot display incoming requests.");
        return;
    }

    setupRequestTable();
    clearTable();
    
    std::vector<PaymentRequest> requests = m_currentUser->getPendingIncomingRequests();

    // Sort requests by date (e.g., newest first)
    std::sort(requests.begin(), requests.end(), [](const PaymentRequest& a, const PaymentRequest& b) {
        return a.getDate() > b.getDate();
    });

    for (const auto& request : requests) {
        addRequestToTable(request);
    }

    ui->labelTitle->setText("Pending Incoming Payment Requests");
}

void TransactionsHistoryPage::on_pushButtonViewOutgoingRequests_clicked() // Plan.md section 2.5
{
    if (!m_currentUser) {
        QMessageBox::warning(this, "No User", "No user data loaded. Cannot display outgoing requests.");
        return;
    }

    setupRequestTable();
    clearTable();

    std::vector<PaymentRequest> requests = m_currentUser->getPendingOutgoingRequests();

    // Sort requests by date (e.g., newest first)
    std::sort(requests.begin(), requests.end(), [](const PaymentRequest& a, const PaymentRequest& b) {
        return a.getDate() > b.getDate();
    });

    for (const auto& request : requests) {
        addRequestToTable(request);
    }

    ui->labelTitle->setText("Pending Outgoing Payment Requests");
}

void TransactionsHistoryPage::on_pushButtonBackToHome_clicked() // Plan.md section 2.5
{
    emit navigateToHomeRequested();
}

#include "TransactionsHistoryPage.h"
#include "ui_TransactionsHistoryPage.h"
#include "../core/User.h"         // For User class
#include "../core/Transaction.h"  // For Transaction class
#include "../core/PaymentRequest.h" // For PaymentRequest class
#include "../core/Database.h" // Required for database interactions
#include <QMessageBox>          // For potential error messages
#include <algorithm>            // For std::sort or std::min
#include <iostream>             // For debugging output
#include <QPushButton>          // For action buttons
#include <QHBoxLayout>          // For layout in cell
#include <QSignalMapper>        // Alternative for button signals, but lambda is preferred
#include <random> // For transaction ID generation
#include <limits> // For transaction ID generation
#include <optional>

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
    ui->tableWidgetTransactions->setColumnCount(8); // ID, Date, Amount, From, To, Message, Status, Actions
    ui->tableWidgetTransactions->setHorizontalHeaderLabels(
        {"ID", "Date", "Amount", "From", "To", "Message", "Status", "Actions"});

    ui->tableWidgetTransactions->setColumnWidth(0, 50);   // ID
    ui->tableWidgetTransactions->setColumnWidth(1, 150);  // Date
    ui->tableWidgetTransactions->setColumnWidth(2, 80);  // Amount
    ui->tableWidgetTransactions->setColumnWidth(3, 100);  // From
    ui->tableWidgetTransactions->setColumnWidth(4, 100);  // To
    ui->tableWidgetTransactions->setColumnWidth(5, 150);  // Message
    ui->tableWidgetTransactions->setColumnWidth(6, 80);   // Status
    ui->tableWidgetTransactions->setColumnWidth(7, 150);  // Actions (for two buttons)

    ui->tableWidgetTransactions->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetTransactions->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetTransactions->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidgetTransactions->horizontalHeader()->setStretchLastSection(false); // Actions column fixed width
    if (ui->tableWidgetTransactions->columnCount() > 7) { // Last real data column stretch
        ui->tableWidgetTransactions->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch); 
    }
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

void TransactionsHistoryPage::addRequestToTable(const PaymentRequest& request) // Modified for Phase 1
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

    // Create Accept and Decline buttons for the "Actions" column (column 7)
    QPushButton *acceptButton = new QPushButton("Accept");
    QPushButton *declineButton = new QPushButton("Decline");
    
    // Store request ID for use in lambda, or use row if requests in table match user's list order
    int requestId = request.getId(); 

    connect(acceptButton, &QPushButton::clicked, this, [this, requestId]() {
        this->handleAcceptRequest(requestId);
    });
    connect(declineButton, &QPushButton::clicked, this, [this, requestId]() {
        this->handleDeclineRequest(requestId);
    });

    QWidget *actionsWidget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(actionsWidget);
    layout->addWidget(acceptButton);
    layout->addWidget(declineButton);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(5);
    actionsWidget->setLayout(layout);

    ui->tableWidgetTransactions->setCellWidget(row, 7, actionsWidget);

    QColor rowColor;
    bool isIncoming = request.getRecipient() == m_currentUser->getUsername();
    if (request.getSender() == m_currentUser->getUsername()) {
        rowColor = QColor(255, 245, 235);
        // Disable buttons if it's an outgoing request displayed in this view for some reason
        // Or ensure this view ONLY shows incoming requests where actions are relevant.
        // Assuming on_pushButtonViewIncomingRequests_clicked filters correctly.
        acceptButton->setEnabled(false);
        declineButton->setEnabled(false);
    } else if (isIncoming) {
        rowColor = QColor(235, 245, 255);
        // Buttons should be enabled only if the request status is "Pending"
        if (QString::fromStdString(request.getStatus()).toLower() != "pending") {
            acceptButton->setEnabled(false);
            declineButton->setEnabled(false);
        }
    } else {
        rowColor = Qt::white;
        acceptButton->setEnabled(false);
        declineButton->setEnabled(false);
    }

    for (int col = 0; col < 7; ++col) { // Don't color the actions widget cell background itself typically
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

void TransactionsHistoryPage::on_pushButtonViewIncomingRequests_clicked() // Modified for Phase 1
{
    if (!m_currentUser) {
        QMessageBox::warning(this, "No User", "No user data loaded. Cannot display incoming requests.");
        return;
    }
    setupRequestTable(); // Switch to request table format with Actions column
    clearTable();
    std::vector<PaymentRequest> requests = m_currentUser->getPendingIncomingRequests();
    std::sort(requests.begin(), requests.end(), [](const PaymentRequest& a, const PaymentRequest& b) {
        return a.getDate() > b.getDate();
    });
    for (const auto& request : requests) {
        if (request.getRecipient() == m_currentUser->getUsername()) { // Ensure only truly incoming are shown for action
             addRequestToTable(request);
        }
    }
    ui->labelTitle->setText("Pending Incoming Payment Requests");
}

void TransactionsHistoryPage::on_pushButtonViewOutgoingRequests_clicked() // Modified for Phase 1
{
    if (!m_currentUser) {
        QMessageBox::warning(this, "No User", "No user data loaded. Cannot display outgoing requests.");
        return;
    }
    setupRequestTable(); // Use request table format, but actions will be disabled
    clearTable();
    std::vector<PaymentRequest> requests = m_currentUser->getPendingOutgoingRequests();
    std::sort(requests.begin(), requests.end(), [](const PaymentRequest& a, const PaymentRequest& b) {
        return a.getDate() > b.getDate();
    });
    for (const auto& request : requests) {
         if (request.getSender() == m_currentUser->getUsername()) { // Ensure only truly outgoing are shown
            addRequestToTable(request); // Buttons will be disabled by logic in addRequestToTable
        }
    }
    ui->labelTitle->setText("Pending Outgoing Payment Requests");
}

void TransactionsHistoryPage::on_pushButtonBackToHome_clicked() // Plan.md section 2.5
{
    emit navigateToHomeRequested();
}

// Placeholder for slots to be implemented in Phase 3 and 4
void TransactionsHistoryPage::handleAcceptRequest(int requestId) {
    if (!m_currentUser) {
        QMessageBox::warning(this, "Error", "No user logged in.");
        return;
    }

    Database* db = Database::getInstance();

    std::optional<PaymentRequest> requestToProcessOpt;
    const auto& incomingRequests = m_currentUser->getPendingIncomingRequests();
    for (const auto& req : incomingRequests) {
        if (req.getId() == requestId) {
            requestToProcessOpt = req;
            break;
        }
    }

    if (!requestToProcessOpt) {
        QMessageBox::warning(this, "Error", "Payment request not found or already processed.");
        on_pushButtonViewIncomingRequests_clicked();
        return;
    }
    
    PaymentRequest requestToProcess = *requestToProcessOpt;

    if (requestToProcess.getStatus() != "Pending" && requestToProcess.getStatus() != "pending") {
         QMessageBox::information(this, "Information", "This request is no longer pending.");
         on_pushButtonViewIncomingRequests_clicked();
         return;
    }

    if (m_currentUser->getBalance() < requestToProcess.getAmount()) {
        QMessageBox::warning(this, "Insufficient Balance", "You do not have enough funds to accept this request.");
        return;
    }

    Account* payeeAccount = db->getAccount(requestToProcess.getSender());
    if (!payeeAccount || payeeAccount->getAccountType() != "User") {
        QMessageBox::critical(this, "Error", "Recipient user account not found or is not a user type.");
        return;
    }
    User* payee = static_cast<User*>(payeeAccount);
    
    try {
        m_currentUser->setBalance(m_currentUser->getBalance() - requestToProcess.getAmount());
        payee->setBalance(payee->getBalance() + requestToProcess.getAmount());

        static thread_local std::mt19937_64 rng(std::random_device{}());
        std::uniform_int_distribution<long long> dist(1, std::numeric_limits<long long>::max());
        long long rawTxId = dist(rng);
        int transactionId = static_cast<int>(rawTxId % std::numeric_limits<int>::max());
        if (transactionId == 0) transactionId = 1; 

        Transaction* newTransaction = new Transaction(
            transactionId, 
            requestToProcess.getAmount(), 
            m_currentUser->getUsername(), 
            payee->getUsername(),         
            "Completed", 
            "Payment",                  
            "Request Fulfillment"       
        );

        db->addTransaction(newTransaction);        
        m_currentUser->addTransaction(newTransaction); 
        payee->addTransaction(newTransaction);         

        bool removedFromPayer = m_currentUser->removePendingIncomingRequest(requestId);
        bool removedFromPayee = payee->removePendingOutgoingRequest(requestId);

        if (!removedFromPayer) {
            qWarning() << "Could not remove accepted request ID:" << requestId << "from payer's list.";
        }
        if (!removedFromPayee) {
            qWarning() << "Could not remove accepted request ID:" << requestId << "from payee's list.";
        }

        payee->addNotificationTransaction(newTransaction); 

        QMessageBox::information(this, "Request Accepted", "Payment of " + QString::number(requestToProcess.getAmount(), 'f', 2) + " to " + QString::fromStdString(payee->getUsername()) + " successful.");
    
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Transaction Error", QString("An error occurred: ") + e.what());
    } catch (...) {
        QMessageBox::critical(this, "Transaction Error", "An unknown error occurred during the transaction.");
    }

    on_pushButtonViewIncomingRequests_clicked();
}

void TransactionsHistoryPage::handleDeclineRequest(int requestId) {
    if (!m_currentUser) {
        QMessageBox::warning(this, "Error", "No user logged in.");
        return;
    }

    Database* db = Database::getInstance();

    // Find the details of the request to get the sender (Payee)
    std::string payeeUsername;
    bool foundRequest = false;
    const auto& incomingRequests = m_currentUser->getPendingIncomingRequests();
    for (const auto& req : incomingRequests) {
        if (req.getId() == requestId) {
            if (req.getStatus() != "Pending" && req.getStatus() != "pending") {
                QMessageBox::information(this, "Information", "This request is no longer pending and cannot be declined.");
                on_pushButtonViewIncomingRequests_clicked();
                return;
            }
            payeeUsername = req.getSender();
            foundRequest = true;
            break;
        }
    }

    if (!foundRequest) {
        QMessageBox::warning(this, "Error", "Payment request not found or already processed.");
        on_pushButtonViewIncomingRequests_clicked(); // Refresh view
        return;
    }

    // Remove from current user's (Payer's) incoming requests
    bool removedFromPayer = m_currentUser->removePendingIncomingRequest(requestId);
    if (!removedFromPayer) {
        QMessageBox::warning(this, "Error", "Failed to remove request from your list. It might have been already processed.");
        // Still try to remove from payee if possible, but log this issue
        qWarning() << "handleDeclineRequest: Failed to remove request ID" << requestId << "from current user (Payer) list.";
    } else {
         QMessageBox::information(this, "Request Declined", "The payment request has been declined.");
    }

    // Get Payee User and remove from their outgoing requests
    if (!payeeUsername.empty()) {
        Account* payeeAccount = db->getAccount(payeeUsername);
        if (payeeAccount && payeeAccount->getAccountType() == "User") {
            User* payee = static_cast<User*>(payeeAccount);
            bool removedFromPayee = payee->removePendingOutgoingRequest(requestId);
            if (!removedFromPayee) {
                qWarning() << "handleDeclineRequest: Could not find/remove request ID" << requestId << "from payee" << QString::fromStdString(payeeUsername) << "outgoing list.";
                // This is not necessarily an error for the current user if their side was successful.
            }
            // Optionally, notify the payee that their request was declined
            // payee->addNotification(new Notification(...)); // Requires a new Notification constructor/type
        } else {
            qWarning() << "handleDeclineRequest: Payee user" << QString::fromStdString(payeeUsername) << "not found or not a User type for request ID" << requestId;
        }
    }

    on_pushButtonViewIncomingRequests_clicked(); // Refresh the view
}

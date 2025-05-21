#include "SendMoneyPage.h"
#include "ui_SendMoneyPage.h"
#include <sstream>
#include <iomanip>

SendMoneyPage::SendMoneyPage(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::SendMoneyPage)
{
    ui->setupUi(this);

    // Get database instance
    database = Database::getInstance();

    // Connect signals and slots
    connect(ui->pushButtonSend, &QPushButton::clicked, this, &SendMoneyPage::on_pushButtonSend_clicked);

    // Update balance display when the page is created
    updateBalanceDisplay();
}

SendMoneyPage::~SendMoneyPage()
{
    delete ui;
}

void SendMoneyPage::updateBalanceDisplay()
{
    // Get current user from database
    Account* currentAccount = database->getCurrentAccount();

    if (currentAccount && currentAccount->getAccountType() == "User") {
        User* currentUser = dynamic_cast<User*>(currentAccount);
        if (currentUser) {
            // Format balance with 2 decimal places
            std::stringstream stream;
            stream << std::fixed << std::setprecision(2) << currentUser->getBalance();
            ui->labelBalanceValue->setText(QString::fromStdString(stream.str()));
        }
    }
}

void SendMoneyPage::on_pushButtonBackToHome_clicked()
{
    emit navigateToHomeRequested();
}

void SendMoneyPage::on_pushButtonSend_clicked()
{
    // Clear previous status
    ui->labelStatus->setText("");

    // Get recipient username
    QString recipientUsername = ui->lineEditRecipient->text();
    if (recipientUsername.isEmpty()) {
        ui->labelStatus->setText("Please enter a recipient username");
        return;
    }

    // Get amount
    double amount = ui->doubleSpinBoxAmount->value();
    if (amount <= 0) {
        ui->labelStatus->setText("Amount must be greater than 0");
        return;
    }

    // Get current user
    Account* currentAccount = database->getCurrentAccount();
    if (!currentAccount || currentAccount->getAccountType() != "User") {
        ui->labelStatus->setText("You must be logged in as a user to send money");
        return;
    }

    User* sender = dynamic_cast<User*>(currentAccount);
    if (!sender) {
        ui->labelStatus->setText("Error getting current user");
        return;
    }

    // Check if sender has sufficient balance
    if (sender->getBalance() < amount) {
        ui->labelStatus->setText("Insufficient balance");
        return;
    }

    // Get recipient user
    Account* recipientAccount = database->getAccount(recipientUsername.toStdString());
    if (!recipientAccount || recipientAccount->getAccountType() != "User") {
        ui->labelStatus->setText("Recipient not found or not a user account");
        return;
    }

    User* recipient = dynamic_cast<User*>(recipientAccount);
    if (!recipient) {
        ui->labelStatus->setText("Error getting recipient user");
        return;
    }

    // Check if recipient account is active
    if (!recipient->isActive()) {
        ui->labelStatus->setText("Recipient account is suspended");
        return;
    }

    try {
        // Create transaction ID (using a simple approach here)
        
        static thread_local std::mt19937_64 rng(std::random_device{}());
        std::uniform_int_distribution<long long> dist(1, std::numeric_limits<long long>::max());
        long long transactionId = dist(rng);
        // Create a new transaction
        Transaction* transaction = new Transaction(
            transactionId++,
            amount,
            sender->getUsername(),
            recipient->getUsername(),
            "Completed",
            "Transfer",
            "Send"
        );

        // Process the transaction
		try {
			sender->setBalance(sender->getBalance() - amount);
			recipient->setBalance(recipient->getBalance() + amount);
		}
		catch (const std::exception& e) {
			ui->labelStatus->setText(QString("Transaction failed: ") + e.what());
			delete transaction; // Clean up
			return;
		}

        // Add the transaction to the database
        database->addTransaction(transaction);
		sender->addTransaction(transaction);
		recipient->addTransaction(transaction);

        // Add notification to recipient
        recipient->addNotificationTransaction(transaction);

        // Update balance display
        updateBalanceDisplay();

        // Show success message
        ui->labelStatus->setText("Money sent successfully!");

        // Clear recipient field and reset amount
        ui->lineEditRecipient->clear();
        ui->doubleSpinBoxAmount->setValue(0.01);

        // Emit transaction completed signal
        emit transactionCompleted(transaction);

    }
    catch (const std::exception& e) {
        // Show error message
        ui->labelStatus->setText(QString("Error: ") + e.what());
    }
}

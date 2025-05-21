#include "RequestMoney.h"
#include "ui_RequestMoney.h"
#include "core/Account.h"
#include "core/User.h"
#include "core/Database.h"
#include "core/PaymentRequest.h"
#include <sstream>
#include <iomanip>
#include <random>
#include <limits>
#include <QDebug>

RequestMoney::RequestMoney(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RequestMoney)
{
    ui->setupUi(this);
    database = Database::getInstance();

    connect(ui->pushButtonSendRequest, &QPushButton::clicked, this, &RequestMoney::on_pushButtonSendRequest_clicked);
}

RequestMoney::~RequestMoney()
{
    delete ui;
}

void RequestMoney::on_pushButtonBackToHome_clicked()
{
    emit navigateToHomeRequested();
}

void RequestMoney::on_pushButtonSendRequest_clicked()
{
    ui->labelStatus->setText("");

    QString targetUsername = ui->lineEditTargetUser->text().trimmed();
    if (targetUsername.isEmpty()) {
        ui->labelStatus->setText("Please enter a target username.");
        return;
    }

    double amount = ui->doubleSpinBoxAmount->value();
    if (amount <= 0) {
        ui->labelStatus->setText("Amount must be greater than 0.");
        return;
    }

    QString message = ui->lineEditMessage->text().trimmed();

    Account* currentAccount = database->getCurrentAccount();
    if (!currentAccount) {
        ui->labelStatus->setText("Error: No user is currently logged in.");
        qWarning() << "RequestMoney: No current account found.";
        return;
    }
    if (currentAccount->getAccountType() != "User") {
        ui->labelStatus->setText("Only users can send payment requests.");
        qWarning() << "RequestMoney: Current account is not a User type:" << QString::fromStdString(currentAccount->getAccountType());
        return;
    }
    User* requester = dynamic_cast<User*>(currentAccount);
    if (!requester) {
        ui->labelStatus->setText("Error processing your user account.");
        qWarning() << "RequestMoney: Failed to cast current account to User.";
        return;
    }

    Account* targetAccount = database->getAccount(targetUsername.toStdString());
    if (!targetAccount) {
        ui->labelStatus->setText("Target user not found.");
        return;
    }
    if (targetAccount->getAccountType() != "User") {
        ui->labelStatus->setText("Target account is not a user account.");
        return;
    }
    User* payer = dynamic_cast<User*>(targetAccount);
    if (!payer) {
        ui->labelStatus->setText("Error processing target user account.");
        return;
    }

    if (!payer->isActive()) {
        ui->labelStatus->setText("Target user account is suspended.");
        return;
    }

    if (requester->getUsername() == payer->getUsername()) {
        ui->labelStatus->setText("You cannot send a payment request to yourself.");
        return;
    }

    try {
        static thread_local std::mt19937_64 rng(std::random_device{}());
        std::uniform_int_distribution<long long> dist(1, std::numeric_limits<long long>::max());
        long long paymentRequestIdRaw = dist(rng);
        int paymentRequestId = static_cast<int>(paymentRequestIdRaw % std::numeric_limits<int>::max());
        if (paymentRequestId == 0) paymentRequestId = 1;

        PaymentRequest* request = new PaymentRequest(
            paymentRequestId,
            amount,
            requester->getUsername(),
            payer->getUsername(),
            "Pending",
            message.toStdString()
        );

        requester->addPendingOutgoingRequest(*request);
        payer->addPendingIncomingRequest(*request);
        payer->addNotificationRequest(request);

        ui->labelStatus->setText("Payment request sent successfully!");

        ui->lineEditTargetUser->clear();
        ui->doubleSpinBoxAmount->setValue(ui->doubleSpinBoxAmount->minimum());
        ui->lineEditMessage->clear();

        emit paymentRequestSent(request);

    } catch (const std::exception& e) {
        ui->labelStatus->setText(QString("Error: ") + e.what());
        qWarning() << "RequestMoney: Exception during payment request: " << e.what();
    } catch (...) {
        ui->labelStatus->setText("An unknown error occurred.");
        qWarning() << "RequestMoney: Unknown exception during payment request.";
    }
} 

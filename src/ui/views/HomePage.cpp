#include "HomePage.h"
#include "ui_HomePage.h"

HomePage::HomePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomePage)
{
    ui->setupUi(this);
}

HomePage::~HomePage()
{
    delete ui;
}

void HomePage::setUserBalance(const QString& balance)
{
    ui->labelBalanceAmount->setText(balance);
}

void HomePage::on_pushButtonSendPaymentRequest_clicked()
{
    emit navigateToSendPaymentRequestRequested();
}

void HomePage::on_pushButtonSendMoney_clicked()
{
    emit navigateToSendMoneyRequested();
}

void HomePage::on_pushButtonShowTransactions_clicked()
{
    emit navigateToTransactionsHistoryRequested();
} 
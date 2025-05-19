#include "HomePage.h"
#include "ui_HomePage.h"

HomePage::HomePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomePage),
    m_accountType("")
{
    ui->setupUi(this);
    // Hide admin button by default
    ui->pushButtonAdmin->setVisible(false);
}

HomePage::~HomePage()
{
    delete ui;
}

void HomePage::setUserBalance(const QString& balance)
{
    ui->labelBalanceAmount->setText(balance);
}

void HomePage::setCurrentAccountType(const QString& accountType)
{
    m_accountType = accountType;
    
    // Show admin button only if the account type is "Admin"
    bool isAdmin = (accountType == "Admin");
    ui->pushButtonAdmin->setVisible(isAdmin);
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

void HomePage::on_pushButtonAdmin_clicked()
{
    emit navigateToAdminRequested();
} 
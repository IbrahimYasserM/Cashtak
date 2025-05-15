#include "SendMoneyPage.h"
#include "ui_SendMoneyPage.h"

SendMoneyPage::SendMoneyPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SendMoneyPage)
{
    ui->setupUi(this);
}

SendMoneyPage::~SendMoneyPage()
{
    delete ui;
}

void SendMoneyPage::on_pushButtonBackToHome_clicked()
{
    emit navigateToHomeRequested();
} 
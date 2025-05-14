#include "SendPaymentRequestPage.h"
#include "ui_SendPaymentRequestPage.h"

SendPaymentRequestPage::SendPaymentRequestPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SendPaymentRequestPage)
{
    ui->setupUi(this);
}

SendPaymentRequestPage::~SendPaymentRequestPage()
{
    delete ui;
}

void SendPaymentRequestPage::on_pushButtonBackToHome_clicked()
{
    emit navigateToHomeRequested();
} 
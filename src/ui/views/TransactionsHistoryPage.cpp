#include "TransactionsHistoryPage.h"
#include "ui_TransactionsHistoryPage.h"

TransactionsHistoryPage::TransactionsHistoryPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransactionsHistoryPage)
{
    ui->setupUi(this);
}

TransactionsHistoryPage::~TransactionsHistoryPage()
{
    delete ui;
}

void TransactionsHistoryPage::on_pushButtonBackToHome_clicked()
{
    emit navigateToHomeRequested();
} 
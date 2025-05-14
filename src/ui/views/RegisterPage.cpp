#include "RegisterPage.h"
#include "ui_RegisterPage.h"

RegisterPage::RegisterPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterPage)
{
    ui->setupUi(this);
}

RegisterPage::~RegisterPage()
{
    delete ui;
}

void RegisterPage::on_pushButtonRegister_clicked()
{
    // Add validation for passwords matching, etc., before emitting
    if (ui->lineEditPassword->text() == ui->lineEditConfirmPassword->text()) {
        emit registerRequested(ui->lineEditUsername->text(), ui->lineEditPassword->text());
    } else {
        // Handle password mismatch (e.g., show a message)
        // For now, just not emitting the signal
    }
}

void RegisterPage::on_pushButtonGoToLogin_clicked()
{
    emit navigateToLoginRequested();
} 
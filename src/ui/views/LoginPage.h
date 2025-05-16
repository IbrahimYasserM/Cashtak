#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>
#include "../../core/Account.h"

// Forward declaration of the UI class
namespace Ui {
class LoginPage;
}

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();

signals:
    void navigateToRegisterRequested();
    void loginRequested(Account* account); // Updated to pass the account pointer

private slots:
    void on_pushButtonLogin_clicked();
    void on_pushButtonGoToRegister_clicked();

private:
    Ui::LoginPage *ui;
};

#endif // LOGINPAGE_H 
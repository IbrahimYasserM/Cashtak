#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include "../core/Account.h"

// Forward declare your new page classes
class LoginPage;
class RegisterPage;
class HomePage;
class RequestMoney;
class SendMoneyPage;
class TransactionsHistoryPage;
class AdminPage;

class CashatakMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CashatakMainWindow(QWidget *parent = nullptr);
    ~CashatakMainWindow();

public slots:
    void navigateToLogin();
    void navigateToRegister();
    void navigateToHome();
    void navigateToSendPaymentRequest();
    void navigateToSendMoney();
    void navigateToTransactionsHistory();
    void navigateToAdmin();

    // Add a slot to handle successful login
    void handleLoginSuccess(Account* account);

private:
    QStackedWidget *m_stackedWidget;
    LoginPage *m_loginPage;
    RegisterPage *m_registerPage;
    HomePage *m_homePage;
    RequestMoney *m_requestMoneyPage;
    SendMoneyPage *m_sendMoneyPage;
    TransactionsHistoryPage *m_transactionsHistoryPage;
    AdminPage *m_adminPage;
    
    Account* m_currentAccount; // Store the current logged-in account

    QWidget* centralContainer = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralContainer);

    void setupConnections();
};

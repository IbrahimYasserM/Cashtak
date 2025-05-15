#pragma once

#include <QMainWindow>
#include <QStackedWidget>

// Forward declare your new page classes
class LoginPage;
class RegisterPage;
class HomePage;
class SendPaymentRequestPage;
class SendMoneyPage;
class TransactionsHistoryPage;

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

    // Example slot for when login is successful (to be called by controller later)
    // void handleLoginSuccess(const QString& username);

private:
    QStackedWidget *m_stackedWidget;
    LoginPage *m_loginPage;
    RegisterPage *m_registerPage;
    HomePage *m_homePage;
    SendPaymentRequestPage *m_sendPaymentRequestPage;
    SendMoneyPage *m_sendMoneyPage;
    TransactionsHistoryPage *m_transactionsHistoryPage;

    void setupConnections();
};

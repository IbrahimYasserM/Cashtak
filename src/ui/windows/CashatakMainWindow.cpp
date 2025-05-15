#include "stdafx.h"
#include "CashatakMainWindow.h"
#include "../views/LoginPage.h"             // Adjusted path
#include "../views/RegisterPage.h"          // Adjusted path
#include "../views/HomePage.h"              // Adjusted path
#include "../views/SendPaymentRequestPage.h"// Adjusted path
#include "../views/SendMoneyPage.h"         // Adjusted path
#include "../views/TransactionsHistoryPage.h"// Adjusted path
#include <QVBoxLayout> // For setting layout if main window doesn't have a .ui

CashatakMainWindow::CashatakMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_stackedWidget = new QStackedWidget(this);

    m_loginPage = new LoginPage(this);
    m_registerPage = new RegisterPage(this);
    m_homePage = new HomePage(this);
    m_sendPaymentRequestPage = new SendPaymentRequestPage(this);
    m_sendMoneyPage = new SendMoneyPage(this);
    m_transactionsHistoryPage = new TransactionsHistoryPage(this);

    m_stackedWidget->addWidget(m_loginPage);
    m_stackedWidget->addWidget(m_registerPage);
    m_stackedWidget->addWidget(m_homePage);
    m_stackedWidget->addWidget(m_sendPaymentRequestPage);
    m_stackedWidget->addWidget(m_sendMoneyPage);
    m_stackedWidget->addWidget(m_transactionsHistoryPage);

    if (!centralWidget()) { // Check if a central widget is already set (e.g. by a .ui file)
        QWidget *centralContainer = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(centralContainer);
        layout->addWidget(m_stackedWidget);
        layout->setContentsMargins(0,0,0,0);
        setCentralWidget(centralContainer);
    }

    setupConnections();

    navigateToLogin(); // Start with the login page
    resize(500, 400); // Default size
    
    // Set the application icon
    QIcon icon(":/assets/images/logo.svg");
    setWindowIcon(icon);
}

CashatakMainWindow::~CashatakMainWindow()
{}

void CashatakMainWindow::setupConnections()
{
    // Login Page Connections
    connect(m_loginPage, &LoginPage::navigateToRegisterRequested, this, &CashatakMainWindow::navigateToRegister);
    // connect(m_loginPage, &LoginPage::loginRequested, this, &SomeController::handleLoginAttempt); // For later

    // Register Page Connections
    connect(m_registerPage, &RegisterPage::navigateToLoginRequested, this, &CashatakMainWindow::navigateToLogin);
    // connect(m_registerPage, &RegisterPage::registerRequested, this, &SomeController::handleRegistrationAttempt); // For later

    // Home Page Connections
    connect(m_homePage, &HomePage::navigateToSendPaymentRequestRequested, this, &CashatakMainWindow::navigateToSendPaymentRequest);
    connect(m_homePage, &HomePage::navigateToSendMoneyRequested, this, &CashatakMainWindow::navigateToSendMoney);
    connect(m_homePage, &HomePage::navigateToTransactionsHistoryRequested, this, &CashatakMainWindow::navigateToTransactionsHistory);
    // connect(m_homePage, &HomePage::logoutRequested, this, &CashatakMainWindow::navigateToLogin); // Example for logout

    // Send Payment Request Page Connections
    connect(m_sendPaymentRequestPage, &SendPaymentRequestPage::navigateToHomeRequested, this, &CashatakMainWindow::navigateToHome);

    // Send Money Page Connections
    connect(m_sendMoneyPage, &SendMoneyPage::navigateToHomeRequested, this, &CashatakMainWindow::navigateToHome);

    // Transactions History Page Connections
    connect(m_transactionsHistoryPage, &TransactionsHistoryPage::navigateToHomeRequested, this, &CashatakMainWindow::navigateToHome);
}

void CashatakMainWindow::navigateToLogin()
{
    m_stackedWidget->setCurrentWidget(m_loginPage);
}

void CashatakMainWindow::navigateToRegister()
{
    m_stackedWidget->setCurrentWidget(m_registerPage);
}

void CashatakMainWindow::navigateToHome()
{
    m_stackedWidget->setCurrentWidget(m_homePage);
}

void CashatakMainWindow::navigateToSendPaymentRequest()
{
    m_stackedWidget->setCurrentWidget(m_sendPaymentRequestPage);
}

void CashatakMainWindow::navigateToSendMoney()
{
    m_stackedWidget->setCurrentWidget(m_sendMoneyPage);
}

void CashatakMainWindow::navigateToTransactionsHistory()
{
    m_stackedWidget->setCurrentWidget(m_transactionsHistoryPage);
}

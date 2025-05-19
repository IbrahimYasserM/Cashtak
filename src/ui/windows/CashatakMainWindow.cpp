#include "stdafx.h"
#include "CashatakMainWindow.h"
#include "../views/LoginPage.h" 		// Adjusted path
#include "../views/RegisterPage.h"          // Adjusted path
#include "../views/HomePage.h"              // Adjusted path
#include "../views/SendPaymentRequestPage.h"// Adjusted path
#include "../views/SendMoneyPage.h"         // Adjusted path
#include "../views/TransactionsHistoryPage.h"// Adjusted path
#include "../views/AdminPage.h"            // Added AdminPage
#include <QVBoxLayout> // For setting layout if main window doesn't have a .ui
#include "../core/User.h" // For User class

CashatakMainWindow::CashatakMainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_currentAccount(nullptr)
{
    m_stackedWidget = new QStackedWidget(this);

    m_loginPage = new LoginPage(this);
    m_registerPage = new RegisterPage(this);
    m_homePage = new HomePage(this);
    m_sendPaymentRequestPage = new SendPaymentRequestPage(this);
    m_sendMoneyPage = new SendMoneyPage(this);
    m_transactionsHistoryPage = new TransactionsHistoryPage(this);
    m_adminPage = new AdminPage(this);

	m_stackedWidget->addWidget(m_loginPage);
	m_stackedWidget->addWidget(m_registerPage);
    m_stackedWidget->addWidget(m_homePage);
    m_stackedWidget->addWidget(m_sendPaymentRequestPage);
    m_stackedWidget->addWidget(m_sendMoneyPage);
    m_stackedWidget->addWidget(m_transactionsHistoryPage);
    m_stackedWidget->addWidget(m_adminPage);

    if (!centralWidget()) { // Check if a central widget is already set (e.g. by a .ui file)
        QWidget *centralContainer = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(centralContainer);
        QLabel* name = new QLabel("Cashatak", this);
        layout->addWidget(name);
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
{
    // The m_currentAccount should not be deleted here as it's managed by the Database
    m_currentAccount = nullptr;
}

void CashatakMainWindow::setupConnections()
{
    // Login Page Connections
    connect(m_loginPage, &LoginPage::navigateToRegisterRequested, this, &CashatakMainWindow::navigateToRegister);
    // Add connection for login success
    connect(m_loginPage, &LoginPage::loginRequested, this, &CashatakMainWindow::handleLoginSuccess);

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
    
    // Admin Page Connections
    connect(m_adminPage, &AdminPage::navigateToHomeRequested, this, &CashatakMainWindow::navigateToHome);
}

void CashatakMainWindow::handleLoginSuccess(Account* account)
{
    m_currentAccount = account;

    // Set the current account in the Database
    Database* db = Database::getInstance();
    db->setCurrentAccount(account);

    // Set up the home page with the current account information
    if (m_currentAccount) {
        // If account is a User, set their balance
        if (m_currentAccount->getAccountType() == "User") {
            User* user = dynamic_cast<User*>(m_currentAccount);
            m_homePage->setUserBalance(QString::asprintf("$%.2f", user->getBalance()));
            // Navigate to home page
            navigateToHome();
        }
        else if (m_currentAccount->getAccountType() == "Admin") {
            // Admin specific setup if needed
            navigateToAdmin();
        }
    }
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

void CashatakMainWindow::navigateToAdmin()
{
    m_stackedWidget->setCurrentWidget(m_adminPage);
}

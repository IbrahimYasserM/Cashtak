#include "stdafx.h"
#include "CashatakMainWindow.h"
#include "../views/LoginPage.h" 		// Adjusted path
#include "../views/RegisterPage.h"          // Adjusted path
#include "../views/HomePage.h"              // Adjusted path
#include "../views/RequestMoney.h"// Adjusted path
#include "../views/SendMoneyPage.h"         // Adjusted path
#include "../views/TransactionsHistoryPage.h"// Adjusted path
#include "../views/AdminPage.h"            // Added AdminPage
#include <QVBoxLayout> // For setting layout if main window doesn't have a .ui
#include <QSystemTrayIcon>
#include "../core/User.h" // For User class

CashatakMainWindow::CashatakMainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_currentAccount(nullptr)
{
    m_stackedWidget = new QStackedWidget(this);

    m_loginPage = new LoginPage(this);
    m_registerPage = new RegisterPage(this);
    m_homePage = new HomePage(this);
    m_requestMoneyPage = new RequestMoney(this);
    m_sendMoneyPage = new SendMoneyPage(this);
    m_transactionsHistoryPage = new TransactionsHistoryPage(this);
    m_adminPage = new AdminPage(this);

	m_stackedWidget->addWidget(m_loginPage);
	m_stackedWidget->addWidget(m_registerPage);
    m_stackedWidget->addWidget(m_homePage);
    m_stackedWidget->addWidget(m_requestMoneyPage);
    m_stackedWidget->addWidget(m_sendMoneyPage);
    m_stackedWidget->addWidget(m_transactionsHistoryPage);
    m_stackedWidget->addWidget(m_adminPage);

    if (!centralWidget()) { // Check if a central widget is already set (e.g. by a .ui file)
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
    connect(m_requestMoneyPage, &RequestMoney::navigateToHomeRequested, this, &CashatakMainWindow::navigateToHome);

    // Send Money Page Connections
    connect(m_sendMoneyPage, &SendMoneyPage::navigateToHomeRequested, this, &CashatakMainWindow::navigateToHome);

    // Transactions History Page Connections
    connect(m_transactionsHistoryPage, &TransactionsHistoryPage::navigateToHomeRequested, this, &CashatakMainWindow::navigateToHome);
    
    // Admin Page Connections
    connect(m_adminPage, &AdminPage::navigateToHomeRequested, this, &CashatakMainWindow::navigateToHome);
    connect(m_adminPage, &AdminPage::navigateToLoginRequested, this, &CashatakMainWindow::navigateToLogin);
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

            QHBoxLayout* headerLayout = new QHBoxLayout();
            QLabel* username = new QLabel(user->getUsername().c_str(), this);
            QLabel* email = new QLabel(user->getEmail().c_str(), this);
			username->setStyleSheet("font-weight: bold; font-size: 16px;");
			email->setStyleSheet("font-size: 14px;");
            headerLayout->addWidget(username);
            headerLayout->addWidget(email);
			QIcon logoutIcon("../../../../assets/images/logout.png");
            QPushButton* logoutButton = new QPushButton("", this);
			logoutButton->setIcon(logoutIcon);
			logoutButton->setIconSize(QSize(24, 24));
			logoutButton->setStyleSheet("border: none; background: transparent;");
            logoutButton->setMaximumSize(20, 20);
            headerLayout->addWidget(logoutButton);
            QWidget* header = new QWidget(this);
            header->setLayout(headerLayout);
            layout->insertWidget(0, header);
            connect(logoutButton, &QPushButton::clicked, this, [=]() {
                layout->removeWidget(header);
				delete header;
				navigateToLogin();
                });
            // Navigate to home page
            navigateToHome();

            // Make sure QSystemTrayIcon is initialized and shown
			QIcon* icon = new QIcon("../../../../assets/images/bell.png");
            QSystemTrayIcon* trayIcon = new QSystemTrayIcon(*icon, this);
            trayIcon->show();
            // Show a balloon message
            while (!user->getNotifications().empty()) {
                Notification notification = user->getNotifications().front();
                user->getNotifications().pop();
                trayIcon->showMessage(notification.getTitle().c_str(), notification.getMessage().c_str(), QSystemTrayIcon::Information, 1000);
            }
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

    m_homePage->setUserBalance(QString::asprintf("$%.2f", dynamic_cast<User*>(Database::getInstance()->getCurrentAccount())->getBalance()));
    m_stackedWidget->setCurrentWidget(m_homePage);
}

void CashatakMainWindow::navigateToSendPaymentRequest()
{
    m_stackedWidget->setCurrentWidget(m_requestMoneyPage);
}

void CashatakMainWindow::navigateToSendMoney()
{
    m_stackedWidget->setCurrentWidget(m_sendMoneyPage);
}

void CashatakMainWindow::navigateToTransactionsHistory()
{
    if (m_currentAccount && m_currentAccount->getAccountType() == "User") {
        User* currentUser = dynamic_cast<User*>(m_currentAccount);
        if (currentUser) {
            m_transactionsHistoryPage->loadUserData(currentUser); // Load data before showing
            m_stackedWidget->setCurrentWidget(m_transactionsHistoryPage);
        } else {
            // This case should ideally not happen if m_currentAccount type is "User"
            // but good to handle defensively.
            QMessageBox::critical(this, "Error", "Failed to cast current account to User type.");
            navigateToLogin(); // Or some other appropriate action
        }
    } else {
        // No user logged in or account is not a User type
        QMessageBox::warning(this, "Login Required", "You must be logged in as a user to view transaction history.");
        navigateToLogin(); // Redirect to login
    }
}

void CashatakMainWindow::navigateToAdmin()
{
    m_stackedWidget->setCurrentWidget(m_adminPage);
}

#include "RegisterPage.h"
#include "ui_RegisterPage.h"
#include "core/Database.h"      
#include "core/User.h"          
#include "core/Account.h"       
#include <QMessageBox>          
#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>
#include "ui/windows/CashatakMainWindow.h" 

RegisterPage::RegisterPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterPage)
{
    ui->setupUi(this);
    
    // Get the form layout
    QFormLayout* formLayout = findChild<QFormLayout*>("formLayout");
    if (formLayout) {
        // Create email field and label
        QLineEdit* emailField = new QLineEdit(this);
        emailField->setObjectName("lineEditEmail");
        QLabel* emailLabel = new QLabel("Email:", this);
        
        // Insert them after username (which is at row 0)
        formLayout->insertRow(1, emailLabel, emailField);
    }
}

RegisterPage::~RegisterPage()
{
    delete ui;
}

bool isValidEmail(const QString& email) {
    // Basic email validation: contains @ and at least one . after @
    int atIndex = email.indexOf('@');
    if (atIndex <= 0) return false; // @ must exist and not be first char
    
    QString domain = email.mid(atIndex + 1);
    return domain.contains('.');
}

void RegisterPage::on_pushButtonRegister_clicked()
{
    QString username = ui->lineEditUsername->text().trimmed();
    QString password = ui->lineEditPassword->text();
    QString confirmPassword = ui->lineEditConfirmPassword->text();
    
    // Get email from the dynamically created field
    QLineEdit* emailField = findChild<QLineEdit*>("lineEditEmail");
    QString email = emailField ? emailField->text().trimmed() : "";

    // Basic Validation
    if (username.isEmpty() || email.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "Registration Failed", "All fields are required.");
        return;
    }

    // Validate email format
    if (!isValidEmail(email)) {
        QMessageBox::warning(this, "Registration Failed", "Please enter a valid email address.");
        return;
    }

    if (password != confirmPassword) {
        QMessageBox::warning(this, "Registration Failed", "Passwords do not match.");
        return;
    }

    std::string usernameStr = username.toStdString();
    std::string emailStr = email.toStdString();
    std::string passwordStr = password.toStdString();

    Database* db = Database::getInstance();

    // Check if user already exists
    if (db->getAccount(usernameStr) != nullptr) {
        QMessageBox::warning(this, "Registration Failed", "Username already exists. Please choose another one.");
        return;
    }

    try {
        User* newUser = new User(usernameStr, emailStr, passwordStr, 0.0); 
        db->addAccount(newUser);
        
        // Remove the real-time save to file
        // if (!db->saveToFiles()) {
        //     QMessageBox::warning(this, "Registration Warning", 
        //         "Account created in memory, but there was an issue saving to file. Your account may not persist after closing the application.");
        // }
        
        QMessageBox::information(this, "Registration Successful", "Account created successfully! You can now log in.");

        // Navigate to Login Page
        CashatakMainWindow *mainWindow = nullptr;
        QWidget *ancestor = this;
        while(ancestor && !(mainWindow = qobject_cast<CashatakMainWindow*>(ancestor))) {
            ancestor = ancestor->parentWidget();
        }
        
        if (mainWindow) {
            mainWindow->navigateToLogin();
        } else {
            qWarning() << "RegisterPage: Could not find CashatakMainWindow instance for login navigation.";
            emit navigateToLoginRequested(); 
        }

    } catch (const std::runtime_error& e) {
        QMessageBox::critical(this, "Registration Error", QString("An error occurred: %1").arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "Registration Error", "An unexpected error occurred during registration.");
    }
}

void RegisterPage::on_pushButtonGoToLogin_clicked()
{
    CashatakMainWindow *mainWindow = nullptr;
    QWidget *ancestor = this;
    while(ancestor && !(mainWindow = qobject_cast<CashatakMainWindow*>(ancestor))) {
        ancestor = ancestor->parentWidget();
    }
    
    if (mainWindow) {
        mainWindow->navigateToLogin();
    } else {
        qWarning() << "RegisterPage: Could not find CashatakMainWindow instance for login navigation.";
        emit navigateToLoginRequested(); 
    }
}
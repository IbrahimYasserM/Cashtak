#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>

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
    void loginRequested(const QString& username, const QString& password); // Example: emit when login button clicked

private slots:
    void on_pushButtonLogin_clicked();
    void on_pushButtonGoToRegister_clicked();

private:
    Ui::LoginPage *ui;
};

#endif // LOGINPAGE_H 
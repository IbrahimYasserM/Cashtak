#ifndef REGISTERPAGE_H
#define REGISTERPAGE_H

#include <QWidget>

namespace Ui {
class RegisterPage;
}

class RegisterPage : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterPage(QWidget *parent = nullptr);
    ~RegisterPage();

signals:
    void navigateToLoginRequested();
    void registerRequested(const QString& username, const QString& password);

private slots:
    void on_pushButtonRegister_clicked();
    void on_pushButtonGoToLogin_clicked();

private:
    Ui::RegisterPage *ui;
};

#endif // REGISTERPAGE_H 
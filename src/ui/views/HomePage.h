#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include "../../core/Account.h"

namespace Ui {
class HomePage;
}

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);
    ~HomePage();

    void setUserBalance(const QString& balance); // Method to update balance display

signals:
    void navigateToSendPaymentRequestRequested();
    void navigateToSendMoneyRequested();
    void navigateToTransactionsHistoryRequested();
    void logoutRequested(); // Example for a logout button if you add one

private slots:
    void on_pushButtonSendPaymentRequest_clicked();
    void on_pushButtonSendMoney_clicked();
    void on_pushButtonShowTransactions_clicked();
    // Add a slot for logout if you add a logout button

private:
    Ui::HomePage *ui;
};

#endif // HOMEPAGE_H 
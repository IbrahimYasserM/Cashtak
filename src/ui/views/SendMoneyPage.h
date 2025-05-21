#ifndef SENDMONEYPAGE_H
#define SENDMONEYPAGE_H

#include <QWidget>
#include <QMessageBox>
#include "../core/Database.h"
#include "../core/User.h"
#include "../core/Transaction.h"

namespace Ui {
    class SendMoneyPage;
}

class SendMoneyPage : public QWidget
{
    Q_OBJECT

public:
    explicit SendMoneyPage(QWidget* parent = nullptr);
    ~SendMoneyPage();

    // Method to update user balance display
    void updateBalanceDisplay();

signals:
    void navigateToHomeRequested();
    void transactionCompleted(Transaction* transaction);

private slots:
    void on_pushButtonBackToHome_clicked();
    void on_pushButtonSend_clicked();

private:
    Ui::SendMoneyPage* ui;
    Database* database;
};

#endif // SENDMONEYPAGE_H

// TransactionsHistoryPage.h
#ifndef TRANSACTIONSHISTORYPAGE_H
#define TRANSACTIONSHISTORYPAGE_H

#include <QWidget>
#include <vector>
#include "../core/Transaction.h"
#include "../core/PaymentRequest.h"
#include "../core/User.h"

namespace Ui {
    class TransactionsHistoryPage;
}

class TransactionsHistoryPage : public QWidget
{
    Q_OBJECT

public:
    explicit TransactionsHistoryPage(QWidget* parent = nullptr);
    ~TransactionsHistoryPage();

    // Method to load user data
    void loadUserData(User* currentUser);

signals:
    void navigateToHomeRequested();

private slots:
    void on_pushButtonBackToHome_clicked();
    void on_pushButtonViewAllTransactions_clicked();
    void on_pushButtonViewIncomingRequests_clicked();
    void on_pushButtonViewOutgoingRequests_clicked();

private:
    Ui::TransactionsHistoryPage* ui;
    User* m_currentUser;

    // Helper methods
    void displayRecentTransactions();
    void showAllTransactions();
    void showIncomingRequests();
    void showOutgoingRequests();
    void setupTransactionTable();
    void setupRequestTable();
    void clearTable();

    // Add these missing methods
    void addTransactionToTable(Transaction* transaction);
    void addRequestToTable(const PaymentRequest& request);
};

#endif // TRANSACTIONSHISTORYPAGE_H

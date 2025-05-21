#ifndef TRANSACTIONSHISTORYPAGE_H
#define TRANSACTIONSHISTORYPAGE_H

#include <QWidget>
#include <vector>
#include <QDateTime>

// Forward declarations to avoid full header includes if possible,
// reducing compilation dependencies. Full includes will be in .cpp.
class User;
class Transaction;
class PaymentRequest;

namespace Ui {
class TransactionsHistoryPage;
}

class TransactionsHistoryPage : public QWidget
{
    Q_OBJECT

public:
    explicit TransactionsHistoryPage(QWidget *parent = nullptr);
    ~TransactionsHistoryPage();

    void loadUserData(User* currentUser);

signals:
    void navigateToHomeRequested();

private slots:
    void on_pushButtonViewAllTransactions_clicked();
    void on_pushButtonViewIncomingRequests_clicked();
    void on_pushButtonViewOutgoingRequests_clicked();
    void on_pushButtonBackToHome_clicked();
    // Added for Phase 1 of Accept/Decline plan
    void handleAcceptRequest(int requestId);
    void handleDeclineRequest(int requestId);

private:
    Ui::TransactionsHistoryPage *ui;
    User* m_currentUser;
    // Assuming Database instance is retrieved globally or passed if needed for Phases 3 & 4
    // For now, it's not a member here. Will be Database::getInstance() in slots.

    void setupTransactionTable();
    void setupRequestTable(); 
    void clearTable();
    void addTransactionToTable(Transaction* transaction);
    void addRequestToTable(const PaymentRequest& request); 
    void displayRecentTransactions();
};

#endif // TRANSACTIONSHISTORYPAGE_H

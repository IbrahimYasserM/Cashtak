#pragma once

#include <QDialog>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <vector>
#include "../../core/Transaction.h"

class TransactionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransactionsDialog(const std::vector<Transaction*>& transactions, 
                               const QString& title = "Transactions",
                               QWidget *parent = nullptr);
    ~TransactionsDialog();

private:
    QTableWidget* tableTransactions;
    QPushButton* closeButton;
    void setupUI();
    void populateTable(const std::vector<Transaction*>& transactions);
};

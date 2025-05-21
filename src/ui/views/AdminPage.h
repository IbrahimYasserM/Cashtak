#pragma once

#include <QWidget>
#include <QTableWidgetItem>
#include <QFormLayout>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QDialog>
#include "../../core/User.h"
#include "../../core/Admin.h"
#include "../../core/Database.h"
#include "../dialogs/TransactionsDialog.h"

namespace Ui {
class AdminPage;
}

class AdminPage : public QWidget
{
    Q_OBJECT

public:
    explicit AdminPage(QWidget *parent = nullptr);
    ~AdminPage();

public slots:
    void refreshUserTable();

signals:
    void navigateToHomeRequested();

private slots:
    void on_pushButtonBackHome_clicked();
    void on_pushButtonRefresh_clicked();
    void on_pushButtonSearch_clicked();
    void on_pushButtonEditBalance_clicked();
    void on_pushButtonToggleStatus_clicked(); // New slot for toggling status
    void on_pushButtonDeleteUser_clicked();
    void on_pushButtonAddUser_clicked();
    void on_pushButtonViewAllTransactions_clicked();
    void on_userTransactionButtonClicked();

private:
    Ui::AdminPage *ui;
    Admin* admin = nullptr;
    std::vector<User*> userList;
    
    void setupConnections();
    void loadUsers();
    void searchUsers(const QString& searchTerm);
    std::vector<std::string> getAllUsernames();
    void setupUserTransactionButtons();
};
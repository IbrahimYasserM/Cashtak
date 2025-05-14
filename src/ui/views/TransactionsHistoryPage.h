#ifndef TRANSACTIONSHISTORYPAGE_H
#define TRANSACTIONSHISTORYPAGE_H

#include <QWidget>

namespace Ui {
class TransactionsHistoryPage;
}

class TransactionsHistoryPage : public QWidget
{
    Q_OBJECT

public:
    explicit TransactionsHistoryPage(QWidget *parent = nullptr);
    ~TransactionsHistoryPage();

signals:
    void navigateToHomeRequested();

private slots:
    void on_pushButtonBackToHome_clicked();

private:
    Ui::TransactionsHistoryPage *ui;
};

#endif // TRANSACTIONSHISTORYPAGE_H 
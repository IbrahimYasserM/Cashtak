#ifndef SENDMONEYPAGE_H
#define SENDMONEYPAGE_H

#include <QWidget>

namespace Ui {
class SendMoneyPage;
}

class SendMoneyPage : public QWidget
{
    Q_OBJECT

public:
    explicit SendMoneyPage(QWidget *parent = nullptr);
    ~SendMoneyPage();

signals:
    void navigateToHomeRequested();

private slots:
    void on_pushButtonBackToHome_clicked();

private:
    Ui::SendMoneyPage *ui;
};

#endif // SENDMONEYPAGE_H 
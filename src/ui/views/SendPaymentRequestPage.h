#ifndef SENDPAYMENTREQUESTPAGE_H
#define SENDPAYMENTREQUESTPAGE_H

#include <QWidget>

namespace Ui {
class SendPaymentRequestPage;
}

class SendPaymentRequestPage : public QWidget
{
    Q_OBJECT

public:
    explicit SendPaymentRequestPage(QWidget *parent = nullptr);
    ~SendPaymentRequestPage();

signals:
    void navigateToHomeRequested();

private slots:
    void on_pushButtonBackToHome_clicked();

private:
    Ui::SendPaymentRequestPage *ui;
};

#endif // SENDPAYMENTREQUESTPAGE_H 
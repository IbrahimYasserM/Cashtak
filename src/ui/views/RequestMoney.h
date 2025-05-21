#ifndef REQUESTMONEY_H
#define REQUESTMONEY_H

#include <QWidget>
#include "core/Database.h" // Assuming path relative to project root or include paths are set
#include "core/User.h"       // Assuming path
#include "core/PaymentRequest.h" // Assuming path
#include <random>
#include <limits>

namespace Ui {
class RequestMoney; // Forward declaration for the UI class
}

class RequestMoney : public QWidget
{
    Q_OBJECT

public:
    explicit RequestMoney(QWidget *parent = nullptr);
    ~RequestMoney();

signals:
    void navigateToHomeRequested();
    void paymentRequestSent(PaymentRequest* request);

private slots:
    void on_pushButtonBackToHome_clicked();
    void on_pushButtonSendRequest_clicked();

private:
    Ui::RequestMoney *ui; // Pointer to the UI class
    Database* database;
};

#endif // REQUESTMONEY_H 
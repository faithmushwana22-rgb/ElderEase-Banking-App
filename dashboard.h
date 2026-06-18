#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>

namespace Ui {
class Dashboard;
}

class Dashboard : public QWidget
{
    Q_OBJECT

public:
    explicit Dashboard(QString username, QString pin, QWidget *parent = 0);
    ~Dashboard();


private slots:
    void goToDeposit();
    void goToWithdraw();
    void goToSendMoney();
    void goToTransactions();

    void depositMoney();
    void backToDashboard();

    void moneyWithdraw();
    void withdraw50();
    void withdraw100();
    void withdraw200();
    void withdraw500();
    void printStatement();

    void sendMoney();
    void saveTransactionToFile(QString type, double amount);
    void loadTransactions();
    void saveBalance();
    void loadBalance();
    void refreshUI();

    bool verifyPin();


private:
    Ui::Dashboard *ui;
    double balance = 0.00;
    void processWithdraw(double amount);
    QString correctPin;
    QString currentUsername;
    QString accountNumber;
    int pinAttempts = 0;
    bool accountLocked = false;

    void unlockAccount();
    void addTransaction(QString type, double amount);
};

#endif // DASHBOARD_H

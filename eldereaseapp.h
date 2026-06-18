#ifndef ELDEREASEAPP_H
#define ELDEREASEAPP_H
#include "loginpage.h"

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>

namespace Ui {
class ElderEaseApp;
}

class LoginPage;

class ElderEaseApp : public QMainWindow
{
    Q_OBJECT

public:
    explicit ElderEaseApp(QWidget *parent = 0);
    ~ElderEaseApp();
    QString registeredUsername;
    QString registeredPin;

private:
    Ui::ElderEaseApp *ui;
    LoginPage *loginPage = 0;
private slots:
    void handleSignUp();
    void handleCancel();
    void goToLogin();
};

#endif // ELDEREASEAPP_H

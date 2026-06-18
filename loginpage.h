#ifndef LOGINPAGE_H
#define LOGINPAGE_H
#include "eldereaseapp.h"

#include <QWidget>

namespace Ui {
class LoginPage;
}

//Forward declaration
class ElderEaseApp;

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(ElderEaseApp * mainApp, QWidget *parent = 0);
    explicit LoginPage(QWidget *parent = 0);
    ~LoginPage();
    QString enteredPin;
    QString username;

private:
    Ui::LoginPage *ui;
    ElderEaseApp *app;

private slots:
    void goToSignup();
    void handleLogin();
};

#endif // LOGINPAGE_H

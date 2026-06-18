#include "loginpage.h"
#include "ui_loginpage.h"
#include "eldereaseapp.h"
#include "dashboard.h"
#include <QMessageBox>

LoginPage::LoginPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginPage)
{
    ui->setupUi(this);
    ui->pinEdit->setEchoMode(QLineEdit::Password);

    //Connections here
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginPage::handleLogin);
    connect(ui->signupRedirectButton, &QPushButton::clicked, this , &LoginPage::goToSignup);
}

LoginPage::~LoginPage()
{
    delete ui;
}

LoginPage::LoginPage(ElderEaseApp *mainApp, QWidget *parent)
    : QWidget(parent),ui(new Ui::LoginPage), app(mainApp)
{
    ui->setupUi(this);

    //Connect login button
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginPage::handleLogin);

    ui->pinEdit->setEchoMode(QLineEdit::Password);
}

void LoginPage::goToSignup()
{
    //Hide the login window
    this->hide();

    //Show the main app with signup
    ElderEaseApp *signup = new ElderEaseApp();
    signup->show();
}

void LoginPage::handleLogin()
{
    //Get user information
    QString username, pin;
    username = ui->uNameEdit->text();
    pin = ui->pinEdit->text();

    //Making sure the creditials are entered correctly- SAFETY PURPOSES
    if(username == app->registeredUsername && pin == app->registeredPin)
    {
        QMessageBox::information(this, "Login Successful", "Welcome back!");

        Dashboard *dashboard = new Dashboard(username, pin);
        dashboard->show();

        this->hide();
    }
    else
    {
        QMessageBox::warning(this, "Login Failed", "Incorrect username or PIN!");
        ui->uNameEdit->clear();
        ui->pinEdit->clear();
    }
}

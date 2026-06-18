#include "eldereaseapp.h"
#include "ui_eldereaseapp.h"
#include "loginpage.h"
#include <QMessageBox>

ElderEaseApp::ElderEaseApp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ElderEaseApp)
{
    ui->setupUi(this);

    //Connection of buttons here
    connect(ui->signButton, &QPushButton::clicked, this, &ElderEaseApp::handleSignUp);
    connect(ui->cancelButton, &QPushButton::clicked, this, &ElderEaseApp::handleCancel);
    connect(ui->loginRedirectButton, &QPushButton::clicked, this , &ElderEaseApp::goToLogin);


    //Hiding the pins
    ui->pinEdit->setEchoMode(QLineEdit::Password);
    ui->confirmEdit->setEchoMode(QLineEdit::Password);
}

ElderEaseApp::~ElderEaseApp()
{
    delete ui;
}

void ElderEaseApp::handleSignUp()
{
    QString Username = ui->nameEdit->text();
    QString pin = ui->pinEdit->text();
    QString confirmPin = ui->confirmEdit->text();

    //Checking that all fields are filled in correctly
    if (Username.isEmpty() || pin.isEmpty() || confirmPin.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please enter all fields!");
        return;
    }

    //Checking that the pin matches
    if (pin != confirmPin)
    {
        QMessageBox::warning(this, "Error", "Pins do not match!");
        return;
    }
    //Save credintials
    registeredUsername = Username;
    registeredPin = pin;


    //Showing the a successful message and that they can proceed to the next page
    QMessageBox::information(this, "Success", "Successful sign up, proceed to log-in");

    //Clearing the fields
    ui->nameEdit->clear();
    ui->pinEdit->clear();
    ui->confirmEdit->clear();

    //Open the next window
    LoginPage *loginWindow = new LoginPage(this);
    loginWindow->show();

    //Hide this window
    this->hide();
}

void ElderEaseApp::handleCancel()
{
    ui->nameEdit->clear();
    ui->pinEdit->clear();
    ui->confirmEdit->clear();

    QMessageBox::information(this, "Cleared", "All fields cleared!");
}

void ElderEaseApp::goToLogin()
{
    if(!loginPage)
    {
        loginPage = new LoginPage(this);
    }
    loginPage->show();
    this->hide();
}

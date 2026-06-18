#include "SignupPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

SignupPage::SignupPage(QWidget *parent) : QWidget(parent)
{
    //Title label
    titleLabel = new QLabel("Welcome to ElderEase banking App!");
    titleLabel ->setStyleSheet("color:#FF69B4; font-size: 28px; font-weight: bold;");
    titleLabel ->setAlignment(Qt::AlignCenter);

    //Input Fields
    nameEdit = new QLineEdit();
    nameEdit ->setPlaceholderText("Enter your name");

    emailEdit = new QLineEdit();
    emailEdit ->setPlaceholderText("Enter your email");

    pinEdit = new QLineEdit();
    pinEdit ->setPlaceholderText("Enter a 4-digit pin");
    pinEdit ->setEchoMode(QLineEdit::Password);

    confirmEdit = new QLineEdit();
    confirmEdit ->setPlaceholderText("Confirm Pin");
    confirmEdit ->setEchoMode(QLineEdit::Password);

    //Buttons
    signButton = new QPushButton("Sign Up");
    cancelButton = new QPushButton("Cancel");

    //Styling buttons
    signButton ->setStyleSheet("background-color: #87CEFA; font-size: 16px; padding: 8px; border-radius: 10px;");
    cancelButton ->setStyleSheet("background-color: #F08080; font-size: 16px; padding: 8px; border-radius: 10px;");

    //Message area
    textBrowser = new QTextBrowser();
    textBrowser ->setText("Please in all fields to create your account");
    textBrowser ->setFixedHeight(50);
    textBrowser ->setStyleSheet("background-color: #FFF0F5; font-size: 14px;");

    //Layout
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(nameEdit);
    mainLayout->addWidget(emailEdit);
    mainLayout->addWidget(pinEdit);
    mainLayout->addWidget(confirmEdit);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(signButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(textBrowser);

    setLayout(mainLayout);

    setWindowTitle("ElderEase - Sign Up");
    setFixedSize(400,400);
    setStyleSheet("background-color: #FFF8DC;");

    //Connect buttons
    connect(signButton, &QPushButton::clicked,this,&SignupPage::handleSignUp);
    connect(cancelButton, &QPushButton::clicked,this,&SignupPage::handleCancel);
}

void SignupPage::handleSignUp()
{
    QString name = nameEdit->text();
    QString email = emailEdit->text();
    QString pin = pinEdit->text();
    QString confirmPin = confirmEdit->text();

    if (name.isEmpty() || email.isEmpty() || pin.isEmpty() || confirmPin.isEmpty())
    {
        textBrowser->setText("Please fill in all fields");
        return;
    }
    if (pin != confirmPin)
    {
        textBrowser->setText("Pins do not match!");
        return;
    }
    textBrowser->setText("Sign-up successful. Proceed to log-in!");
}

void SignupPage::handleCancel()
{
    close();
}

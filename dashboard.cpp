#include "dashboard.h"
#include "ui_dashboard.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QHeaderView>
#include <QDate>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QDebug>
#include <QPagedPaintDevice>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QDate>
#include <QPixmap>

Dashboard::Dashboard(QString username, QString pin, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dashboard)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    currentUsername = username;
    accountNumber = "5521 45" + QString::number(qrand() % 9000 + 1000);
    ui->welcomeLabel->setText("Welcome, " + username);

    this->correctPin = pin;
    loadBalance();
    refreshUI();

    ui->availBalLabel->setText("R" + QString::number(balance, 'f', 2));
    ui->balanceLabel_2->setText("R" + QString::number(balance, 'f', 2));
    ui->amountLabel->setText("R" + QString::number(balance, 'f', 2));

    ui->transactionsTableWidget->setColumnCount(3);
    QStringList headers;
    headers<< "Date" << "Type" << "Amount";
    ui->transactionsTableWidget->setHorizontalHeaderLabels(headers);
    ui->transactionsTableWidget->horizontalHeader()->setStretchLastSection(true);

    ui->transactionsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    loadTransactions();
    //Connection of buttons
    connect(ui->depositButton, &QPushButton::clicked, this, &Dashboard::goToDeposit);
    connect(ui->withdrawButton,&QPushButton::clicked, this, &Dashboard::goToWithdraw);
    connect(ui->sendButton, &QPushButton::clicked, this, &Dashboard::goToSendMoney);
    connect(ui->transactionsButton, &QPushButton::clicked, this, &Dashboard::goToTransactions);
    connect(ui->confirmButton, &QPushButton::clicked, this, &Dashboard::depositMoney);
    connect(ui->backButton, &QPushButton::clicked, this, &Dashboard::backToDashboard);

    connect(ui->withdrawButton_2, &QPushButton::clicked, this, &Dashboard::moneyWithdraw);
    connect(ui->R50Button, &QPushButton::clicked, this, &Dashboard::withdraw50);
    connect(ui->R100Button, &QPushButton::clicked, this, &Dashboard::withdraw100);
    connect(ui->R200Button, &QPushButton::clicked, this, &Dashboard::withdraw200);
    connect(ui->R500Button, &QPushButton::clicked, this, &Dashboard::withdraw500);

    connect(ui->backButton_2, &QPushButton::clicked, this, &Dashboard::backToDashboard);
    connect(ui->backButton_3, &QPushButton::clicked, this, &Dashboard::backToDashboard);
    connect(ui->backButton_4, &QPushButton::clicked, this, &Dashboard::backToDashboard);

    connect(ui->printButton, &QPushButton::clicked, this, &Dashboard::printStatement);

    connect(ui->sendButton_2, &QPushButton::clicked, this, &Dashboard::sendMoney);
}

void Dashboard::backToDashboard()
{
    ui->stackedWidget->setCurrentWidget(ui->dashboardPage);
}

void Dashboard::goToDeposit()
{
    ui->stackedWidget->setCurrentWidget(ui->depositPage);
    ui->balanceLabel_2->setText("Current Balance: R" + QString::number(balance, 'f', 2));
}

void Dashboard::goToWithdraw()
{
    ui->stackedWidget->setCurrentWidget(ui->withdrawPage);
    ui->availBalLabel->setText("R" + QString::number(balance, 'f', 2));
}

void Dashboard::goToSendMoney()
{
    ui->stackedWidget->setCurrentWidget(ui->page);
}

void Dashboard::goToTransactions()
{
    ui->stackedWidget->setCurrentWidget(ui->transactionsPage);
}

void Dashboard::depositMoney()
{
    double amount = ui->amountEdit->text().toDouble();
    if(amount > 0)
    {
        balance += amount;
        saveBalance();
        addTransaction("Deposit", amount);
        saveTransactionToFile("Deposit", amount);

        ui->balanceLabel_2->setText("R" + QString::number(balance, 'f', 2));
        ui->amountLabel->setText("R" + QString::number(balance, 'f', 2));
        ui->amountEdit->clear();

        QMessageBox::information(this, "Deposit Successful", "ElderEase Alert: R" + QString::number(amount, 'f', 2) + "has been deposited into your account.");

        ui->cashRadioButton->setAutoExclusive(false);
        ui->cashRadioButton->setChecked(false);

        ui->bankRadioButton->setAutoExclusive(false);
        ui->bankRadioButton->setChecked(false);

        ui->cashRadioButton->setAutoExclusive(true);
        ui->bankRadioButton->setAutoExclusive(true);
    }

}


void Dashboard::moneyWithdraw()
{
    double amount = ui->amtEdit->text().toDouble();
    processWithdraw(amount);
}

void Dashboard::withdraw50()
{
    processWithdraw(50);
}

void Dashboard::withdraw100()
{
    processWithdraw(100);
}

void Dashboard::withdraw200()
{
    processWithdraw(200);
}

void Dashboard::withdraw500()
{
    processWithdraw(500);
}

void Dashboard::processWithdraw(double amount)
{
    if(!verifyPin())
    {
        return;
    }

    if (amount > 0 && amount <= balance)
    {
        balance -= amount;
        saveBalance();
        addTransaction("Withdraw", amount);
        saveTransactionToFile("Withdraw", amount);

        ui->availBalLabel->setText("R" + QString::number(balance, 'f', 2));
        ui->amountLabel->setText("R" + QString::number(balance, 'f', 2));

        ui->amtEdit->clear();

        QMessageBox::information(this, "Withdrawal Successful", "ElderEase Alert: R" + QString::number(amount,'f',2) + " has been withdrawn from your account.");
    }
    else
    {
        QMessageBox::warning(this, "Insufficient Funds", "Your balance is too low for this withdrawal");
    }
}

bool Dashboard::verifyPin()
{
    if(accountLocked)
    {
        QMessageBox::critical(this, "Account Locked", "Your account has been locked due to multiple incorrect PIN attempts.");
        return false;
    }
    bool ok;
    QString pin = QInputDialog::getText(this, "Security Check", "Enter your login PIN to continue:", QLineEdit::Password,"",&ok);
    if(!ok) return false;
    qDebug()<<"Enter Pin:" <<pin;
    qDebug()<<"CorrectPin:" <<correctPin;
    if(pin.trimmed() == correctPin.trimmed())
    {
        pinAttempts = 0;
        return true;
    }
    else
    {
        pinAttempts++;
        QMessageBox::warning(this,"Incorrect PIN","Wrong PIN entered. Attempt " + QString::number(pinAttempts) + " of 3.");

        if(pinAttempts >= 3)
        {
            accountLocked = true;
            QMessageBox::critical(this, "Account Locked", "Too many incorrect attempts. \nPlease unlock your account.");

            unlockAccount();
            return false;
        }
    }

    return false;
}

void Dashboard::unlockAccount()
{
    bool ok;
    QString pin = QInputDialog::getText(this, "Unlock Account", "Enter your PIN to unlock account:", QLineEdit::Password,"",&ok);

    if(!ok)
        return;

    if(pin == correctPin)
    {
        accountLocked = false;
        pinAttempts = 0;

        QMessageBox::information(this,"Account Unlocked","Your Account has been successfully unlocked.");
    }
    else
    {
        QMessageBox::warning(this,"Incorrect PIN","Unable to unlock account.");
    }
}

void Dashboard::addTransaction(QString type, double amount)
{
    int row = ui->transactionsTableWidget->rowCount();
    ui->transactionsTableWidget->insertRow(row);
    QString date = QDate::currentDate().toString("dd MMM");
    ui->transactionsTableWidget->setItem(row, 0, new QTableWidgetItem(date));
    ui->transactionsTableWidget->setItem(row, 1, new QTableWidgetItem(type));

    QString amountText;
    if(type == "Deposit")
    {
        amountText = "+R" + QString::number(amount, 'f', 2);
    }
    else
    {
        amountText = "-R" + QString::number(amount, 'f', 2);
    }
    ui->transactionsTableWidget->setItem(row, 2, new QTableWidgetItem(amountText));
}

void Dashboard::saveTransactionToFile(QString type, double amount)
{
    QFile file(currentUsername + "_transactions.txt");

    if(file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        QString date = QDate::currentDate().toString("dd MMM yyyy");
        out << date <<"," << type << "," << amount << "\n";

        file.close();
    }
}

void Dashboard::loadTransactions()
{
    QFile file(currentUsername + "_transactions.txt");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);

        ui->transactionsTableWidget->setRowCount(0);

        while(!in.atEnd())
        {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            if(parts.size() == 3)
            {
                QString date = parts[0];
                QString type = parts[1];
                double amount = parts[2].toDouble();
                int row = ui->transactionsTableWidget->rowCount();
                ui->transactionsTableWidget->insertRow(row);
                ui->transactionsTableWidget->setItem(row, 0, new QTableWidgetItem(date));
                ui->transactionsTableWidget->setItem(row, 1, new QTableWidgetItem(type));

                QString amountText;
                if(type == "Deposit")
                {
                    amountText = "+R" + QString::number(amount, 'f', 2);
                }
                else
                {
                    amountText = "-R" + QString::number(amount, 'f', 2);
                }
                ui->transactionsTableWidget->setItem(row, 2, new QTableWidgetItem(amountText));
            }
        }
        file.close();
    }
}

void Dashboard::printStatement()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save PDF Statement", "ElderEase_Statement.pdf", "*.pdf");

    if(filename.isEmpty())
        return;

    QPdfWriter pdf(filename);

    pdf.setPageSize(QPagedPaintDevice::A4);
    pdf.setResolution(96);

    QPainter painter(&pdf);

    if(!painter.isActive())
    {
        QMessageBox::warning(this, "Error", "Could not create PDF.");
        return;
    }

    int y = 100;

    //BANK Title
    painter.setFont(QFont("Arial", 18, QFont::Bold));
    painter.drawText(200, y, "ELDEREASE BANK");

    y += 40;

    //Date
    painter.setFont(QFont("Arial", 10));
    painter.drawText(200, y, "Official Transaction Statement");

    y += 60;

    //=====USER DETAILS====
    painter.setFont(QFont("Arial", 11, QFont::Bold));

    painter.drawText(200, y, "Account Holder: " + currentUsername);
    y += 30;

    painter.drawText(200, y, "Account Number: " + accountNumber);
    y += 30;

    painter.drawText(200, y, "Current Balance: R" + QString::number(balance, 'f', 2));
    y += 50;

    //Table Headers
    painter.setFont(QFont("Arial", 11, QFont::Bold));

    painter.drawText(200, y, "Date");
    painter.drawText(400, y, "Type");
    painter.drawText(700, y, "Amount");

    y += 10;

    painter.drawLine(200, y, 900, y);

    y += 30;

    //Transactions
    painter.setFont(QFont("Arial", 10));

    for(int row = 0; row < ui->transactionsTableWidget->rowCount(); row++)
    {
        QTableWidgetItem *dateItem = ui->transactionsTableWidget->item(row, 0);
        QTableWidgetItem *typeItem = ui->transactionsTableWidget->item(row, 1);
        QTableWidgetItem *amountItem = ui->transactionsTableWidget->item(row, 2);

        QString date = dateItem ? dateItem->text() : "";
        QString type = typeItem ? typeItem->text() : "";
        QString amount = amountItem ? amountItem->text() : "";

        painter.drawText(200,y,date);
        painter.drawText(400,y,type);
        painter.drawText(700,y,amount);

        y += 25;
    }
    y += 60;

    //====SIGNATURE===
    QPixmap signature(":/images/Elderease signature.png");
    painter.drawPixmap(580, y - 80, 180, 60, signature);
    painter.drawLine(550, y, 800, y);

    y += 20;

    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(610, y + 30, "Authorized Signature");

    y += 20;

    painter.setFont(QFont("Arial", 9));
    painter.drawText(640, y, "ElderEase Bank");

    y += 40;

    //===FOOTER===
    painter.setFont(QFont("Arial", 9));
    painter.drawText(200, y, "Thank you for banking with ElderEase.");

    //DATE
    y += 20;

    QString currentDate = QDate::currentDate().toString("dd/MM/yyyy");
    painter.drawText(200, y, "Generated on: " + currentDate);
    painter.end();

    QMessageBox::information(this,"Statement Generated", "Your PDF bank statement was created successfully.");

}

void Dashboard::sendMoney()
{
    QString recipient = ui->nameEdit->text();
    QString phone = ui->phoneEdit->text();
    double amount = ui->amountEdit_2->text().toDouble();

    //1. Validate input
    if(recipient.isEmpty() || phone.isEmpty() || amount <= 0)
    {
        QMessageBox::warning(this, "Error", "Please fill in all fields correctly.");
        return;
    }

    //2. Confirm action
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Transfer", "Send R" + QString::number(amount, 'f', 2) + " to " + recipient + "?", QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::No)
        return;

    //3. PIN Verification
    if(!verifyPin())
        return;

    //4. Check Balance
    if(amount > balance)
    {
        QMessageBox::warning(this, "Failed", "Insufficient funds.");
        return;
    }

    //5. Process Transfer
    balance -= amount;
    saveBalance();
    ui->amountLabel->setText("R" + QString::number(balance, 'f', 2));

    //6. Transaction log
    addTransaction("Transfer", amount);
    saveTransactionToFile("Transfer", amount);

    //7. Success message
    QMessageBox::information(this, "Transfer Successful", "R" + QString::number(amount, 'f',2) + " sent to " + recipient + "\nRemaining Balance: R" + QString::number(balance, 'f', 2));

    //8. Clear fields
    ui->nameEdit->clear();
    ui->phoneEdit->clear();
    ui->amountEdit->clear();
}

void Dashboard::saveBalance()
{
    QFile file(currentUsername + "_balance.txt");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out<<balance;
        file.close();
    }
}

void Dashboard::loadBalance()
{
    QFile file(currentUsername + "_balance.txt");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        in>>balance;
        file.close();
    }
}

void Dashboard::refreshUI()
{
    ui->availBalLabel->setText("R" + QString::number(balance, 'f', 2));
    ui->balanceLabel_2->setText("R" +QString::number(balance, 'f', 2));
    ui->amountLabel->setText("R" + QString::number(balance, 'f', 2));
}

Dashboard::~Dashboard()
{
    delete ui;
}

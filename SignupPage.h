#ifndef SIGNUPPAGE_H
#define SIGNUPPAGE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextBrowser>

class SignupPage : public QWidget
{
    Q_OBJECT
private:
    QLabel* titleLabel;
    QLineEdit* nameEdit;
    QLineEdit* emailEdit;
    QLineEdit* pinEdit;
    QLineEdit* confirmEdit;
    QPushButton* signButton;
    QPushButton* cancelButton;
    QTextBrowser* textBrowser;
private slots:
    void handleSignUp();
    void handleCancel();
public:
    SignupPage(QWidget* parent = 0);
};

#endif // SIGNUPPAGE_H

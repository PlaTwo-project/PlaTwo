#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

    void clearPass();

signals:
    void loginRequested(const QString& username, const QString& password);
    void navigateToSignup();
    void navigateToForgotPassword();

private slots:
    void on_pushButton_login_clicked();
    void on_pushButton_signup_clicked();
    void on_pushButton_forgotPassword_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_pushButton_login_clicked();
    void on_pushButton_signup_clicked();
    void on_pushButton_forgotPassword_clicked();

signals:
    void goToSignup();
    void goToForgotPassword();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
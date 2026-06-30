#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>

namespace Ui {
class Signup;
}

class Signup : public QWidget
{
    Q_OBJECT

public:
    explicit Signup(QWidget *parent = nullptr);
    ~Signup();

signals:
    void signupRequested(const QString& name, const QString& username, const QString& email, const QString& phone, const QString& password);

    void navigateToLogin();

private slots:
    void on_pushButton_submit_clicked();
    void on_pushButton_login_clicked();

private:
    Ui::Signup *ui;
};

#endif // SIGNUP_H
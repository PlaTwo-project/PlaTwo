#ifndef FORGOT_PASSWORD_H
#define FORGOT_PASSWORD_H

#include <QWidget>
#include "forgot_password2.h"

namespace Ui {
class ForgotPassword;
}

class ForgotPassword : public QWidget
{
    Q_OBJECT

public:
    explicit ForgotPassword(QWidget *parent = nullptr);
    ~ForgotPassword();

private slots:
    void on_pushButton_next_clicked();

    void on_pushButton_back_clicked();

signals:
    void goToPage2();
    void backToLogin();

private:
    Ui::ForgotPassword *ui;
    ForgotPassword2 *next_page;
};

#endif // FORGOT_PASSWORD_H

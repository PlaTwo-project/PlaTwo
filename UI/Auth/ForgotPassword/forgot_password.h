#ifndef FORGOT_PASSWORD_H
#define FORGOT_PASSWORD_H

#include <QWidget>

namespace Ui {
class ForgotPassword;
}

class ForgotPassword : public QWidget
{
    Q_OBJECT

public:
    explicit ForgotPassword(QWidget *parent = nullptr);
    ~ForgotPassword();

signals:
    void forgotPasswordStep2Requested(const QString& username, const QString& phone);
    void navigateToLogin();

private slots:
    void on_pushButton_next_clicked();
    void on_pushButton_back_clicked();

private:
    Ui::ForgotPassword *ui;
};

#endif // FORGOT_PASSWORD_H